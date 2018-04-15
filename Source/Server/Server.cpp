#include <Common\CustomPacket.h>
#include "Server.h"

void Server::initialise()
{
	network_server.initialize();

	/* 
	Example code of adding lamdba callbacks to the network server
	network_server.onConnected([&](server_client& client)
	{
		network_server.trace("on_client_connected");
	});

	network_server.onDisconnected([&](unsigned int client_uid)
	{
		network_server.trace("on_client_disconnected");
	}); 
	*/

	// The function called when data is recevied over the network.
	using namespace std::placeholders;
	network_server.onData(std::bind(&Server::onClientData, this, _1, _2, _3));
	network_server.onConnected(std::bind(&Server::onClientConnect, this, _1));
	network_server.onDisconnected(std::bind(&Server::onClientDisconnect, this, _1));
}

int Server::run()
{
	// while Delete key has not been pressed
	while (!GetAsyncKeyState(VK_DELETE))
	{
		network_server.consumeEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	network_server.deinitialize();
	return 0;
}

void Server::onClientData(server_client& client, const enet_uint8* data, size_t data_size)
{
	CustomPacket msg((char*)data);

	if (msg.getType() == "chat")
	{
		network_server.trace(
			"received packet from client " +
			std::to_string(client.get_id()) +
			": via the username: " +
			msg.getUsername() +
			" => '" +
			msg.getMsg() + 
			"'\n");

		network_server.trace("Forwarding to all other clients");
		network_server.getServer()->send_packet_to_all_if(0, data, data_size, ENET_PACKET_FLAG_RELIABLE,
			[&](const server_client& destination) {return destination.get_id() != client.get_id(); });
	}

	if (msg.getType() == "unit")
	{
		network_server.trace(
			"received packet from client " +
			std::to_string(client.get_id()) +
			": which contains unit data\n"
		);

		network_server.trace("Forwarding to all other clients");
		network_server.getServer()->send_packet_to_all_if(0, data, data_size, ENET_PACKET_FLAG_RELIABLE,
			[&](const server_client& destination) {return destination.get_id() != client.get_id(); });
	}

	if (msg.getType() == "turn")
	{
		network_server.trace(
			"received packet from client " +
			std::to_string(client.get_id()) +
			": which contains the turn of current game\n"
		);

		network_server.trace("forwarding onto other client");
		network_server.getServer()->send_packet_to_all_if(0, data, data_size, ENET_PACKET_FLAG_RELIABLE,
			[&](const server_client& destination) {return destination.get_id() != client.get_id(); });
	}

	if (msg.getType() == "player")
	{
		network_server.trace(
			"received packet from client " +
			std::to_string(client.get_id()) +
			": which contains the other players team info\n"
		);

		network_server.trace("forwarding onto other client");
		network_server.getServer()->send_packet_to_all_if(0, data, data_size, ENET_PACKET_FLAG_RELIABLE,
			[&](const server_client& destination) {return destination.get_id() != client.get_id(); });
	}

	if (msg.getType() == "update_complete")
	{
		network_server.trace(
			"received packet from client " +
			std::to_string(client.get_id()) +
			": which says that all update packets have been sent\n"
		);

		network_server.trace("forwarding onto other client");
		network_server.getServer()->send_packet_to_all_if(0, data, data_size, ENET_PACKET_FLAG_RELIABLE,
			[&](const server_client& destination) {return destination.get_id() != client.get_id(); });
	}


	if (msg.getType() == "resume")
	{
		network_server.trace(
			"received packet from client " +
			std::to_string(client.get_id()) +
			": which says that the reconnect is complete\n allowing players to resume\n"
		);

		auto clients = network_server.getServer()->get_connected_clients();

		CustomPacket completed("reconnect", "", "false");

		unsigned int complete_length = 0;
		auto complete_data = completed.data(complete_length);


		assert(sizeof(char) == sizeof(enet_uint8));
		network_server.getServer()->send_packet_to(clients[0]->get_id(), 0, reinterpret_cast<enet_uint8*>(complete_data), complete_length, ENET_PACKET_FLAG_RELIABLE);
		network_server.getServer()->send_packet_to(clients[1]->get_id(), 0, reinterpret_cast<enet_uint8*>(complete_data), complete_length, ENET_PACKET_FLAG_RELIABLE);
	}
	
}

void Server::onClientConnect(server_client & client)
{
	network_server.trace("on_client_connected id :" + std::to_string(client._uid));

	auto clients = network_server.getServer()->get_connected_clients();


	if (!game_active)
	{

		assignClientLocalID(client);

		if (clients.size() >= 2)
		{
			CustomPacket start("start", "", "");

			unsigned int start_length = 0;
			auto start_data = start.data(start_length);

			assert(sizeof(char) == sizeof(enet_uint8));
			network_server.getServer()->send_packet_to(clients[0]->get_id(), 0, reinterpret_cast<enet_uint8*>(start_data), start_length, ENET_PACKET_FLAG_RELIABLE);
			network_server.getServer()->send_packet_to(clients[1]->get_id(), 0, reinterpret_cast<enet_uint8*>(start_data), start_length, ENET_PACKET_FLAG_RELIABLE);
			game_active = true;
		}
	}

	else if (game_active)
	{
		CustomPacket recon("reconnect", "", "true");

		unsigned int recon_length = 0;
		auto recon_data = recon.data(recon_length);

		assert(sizeof(char) == sizeof(enet_uint8));
		network_server.getServer()->send_packet_to(client._uid, 0, reinterpret_cast<enet_uint8*>(recon_data), recon_length, ENET_PACKET_FLAG_RELIABLE);

		CustomPacket ready("ready", "", "");

		unsigned int ready_length = 0;
		auto ready_data = ready.data(ready_length);

		assert(sizeof(char) == sizeof(enet_uint8));
		network_server.getServer()->send_packet_to_all_if(0, reinterpret_cast<enet_uint8*>(ready_data), ready_length, ENET_PACKET_FLAG_RELIABLE,
			[&](const server_client& destination) {return destination.get_id() != client.get_id(); });

	}

}

void Server::onClientDisconnect(unsigned int client_uid)
{
	network_server.trace("on_client_disconnected id :" + std::to_string(client_uid));

	auto clients = network_server.getServer()->get_connected_clients();

	if (game_active)
	{

		if (clients.size() == 0)
		{
			network_server.trace("No clients remaining, resetting session for new game");

			next_uid = 0;
			game_active = false;
		}

		else
		{
			CustomPacket data_sender("data_sender", "", "");

			unsigned int data_length = 0;
			auto data = data_sender.data(data_length);

			assert(sizeof(char) == sizeof(enet_uint8));
			network_server.getServer()->send_packet_to(clients[0]->get_id(), 0, reinterpret_cast<enet_uint8*>(data), data_length, ENET_PACKET_FLAG_RELIABLE);
		}
	}
}

void Server::assignClientLocalID(server_client & client)
{

	bool even_check = false;
	int id = -1;

	auto clients = network_server.getServer()->get_connected_clients();

	if (clients.size())
	{
		for (auto& conclients : clients)
		{
			if (conclients->_uid % 2 == 0 && conclients->get_id() != client.get_id()) { even_check = true; };
		}
	}

	if (even_check)
	{
		id = 1;
	}

	if (!even_check)
	{
		id = 0;
	}

	CustomPacket initpacket("init", "", std::to_string(id));

	unsigned int packet_length = 0;
	auto packet_data = initpacket.data(packet_length);


	assert(sizeof(char) == sizeof(enet_uint8));
	network_server.getServer()->send_packet_to(client._uid, 0, reinterpret_cast<enet_uint8*>(packet_data), packet_length, ENET_PACKET_FLAG_RELIABLE);
}
