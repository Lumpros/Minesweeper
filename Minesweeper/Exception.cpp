#include "Exception.h"

void ThrowErrorUsingSDLMessage(const std::string& message, const std::string& sdl_message)
{
	throw Exception(message + sdl_message);
}