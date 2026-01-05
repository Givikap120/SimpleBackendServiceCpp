#pragma once

#include "../core/job_handler.h"

class QuadraticEquationHandler : public JobHandler
{
public:

	QuadraticEquationHandler()
	{
		m_type = "quadratic_equation";
	}

	JobResult handle(const Job& job) override
	{
		if (job.payload().size() != 3 * sizeof(double))
		{
			return JobResult::Fail("Invalid payload size for quadratic equation job");
		}

		const double* coeffs = reinterpret_cast<const double*>(job.payload().data());
		double a = coeffs[0];
		double b = coeffs[1];
		double c = coeffs[2];

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