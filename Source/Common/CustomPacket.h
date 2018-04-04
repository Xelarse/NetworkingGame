#pragma once

class CustomPacket
{
public:
	CustomPacket(std::string type, std::string usr, std::string msg);
	CustomPacket(char* data);

	~CustomPacket() = default;
	
	std::string getMsg();
	std::string getUsername();
	std::string getType();

	char* data(unsigned int& size) const;


private:
	CustomPacket() = default;
	std::string username;
	std::string msg_text;
	std::string type;
};