#include <gtest/gtest.h>
#include "test_utils.h"

using ExecutorRunTest = ExecutorTest;

TEST_F(ExecutorRunTest, HandlesSuccessJob)
{
	m_executor->start(1);
	uint64_t okJobId = m_jobService->submit(ApiJobRequest("test", "Ok"));
	auto result = waitForJob(*m_jobService, okJobId);
	ASSERT_TRUE(result.has_value());
	EXPECT_TRUE(result->success);
	EXPECT_EQ(result->result, "Ok");
	EXPECT_EQ(result->errorMessage, "");
}

TEST_F(ExecutorRunTest, HandlesFailJob)
{
	m_executor->start(1);
	uint64_t failJobId = m_jobService->submit(ApiJobRequest("test", "Fail"));
	auto result = waitForJob(*m_jobService, failJobId);
	ASSERT_TRUE(result.has_value());
	EXPECT_FALSE(result->success);
	EXPECT_EQ(result->errorMessage, "Fail");
	EXPECT_EQ(result->result, "");
}

TEST_F(ExecutorRunTest, HandlesExceptionJob)
{
	m_executor->start(1);
	uint64_t exceptionJobId = m_jobService->submit(ApiJobRequest("test", "Unknown"));
	auto result = waitForJob(*m_jobService, exceptionJobId);
	ASSERT_TRUE(result.has_value());
	EXPECT_FALSE(result->success);
	EXPECT_EQ(result->errorMessage, "Unhandled payload in TestHandler");
	EXPECT_EQ(result->result, "");
}

