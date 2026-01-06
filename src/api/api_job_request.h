#pragma once

#include <string>
#include <vector>
#include <cstddef>

struct ApiJobRequest
{
	std::string type = "";
	std::vector<std::byte> payload{};
};
