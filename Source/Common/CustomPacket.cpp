#include "CustomPacket.h"

CustomPacket::CustomPacket(std::string classifier, std::string usr, std::string msg) : type(classifier), username(usr), msg_text(msg)
{
}

CustomPacket::CustomPacket(char * data)
{
	type = (reinterpret_cast<const char*>(data));
	username = (reinterpret_cast<const char*>(data + type.length() + 1));
	msg_text = (reinterpret_cast<const char*>(data + type.length() + username.length() + 2));
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
	const int username_length = username.length() + 1;
	const int msg_length = msg_text.length() + 1;
	const int type_length = type.length() + 1;

	size = username_length + msg_length + type_length;

	char* data = new char[size];

	memcpy((void*)&data[0], type.data(), type_length);

	memcpy((void*)&data[type_length], username.data(), username_length);

	memcpy((void*)&data[type_length + username_length], msg_text.data(), msg_length);

	return data;
}
