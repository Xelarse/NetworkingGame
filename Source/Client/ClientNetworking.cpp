#include "stdafx.h"
#include "ClientNetworking.h"

bool ClientComponent::initialize()
{
	client.connect(enetpp::client_connect_params()
		.set_channel_count(channel_count)
		.set_server_host_name_and_port(connecting_ip.c_str(), 8888));

	return true;
}

bool ClientComponent::deinitialize()
{
	client.disconnect();
	enetpp::global_state::get().deinitialize();
	return true;
}

void ClientComponent::consumeEvents()
{
	while (client.is_connecting_or_connected() && !kill_thread)
	{
		using namespace std::placeholders;
		client.consume_events(
			std::bind(&ClientComponent::on_connect, this),
			std::bind(&ClientComponent::on_disconnect, this),
			std::bind(&ClientComponent::on_data, this, _1, _2));

		while (recieved_queue.size())
		{
			std::lock_guard<std::mutex> lock(recieved_mtx);
			
			auto front = recieved_queue.front();
			processRecievedPackets(std::move(front));

			recieved_queue.pop();
		}

		while (sending_queue.size())
		{
			sendFrontOfQueue();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void ClientComponent::on_connect()
{
	trace("on_connected");
}

void ClientComponent::on_disconnect()
{
	trace("on_disconnected");
}

void ClientComponent::on_data(const enet_uint8* data, size_t data_size)
{
	//TODO make a queue when you get flooded with packets you fucking retard

	CustomPacket msg((char*)data);

	std::lock_guard<std::mutex> lock(recieved_mtx);
	recieved_queue.push(std::move(msg));
}

bool ClientComponent::isConnected() const
{
	return 
		client.get_connection_status() == enetpp::client::CONNECTED;
}

bool ClientComponent::isConnecting() const
{
	return
		client.get_connection_status() == enetpp::client::CONNECTING;
}

std::string ClientComponent::getUsername()
{
	std::lock_guard<std::mutex> lock(username_mtx);
	return username;
}

void ClientComponent::setUsername(std::string str)
{
	std::lock_guard<std::mutex> lock(username_mtx);
	username = str;
}

void ClientComponent::sendFrontOfQueue()
{
	std::lock_guard<std::mutex> lock(sending_mtx);

	const auto& msg = sending_queue.front();
	unsigned int msg_length = 0;
	auto msg_data = msg.data(msg_length);

	sending_queue.pop();

	assert(sizeof(char) == sizeof(enet_uint8));
	client.send_packet(0, reinterpret_cast<enet_uint8*>(msg_data), msg_length, ENET_PACKET_FLAG_RELIABLE);
}

void ClientComponent::processRecievedPackets(CustomPacket msg)
{
	if (msg.getType() == "chat")
	{
		std::lock_guard<std::mutex> lock(chat_recieved_mtx);
		chat_recieved_queue.push(std::move(msg));
	}

	else if (msg.getType() == "init")
	{
		user_ID = std::stoi(msg.getMsg());
	}

	else if (msg.getType() == "unit")
	{
		std::lock_guard<std::mutex> lock(unit_update_mtx);
		unit_update_queue.push(std::move(msg));
	}

	else if (msg.getType() == "turn")
	{
		player_turn = std::stoi(msg.getMsg());
	}

	else if (msg.getType() == "player")
	{
		assigned_player = std::stoi(msg.getMsg());
	}

	else if (msg.getType() == "reconnect")
	{
		if (msg.getMsg() == "true")
		{
			is_reconnecting = true;
			is_lobby = false;
		}

		else if (msg.getMsg() == "false")
		{
			is_reconnecting = false;
			data_sender = false;
		}
	}

	else if (msg.getType() == "start")
	{
		is_lobby = false;
	}

	else if (msg.getType() == "data_sender")
	{
		data_sender = true;
		is_reconnecting = true;
	}

	else if (msg.getType() == "ready")
	{
		ready_to_send = true;
	}

	else if (msg.getType() == "update_complete")
	{
		update_complete = true;
	}
}
