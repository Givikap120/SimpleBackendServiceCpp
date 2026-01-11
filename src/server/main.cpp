#include <iostream>
#include <nlohmann/json.hpp>

#include "../core/executor.h"
#include "../intermediate/job_submission_service.h"

#include "../handlers/print_handler.h"
#include "../handlers/quadratic_equation_handler.h"

#include "proto/jobs.grpc.pb.h"
#include "proto/jobs.pb.h"


using json = nlohmann::json;

int main() {
	Executor executor;

	// Register job handlers
	executor.registerHandler<PrintHandler>();
	executor.registerHandler<QuadraticEquationHandler>();

	JobSubmissionService jobService(executor);

	executor.start(4);

	// Push jobs
	uint64_t idPrint = jobService.submit(ApiJobRequest("print", "Hello world"));

	json payload;
	payload["a"] = 1.0;
	payload["b"] = -2.0;
	payload["c"] = 1.0;
	uint64_t idEquation = jobService.submit(ApiJobRequest("quadratic_equation", payload.dump()));

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
