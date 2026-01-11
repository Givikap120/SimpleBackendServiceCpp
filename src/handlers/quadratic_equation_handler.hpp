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

		json result;

		if (discriminant < 0)
		{
			result["roots"] = json::array();
			return JobResult::Ok(result.dump());
		}

		double root1 = (-b + std::sqrt(discriminant)) / (2 * a);
		double root2 = (-b - std::sqrt(discriminant)) / (2 * a);

		result["roots"] = (root1 == root2) ? json::array({ root1 }) : json::array({ root1, root2 });

		return JobResult::Ok(result.dump());
	}
};