#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <chrono>

#include "../core/job_result.h"

enum class JobStatus {
	Unknown,
	Pending,
	Running,
	Completed,
	Failed
};

struct JobRecord {
	uint64_t id = 0;
	std::string type = "";
	JobStatus status = JobStatus::Pending;
	std::optional<JobResult> result{};
	std::chrono::system_clock::time_point createdAt = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point startedAt{};
	std::chrono::system_clock::time_point finishedAt{};
};

inline const char* to_string(JobStatus status) noexcept
{
	switch (status)
	{
	case JobStatus::Pending:   return "Pending";
	case JobStatus::Running:   return "Running";
	case JobStatus::Completed: return "Completed";
	case JobStatus::Failed:    return "Failed";
	}

	return "Unknown";
}
