#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <cstdint>

class Job
{
public:
	using Clock = std::chrono::system_clock;

	Job(uint64_t id, std::string type, std::vector<std::byte> payload)
		: m_id(id), m_type(std::move(type)), m_payload(std::move(payload)), m_createdAt(Clock::now())
	{
	}

	Job(uint64_t id, std::string type, std::string payload)
		: m_id(id), m_type(std::move(type)), m_createdAt(Clock::now())
	{
		m_payload.resize(payload.size());
		std::memcpy(m_payload.data(), payload.data(), payload.size());
	}

	inline uint64_t id() const noexcept { return m_id; }
	inline const std::string& type() const noexcept { return m_type; }
	inline const std::vector<std::byte>& payload() const noexcept { return m_payload; }
	inline Clock::time_point createdAt() const noexcept { return m_createdAt; }

private:
	uint64_t m_id = 0;
	std::string m_type{};
	std::vector<std::byte> m_payload{};
	Clock::time_point m_createdAt = Clock::now();
};
