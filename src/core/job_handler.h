#pragma once

#include "job.h"
#include "job_result.h"

class JobHandler
{
protected:
	// Should be determined during construction, used only for registration, after which it's not important, since the actual lookup key is stored by the map now.
	std::string m_type = "";

public:
	virtual ~JobHandler() = default;

	const std::string& type() const { return m_type; }
	virtual JobResult handle(const Job& job) = 0;
};
