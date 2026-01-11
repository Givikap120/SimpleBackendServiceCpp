#pragma once

#include "../core/job_handler.h"

#include <iostream>

class PrintHandler : public JobHandler
{
public:

	PrintHandler() 
	{
		m_type = "print";
	}

	JobResult handle(const Job& job) override
	{
		std::string logMsg = "[PrintHandler] " + job.payload() + "\n";
		std::cout << logMsg;
		return JobResult::Ok();
	}
};