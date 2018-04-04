#include "stdafx.h"
#include "ClientNetworking.h"

bool ClientComponent::initialize()
{
	client.connect(enetpp::client_connect_params()
		.set_channel_count(channel_count)
		.set_server_host_name_and_port("localhost", 8888));

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

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
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
	trace("received packet from server : '" +
		std::string(reinterpret_cast<const char*>(data), data_size) + "'");
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



