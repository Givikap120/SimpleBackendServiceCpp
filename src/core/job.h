#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <cstdint>

class Job
{
public:
	using Clock = std::chrono::system_clock;

	Job(uint64_t id, std::string type, std::string payload)
		: m_id(id), m_type(std::move(type)), m_payload(std::move(payload))
	{
	}

	uint64_t id() const noexcept { return m_id; }
	const std::string& type() const noexcept { return m_type; }
	const std::string& payload() const noexcept { return m_payload; }

private:
	uint64_t m_id = 0;
	std::string m_type{};
	std::string m_payload{};
};
