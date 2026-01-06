#pragma once


struct JobResult
{
	bool success = false;
	std::string result{};
	std::string errorMessage{};

	static JobResult Ok(std::string result = "")
	{
		return { true, std::move(result), "Job was completed successfully" };
	}

	static JobResult Fail(std::string message)
	{
		return { false, "FAIL", std::move(message) };
	}

	inline std::string log(bool printResult = true) const
	{
		std::ostringstream oss;

		oss << "[Thread " << std::this_thread::get_id() << "] "
			<< (success ? "Success" : "Failure");

		if (printResult && success && !result.empty())
			oss << ". " << result;

		if (!success && !errorMessage.empty())
			oss << ": " << errorMessage;

		oss << "\n";

		return oss.str();
	}
};