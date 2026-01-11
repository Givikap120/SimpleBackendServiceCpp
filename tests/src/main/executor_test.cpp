#include <gtest/gtest.h>
#include "test_utils.h"
#include "core/executor.h"
#include "intermediate/job_submission_service.h"

TEST_F(ExecutorTest, HandlesSuccessJob) {
	uint64_t okJobId = m_jobService->submit(ApiJobRequest("test", "Ok"));
	auto result = waitForJob(*m_jobService, okJobId);
	ASSERT_TRUE(result.has_value());
	EXPECT_TRUE(result->success);
}

TEST_F(ExecutorTest, HandlesFailJob) {
	uint64_t failJobId = m_jobService->submit(ApiJobRequest("test", "Fail"));
	auto result = waitForJob(*m_jobService, failJobId);
	ASSERT_TRUE(result.has_value());
	EXPECT_FALSE(result->success);
	EXPECT_EQ(result->errorMessage, "Fail");
}

TEST_F(ExecutorTest, HandlesExceptionJob) {
	uint64_t exceptionJobId = m_jobService->submit(ApiJobRequest("test", "Unknown"));
	auto result = waitForJob(*m_jobService, exceptionJobId);
	ASSERT_TRUE(result.has_value());
	EXPECT_FALSE(result->success);
	EXPECT_EQ(result->errorMessage, "Unhandled payload in TestHandler");
}
