#pragma once
#include <Server\ServerNetworking.h>


class Server
{
public:
	Server() = default;
	~Server() = default;

	void initialise();
	int  run();
	
private:
	ServerComponent network_server;
	void onClientData(server_client& client, const enet_uint8* data, size_t data_size);
	void onClientConnect(server_client& client);
	void onClientDisconnect(unsigned int client_uid);

	void assignClientLocalID(server_client& client);

	bool game_active = false;

public:
	static std::mutex s_cout_mutex;
};