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
		std::string message(reinterpret_cast<const char*>(job.payload().data()), job.payload().size());
		std::string logMsg = "[PrintHandler] " + message + "\n";
		std::cout << logMsg;
		return JobResult::Ok();
	}
};