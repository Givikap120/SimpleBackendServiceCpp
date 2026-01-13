#pragma once

#include <cstdint>
#include <httplib.h>

#include "intermediate/job_submission_service.h"

class RestServer {
public:
	RestServer(JobSubmissionService& jobService)
		: m_jobService(jobService) {}

	void run(uint16_t port);
	
	// Test server
	void handleHelloWorld(const httplib::Request& req, httplib::Response& res);

	// Submit a new job
	void handleSubmit(const httplib::Request& req, httplib::Response& res);

	// Get job status
	void handleStatus(const httplib::Request& req, httplib::Response& res);

	// Get job status and result if available
	void handleResult(const httplib::Request& req, httplib::Response& res);

	// Testing endpoint. Shutdowns server. WARNING: This doesn't touch the executor or job service, only stops the REST server.
	void handleShutdown(const httplib::Request& req, httplib::Response& res);

private:
	JobSubmissionService& m_jobService;
	std::unique_ptr<httplib::Server> m_server = nullptr;

	// Forcing localhost only, this is a demonstration project anyway.
	std::string m_ip = "0.0.0.0";
};
