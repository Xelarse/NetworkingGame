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

	std::string getIp() { return connecting_ip; };
	void setIp(std::string ip) { connecting_ip = ip; };

	std::string getUsername();
	int getUserID()				{ return user_ID; };
	void setUserID(int id)		{ user_ID = id; };
	void setUsername(std::string str);
	void killThread()			{ kill_thread = true; };

	bool getIsLobby()			{ return is_lobby; };
	bool getIsReconnecting()	{ return is_reconnecting; };
	bool getIsDataSender()		{ return data_sender; };
	bool getIsReadyToSend()		{ return ready_to_send; };
	bool getIsUpdateCompleted() { return update_complete; };
	int getPlayerTurn()			{ return player_turn; };
	int getAssignedPlayer()		{ return assigned_player; };
	void resetReadyToSend()		{ ready_to_send = false; };
	void resetUpdateComplete() { update_complete = false; };

	void sendFrontOfQueue();
	void processRecievedPackets(CustomPacket msg);


	std::queue<CustomPacket> chat_recieved_queue;
	std::mutex chat_recieved_mtx;

	std::queue<CustomPacket> sending_queue;
	std::mutex sending_mtx;

	std::queue<CustomPacket> unit_update_queue;
	std::mutex unit_update_mtx;

	std::queue<CustomPacket> recieved_queue;
	std::mutex recieved_mtx;


private:
	std::atomic<bool> kill_thread = false;

	std::atomic<bool> is_lobby = true; //Used to signify if players are in lobby waiting for someone to play against
	std::atomic<bool> is_reconnecting = false; //To signify if a player has disconnected
	std::atomic<bool> data_sender = false; //To signify if this client will be the player to give the unit data to the reconnecting player
	std::atomic<bool> ready_to_send = false; //To signify that the reconnecting player is ready to recieve.
	std::atomic<int> player_turn = -1; //To signify the players turn when reconnecting 
	std::atomic<int> assigned_player = -1; //To signify the clients side when reconnecting
	std::atomic<bool> update_complete = false;

	std::string connecting_ip = "";

	std::string username = "";
	std::mutex username_mtx;

	std::atomic<int> user_ID = -1;

	// the client thread
	enetpp::client client;
	
	int channel_count = 2;
};
