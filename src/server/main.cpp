#include <iostream>

#include "rest_server.h"
#include "grpc_server.h"

#include "core/executor.h"
#include "intermediate/job_submission_service.h"

#include "handlers/print_handler.hpp"
#include "handlers/quadratic_equation_handler.hpp"

std::jthread runServerAsync(Server& server, uint16_t port)
{
	return std::jthread([&server, port]() {
		server.run(port); // blocks inside run()
		});
}

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
	RestServer restServer(jobService);

	// Grpc server handles gRPC requests and sends them to job service
	GrpcServer grpcServer(jobService);

	// Run servers asynchronously
	std::jthread restThread = runServerAsync(restServer, 8080);
	std::jthread grpcThread = runServerAsync(grpcServer, 50051);

	// Wait for both servers (blocks here until they exit)
	restThread.join();
	grpcThread.join();

	// Shutdown executor after both servers stop
	executor.shutdown();

	return 0;
}
