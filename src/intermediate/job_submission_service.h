#pragma once

#include <cstdint>
#include <optional>
#include <atomic>

#include "../api/api_job_request.h"
#include "../core/executor.h"
#include "../intermediate/job_repository.h"

class JobSubmissionService {
public:
	JobSubmissionService(Executor& executor);

	uint64_t submit(const ApiJobRequest& apiRequest);
	JobStatus getStatus(uint64_t jobId);
	std::optional<JobResult> getResult(uint64_t jobId);

private:
	Job createJobFromRequest(const ApiJobRequest& request);
	uint64_t generateJobId();

	std::atomic<uint64_t> m_nextJobId{ 1 };

	Executor& m_executor;
	JobRepository m_repo{};
};
