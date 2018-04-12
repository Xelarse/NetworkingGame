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
	
}