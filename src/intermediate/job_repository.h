#pragma once

#include <cstdint>
#include <optional>
#include <unordered_map>
#include <mutex>

#include "job_record.h"

class JobRepository {
public:
	void add(JobRecord record);
	void markAsRunning(uint64_t id);
	void sendResult(uint64_t id, JobResult&& result);

	std::optional<JobRecord> get(uint64_t id);

private:
	std::mutex m_mutex{};
	std::unordered_map<uint64_t, JobRecord> m_jobs{};
};
