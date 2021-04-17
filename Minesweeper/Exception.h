#pragma once

#include <stdexcept>
#include <string>

class Exception : public std::exception
{
private:
	const std::string message;

public:
	explicit Exception(const std::string& message)
		: message(message) { }

	const char* what(void) const override {
		return message.c_str();
	}
};

void ThrowErrorUsingSDLMessage(const std::string& message, const std::string& sdl_message);