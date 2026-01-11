#pragma once

#include <nlohmann/json.hpp>

#include "../core/job_handler.h"

class QuadraticEquationHandler : public JobHandler
{
public:

	QuadraticEquationHandler()
	{
		m_type = "quadratic_equation";
	}

	using json = nlohmann::json;

	JobResult handle(const Job& job) override
	{
		json j = json::parse(job.payload());

		double a = j.at("a").get<double>();
		double b = j.at("b").get<double>();
		double c = j.at("c").get<double>();

		double discriminant = b * b - 4 * a * c;

		if (discriminant < 0)
		{
			return JobResult::Ok("No real roots exist");
		}

		double root1 = (-b + std::sqrt(discriminant)) / (2 * a);
		double root2 = (-b - std::sqrt(discriminant)) / (2 * a);

		// This should be changed to json in the future for better interoperability
		std::ostringstream oss;
		oss << "Roots: " << root1;
		if (root1 != root2) oss << ", " << root2;
		oss << "\n";

		return JobResult::Ok(oss.str());
	}
};