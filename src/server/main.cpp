#include <iostream>
#include "../core/executor.h"
#include "../intermediate/job_submission_service.h"

#include "../handlers/print_handler.h"
#include "../handlers/quadratic_equation_handler.h"

#include "proto/jobs.grpc.pb.h"
#include "proto/jobs.pb.h"

std::vector<std::byte> to_bytes(const std::string& str)
{
	std::vector<std::byte> bytes(str.size());
	std::memcpy(bytes.data(), str.data(), str.size());
	return bytes;
}

template <typename T>
std::vector<std::byte> to_bytes(const T& value)
{
	static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");

	std::vector<std::byte> bytes(sizeof(T));
	std::memcpy(bytes.data(), &value, sizeof(T));
	return bytes;
}

template <typename T>
std::vector<std::byte> to_bytes(const std::vector<T>& vec)
{
	static_assert(std::is_trivially_copyable_v<T>, "vector element type must be trivially copyable");

	std::vector<std::byte> bytes(vec.size() * sizeof(T));
	if (!vec.empty())
		std::memcpy(bytes.data(), vec.data(), vec.size() * sizeof(T));
	return bytes;
}

template <typename T>
std::vector<std::byte> to_bytes(std::initializer_list<T> list)
{
	static_assert(std::is_trivially_copyable_v<T>, "initializer_list element type must be trivially copyable");

	std::vector<std::byte> bytes(list.size() * sizeof(T));
	std::memcpy(bytes.data(), list.begin(), list.size() * sizeof(T));
	return bytes;
}

int main() {
	Executor executor;

	// Register job handlers
	executor.registerHandler<PrintHandler>();
	executor.registerHandler<QuadraticEquationHandler>();

	JobSubmissionService jobService(executor);

	executor.start(4);

	// Push jobs
	uint64_t idPrint = jobService.submit(ApiJobRequest("print", to_bytes("Hello world")));
	uint64_t idEquation = jobService.submit(ApiJobRequest("quadratic_equation", to_bytes({1.0, 2.0, 1.0})));

	std::optional<JobResult> resultPrint = jobService.getResult(idPrint);
	std::cout << "Print job ("<< idPrint  <<") status: " << to_string(jobService.getStatus(idPrint)) << "\n";
	std::cout << "Print job (" << idPrint << ") result: " << (resultPrint ? resultPrint.value().result : "NO RESULT") << "\n";

	// Wait a bit for processing
	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::optional<JobResult> resultEquation = jobService.getResult(idEquation);
	std::cout << "Equation job (" << idEquation << ") status: " << to_string(jobService.getStatus(idEquation)) << "\n";
	std::cout << "Equation job (" << idEquation << ") result: " << (resultEquation ? resultEquation.value().result : "NO RESULT") << "\n";

	// Shutdown executor
	executor.shutdown();

	return 0;
}
