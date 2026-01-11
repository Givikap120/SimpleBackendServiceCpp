#include <iostream>
#include <nlohmann/json.hpp>

#include "rest_server.h"
#include "core/executor.h"
#include "intermediate/job_submission_service.h"

#include "handlers/print_handler.hpp"
#include "handlers/quadratic_equation_handler.hpp"

#include "proto/jobs.grpc.pb.h"
#include "proto/jobs.pb.h"

using json = nlohmann::json;

void simpleTest(JobSubmissionService& jobService)
{
	// Push jobs
	uint64_t idPrint = jobService.submit(ApiJobRequest("print", "Hello world"));

	json payload;
	payload["a"] = 1.0;
	payload["b"] = -2.0;
	payload["c"] = 1.0;
	uint64_t idEquation = jobService.submit(ApiJobRequest("quadratic_equation", payload.dump()));

	std::optional<JobResult> resultPrint = jobService.getResult(idPrint);
	std::cout << "Print job (" << idPrint << ") status: " << to_string(jobService.getStatus(idPrint)) << "\n";
	std::cout << "Print job (" << idPrint << ") result: " << (resultPrint ? resultPrint.value().result : "NO RESULT") << "\n";

	// Wait a bit for processing
	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::optional<JobResult> resultEquation = jobService.getResult(idEquation);
	std::cout << "Equation job (" << idEquation << ") status: " << to_string(jobService.getStatus(idEquation)) << "\n";
	std::cout << "Equation job (" << idEquation << ") result: " << (resultEquation ? resultEquation.value().result : "NO RESULT") << "\n";
}

int main() {
	// Executor object runs the task via registered handlers
	Executor executor;

	// Register job handlers
	executor.registerHandler<PrintHandler>();
	executor.registerHandler<QuadraticEquationHandler>();

	// Job submission service allows to submit jobs and query status/results
	JobSubmissionService jobService(executor);

	// Rest server handles HTTP requests and sends them to job service
	RestServer server(jobService);

	// Start executor with 4 threads
	executor.start(4);

	// Start REST server on port 8080
	server.run(8080);

	// Shutdown executor after server stops
	executor.shutdown();

	return 0;
}
