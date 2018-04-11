#include "CustomPacket.h"
#include  <queue>

CustomPacket::CustomPacket(std::string classifier, std::string usr, std::string msg) : type(classifier), username(usr), msg_text(msg)
{
}

CustomPacket::CustomPacket(char * data)
{
	type = (reinterpret_cast<const char*>(data));
	username = (reinterpret_cast<const char*>(data + type.length() + 1));
	msg_text = (reinterpret_cast<const char*>(data + (type.length() + 1) + (username.length() + 1)));
}

std::string CustomPacket::getMsg()
{
	return msg_text;
}

std::string CustomPacket::getUsername()
{
	return username;
}

std::string CustomPacket::getType()
{
	return type;
}

char * CustomPacket::data(unsigned int & size) const
{
	const int type_length = type.length() + 1;
	const int username_length = username.length() + 1;
	const int msg_length = msg_text.length() + 1;

	size = type_length + username_length + msg_length;

	char* data = new char[size];

	memcpy((void*)&data[0], type.data(), type_length);

	memcpy((void*)&data[type_length], username.data(), username_length);

	memcpy((void*)&data[type_length + username_length], msg_text.data(), msg_length);

	return data;
}

void CustomPacket::unitDataDeciper(std::string & unit_name, int & x_pos, int & y_pos, int & squad_size, int & unit_hp)
{
	std::string s = getMsg();
	std::string delimiter = "&";

	size_t pos = 0;
	std::queue<std::string> results;

	while ((pos = s.find(delimiter)) != std::string::npos) {
		results.push(s.substr(0, pos));
		s.erase(0, pos + delimiter.length());
	}

	unit_name = results.front();
	results.pop();

	x_pos = std::stoi(results.front());
	results.pop();

	y_pos = std::stoi(results.front());
	results.pop();

	squad_size = std::stoi(results.front());
	results.pop();

	unit_hp = std::stoi(results.front());
	results.pop();
}
