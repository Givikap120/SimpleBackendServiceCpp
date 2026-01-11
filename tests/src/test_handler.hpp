#pragma once

#include "core/job_handler.h"
#include <iostream>

class TestHandler : public JobHandler
{
public:

	TestHandler() 
	{
		m_type = "test";
	}

	JobResult handle(const Job& job) override
	{
		if (job.payload() == "Ok")
			return JobResult::Ok("Ok");

		if (job.payload() == "Fail")
			return JobResult::Fail("Fail");

		throw std::runtime_error("Unhandled payload in TestHandler");
	}
};