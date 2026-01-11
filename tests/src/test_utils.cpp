#include "test_utils.h"

#include "test_handler.hpp"

std::optional<JobResult> waitForJob(JobSubmissionService& service, uint64_t jobId, int timeoutMs, int checkInterval)
{
	int waited = 0;

	while (waited < timeoutMs) {
		auto status = service.getStatus(jobId);

		if (status == JobStatus::Completed || status == JobStatus::Failed)
			return service.getResult(jobId);

		std::this_thread::sleep_for(std::chrono::milliseconds(checkInterval));
		waited += checkInterval;
	}

	// Timeout
	return {};
}

void ExecutorTest::registerHandlers()
{
	m_executor->registerHandler<TestHandler>();
}

void ExecutorTest::SetUp() {
	m_executor = std::make_unique<Executor>(false);
	m_jobService = std::make_unique<JobSubmissionService>(*m_executor);

	registerHandlers();
}

void ExecutorTest::TearDown() {
	m_executor->shutdown();

	m_jobService.release();
	m_executor.release();
}
