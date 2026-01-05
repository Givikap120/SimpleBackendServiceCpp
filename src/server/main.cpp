#include <iostream>
#include "../core/executor.h"

#include "../handlers/print_handler.h"

#include "proto/jobs.grpc.pb.h"
#include "proto/jobs.pb.h"

int main() {
	Executor executor;

	// Register job handlers
	executor.registerHandler<PrintHandler>();

	executor.start(4);

	// Push jobs
	executor.addJob(Job(1, "print", "Hello world"));
	executor.addJob(Job(2, "compute", "42"));

	// Wait a bit for processing
	std::this_thread::sleep_for(std::chrono::seconds(1));

	// Shutdown executor
	executor.shutdown();

	return 0;
}
