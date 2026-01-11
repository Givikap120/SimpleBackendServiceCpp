#include <gtest/gtest.h>
#include "test_utils.h"

using ExecutorRegistrationTest = ExecutorTest;

class DummyHandler : public JobHandler
{
public:

	DummyHandler(std::string type = "dummy")
	{
		m_type = std::move(type);
	}

	JobResult handle(const Job& job) override
	{
		return JobResult::Ok("Ok");
	}
};

TEST_F(ExecutorRegistrationTest, HandlerRegistrationAfterRun)
{
	m_executor->start(1);

	// Attempting to register handler after start should throw
	EXPECT_THROW(m_executor->registerHandler<DummyHandler>(), std::runtime_error);

	m_executor->shutdown();
}

TEST_F(ExecutorRegistrationTest, HandlerRegistrationTwice)
{
	m_executor->registerHandler<DummyHandler>();

	// Attempting to register the same handler twice should throw
	EXPECT_THROW(m_executor->registerHandler<DummyHandler>(), std::runtime_error);
}

TEST_F(ExecutorRegistrationTest, NoTypeHandlerRegistration)
{
	// Attempting to register the handler with empty type should throw
	EXPECT_THROW(m_executor->registerHandler<DummyHandler>(""), std::runtime_error);
}

TEST_F(ExecutorRegistrationTest, SubmitBeforeRun) {
	EXPECT_THROW(m_jobService->submit(ApiJobRequest("test", "test")), std::runtime_error);
}

TEST_F(ExecutorRegistrationTest, StartTwice) {
	m_executor->start(1);
	EXPECT_THROW(m_executor->start(1), std::runtime_error);
}

TEST_F(ExecutorRegistrationTest, StartZeroThreads) {
	EXPECT_THROW(m_executor->start(0), std::invalid_argument);
}

TEST_F(ExecutorRegistrationTest, AddTaskAfterShutdown) {
	m_executor->start(1);
	m_executor->shutdown();
	EXPECT_THROW(m_jobService->submit(ApiJobRequest("test", "test")), std::runtime_error);
}