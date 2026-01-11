#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "test_utils.h"
#include "handlers/quadratic_equation_handler.hpp"

using json = nlohmann::json;

class QuadraticEquationExecutorTest : public ExecutorTest {
protected:
	void registerHandlers() override 
	{
		m_executor->registerHandler<QuadraticEquationHandler>();
	}
};

TEST_F(QuadraticEquationExecutorTest, TwoDistinctRoots)
{
	m_executor->start(1);

	json payload {
		{"a", 1.0},
		{"b", -3.0},
		{"c", 2.0}
	};

	uint64_t jobId = m_jobService->submit(
		ApiJobRequest("quadratic_equation", payload.dump())
	);

	auto resultOpt = waitForJob(*m_jobService, jobId);
	ASSERT_TRUE(resultOpt.has_value());
	ASSERT_TRUE(resultOpt->success);

	json result = json::parse(resultOpt->result);
	ASSERT_TRUE(result.contains("roots"));
	ASSERT_EQ(result["roots"].size(), 2);

	double root1 = result["roots"][0];
	double root2 = result["roots"][1];

	EXPECT_DOUBLE_EQ(std::max(root1, root2), 2.0);
	EXPECT_DOUBLE_EQ(std::min(root1, root2), 1.0);
}


TEST_F(QuadraticEquationExecutorTest, SingleRoot)
{
	m_executor->start(1);

	json payload {
		{"a", 1.0},
		{"b", -6.0},
		{"c", 9.0}
	};

	uint64_t jobId = m_jobService->submit(
		ApiJobRequest("quadratic_equation", payload.dump())
	);

	auto resultOpt = waitForJob(*m_jobService, jobId);
	ASSERT_TRUE(resultOpt.has_value());
	ASSERT_TRUE(resultOpt->success);

	json result = json::parse(resultOpt->result);
	ASSERT_EQ(result["roots"].size(), 1);
	EXPECT_DOUBLE_EQ(result["roots"][0], 3.0);
}


TEST_F(QuadraticEquationExecutorTest, NoRealRoots)
{
	m_executor->start(1);

	json payload {
		{"a", 7.0},
		{"b", 2.0},
		{"c", 7.0}
	};

	uint64_t jobId = m_jobService->submit(
		ApiJobRequest("quadratic_equation", payload.dump())
	);

	auto resultOpt = waitForJob(*m_jobService, jobId);
	ASSERT_TRUE(resultOpt.has_value());
	ASSERT_TRUE(resultOpt->success);

	json result = json::parse(resultOpt->result);
	ASSERT_TRUE(result["roots"].is_array());
	EXPECT_TRUE(result["roots"].empty());
}

TEST_F(QuadraticEquationExecutorTest, IncorrectPayloadMissingFields)
{
	m_executor->start(1);

	json payload {
		{"a", 1.0}
	};

	uint64_t jobId = m_jobService->submit(
		ApiJobRequest("quadratic_equation", payload.dump())
	);

	auto resultOpt = waitForJob(*m_jobService, jobId);
	ASSERT_TRUE(resultOpt.has_value());

	EXPECT_FALSE(resultOpt->success);
	EXPECT_EQ(resultOpt->errorMessage, "[json.exception.out_of_range.403] key 'b' not found");
}

TEST_F(QuadraticEquationExecutorTest, IncorrectPayloadWrongFieldType)
{
	m_executor->start(1);

	json payload {
		{"a", 1.0},
		{"b", "not_a_number"},
		{"c", 3.0}
	};

	uint64_t jobId = m_jobService->submit(
		ApiJobRequest("quadratic_equation", payload.dump())
	);

	auto resultOpt = waitForJob(*m_jobService, jobId);
	ASSERT_TRUE(resultOpt.has_value());

	EXPECT_FALSE(resultOpt->success);
	EXPECT_EQ(resultOpt->errorMessage, "[json.exception.type_error.302] type must be number, but is string");
}