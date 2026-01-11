#pragma once

#include <optional>
#include <gtest/gtest.h>
#include "intermediate/job_submission_service.h"

std::optional<JobResult> waitForJob(JobSubmissionService& service, uint64_t jobId, int timeoutMs = 500, int checkInterval = 10);

class ExecutorTest : public ::testing::Test {
protected:
	std::unique_ptr<Executor> m_executor = nullptr;
	std::unique_ptr<JobSubmissionService> m_jobService = nullptr;

	// By default only has test handler
	virtual void registerHandlers();

	void SetUp() override;
	void TearDown() override;
};
