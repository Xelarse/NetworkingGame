#pragma once

class CustomPacket
{
public:
	CustomPacket(std::string type, std::string usr, std::string msg);
	CustomPacket(char* data);

	~CustomPacket() = default;
	

	// JH - these functions all return copies of strings, prefer const references instead
	// const std::string& getMsg() const;
	std::string getMsg();
	std::string getUsername();
	std::string getType();

	// JH - inconsistent use of const correctness
	char* data(unsigned int& size) const;

	//Used to find out unit data from a packet tagged as unit, put in variables as reference and they will be changed to the packets data
	void unitDataDeciper(std::string& unit_name, int& x_pos, int& y_pos, int& squad_size, int& unit_hp);


private:

	// JH - instead of making this default private, just delete it
	CustomPacket() = delete;
	std::string username;
	std::string msg_text;
	std::string type;
};