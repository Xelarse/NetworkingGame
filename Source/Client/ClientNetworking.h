#pragma once
#include <enetpp/client.h>
#include <Common/CommonNetworking.h>

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

	bool isConnected() const;
	bool isConnecting() const;

	std::string getUsername() const {return username; };
	void setUsername(std::string str) { username = str; };
	void killThread() { kill_thread = true; };

private:
	std::atomic<bool> kill_thread = false;

	std::string username = "";

	// the client thread
	enetpp::client client;

	// three consume functions to process networking
	std::function<void()> on_connected;
	std::function<void()> on_disconnected;
	std::function<void(const enet_uint8* data, size_t data_size)> on_data_received;
	
	int channel_count = 2;
};
