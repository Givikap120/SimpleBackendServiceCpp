#pragma once

#include <cstdint>

// Interface for different server types (REST, gRPC, etc.)
class Server
{
public:
	virtual void run(uint16_t port) = 0;

	// This shoudln't be used polymorphically, but just in case.
	virtual ~Server() = default;
};