#pragma once
#include <enetpp/client.h>
#include <Common/CommonNetworking.h>
#include <Common/CustomPacket.h>

class ClientComponent :
	public NetworkComponent
{
public:
	ClientComponent() = default;
	~ClientComponent() = default;

	// Inherited via NetworkComponent
	virtual bool initialize() override;
	virtual bool deinitialize() override;
	virtual void consumeEvents() override;

	void on_connect();
	void on_disconnect();
	void on_data(const enet_uint8* data, size_t data_size);

	bool isConnected() const;
	bool isConnecting() const;

	std::string getUsername();
	int getUserID() { return user_ID; };
	void setUsername(std::string str);
	void killThread() { kill_thread = true; };


	std::queue<CustomPacket> recieved_queue;
	std::mutex recieved_mtx;

	std::queue<CustomPacket> sending_queue;
	std::mutex sending_mtx;

	std::queue<CustomPacket> unit_update_queue;
	std::mutex unit_update_mtx;


private:
	std::atomic<bool> kill_thread = false;

	std::string username = "";
	std::mutex username_mtx;

	std::atomic<int> user_ID = -1;

	// the client thread
	enetpp::client client;
	
	int channel_count = 2;
};
