#include <iostream>

#include "rest_server.h"
#include "core/executor.h"
#include "intermediate/job_submission_service.h"

#include "handlers/print_handler.hpp"
#include "handlers/quadratic_equation_handler.hpp"

int main() {
	// Executor object runs the task via registered handlers
	Executor executor;

	// Register job handlers
	executor.registerHandler<PrintHandler>();
	executor.registerHandler<QuadraticEquationHandler>();

	// Start executor with 4 threads
	executor.start(4);

	// Job submission service allows to submit jobs and query status/results
	JobSubmissionService jobService(executor);

	// Rest server handles HTTP requests and sends them to job service
	RestServer server(jobService);

	// Start REST server on port 8080
	server.run(8080);

	// Shutdown executor after server stops
	executor.shutdown();

	return 0;
}
