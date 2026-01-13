#include "rest_server.h"

#include <nlohmann/json.hpp>

void RestServer::run(uint16_t port) 
{
	// Stop old server if it was running. Safe even if it wasn't running.
	if (m_server) m_server->stop();

	// Create a new server
	m_server = std::make_unique<httplib::Server>();
	
	// Endpoints
	m_server->Get("/", [this](const httplib::Request& req,
		httplib::Response& res) {
			handleHelloWorld(req, res);
		});

	m_server->Post("/jobs", [this](const httplib::Request& req,
		httplib::Response& res) {
			handleSubmit(req, res);
		});

	m_server->Get(R"(/jobs/(\d+)/status)", [this](const httplib::Request& req,
		httplib::Response& res) {
			handleStatus(req, res);
		});

	m_server->Get(R"(/jobs/(\d+)/result)", [this](const httplib::Request& req,
		httplib::Response& res) {
			handleResult(req, res);
		});

	m_server->Post("/shutdown", [this](const httplib::Request& req,
		httplib::Response& res) {
			handleShutdown(req, res);
		});

	std::cout << "[REST Server] Listening on " << m_ip << ":" << port << "\n";
	m_server->listen(m_ip, port);
}

using json = nlohmann::json;

void RestServer::handleHelloWorld(const httplib::Request& req, httplib::Response& res)
{
	json response;
	response["message"] = "Hello, World!";
	res.set_content(response.dump(), "application/json");
}

void RestServer::handleSubmit(const httplib::Request& req, httplib::Response& res)
{
	json body = json::parse(req.body);

	// Send to job service
	ApiJobRequest apiReq;
	apiReq.type = body.at("type").get<std::string>();
	apiReq.payload = body.at("payload").dump();

	uint64_t jobId = m_jobService.submit(apiReq);

	// Return job ID
	json response;
	response["jobId"] = jobId;

	res.set_content(response.dump(), "application/json");
}

void RestServer::handleStatus(const httplib::Request& req, httplib::Response& res)
{
	// Parse job ID from URL
	uint64_t jobId = std::stoull(req.matches[1]);

	JobStatus status = m_jobService.getStatus(jobId);

	// Return status
	json response;
	response["status"] = to_string(status);

	res.set_content(response.dump(), "application/json");
}

void RestServer::handleResult(const httplib::Request& req, httplib::Response& res)
{
	// Parse job ID from URL
	uint64_t jobId = std::stoull(req.matches[1]);

	// Write status
	JobStatus status = m_jobService.getStatus(jobId);

	json response;
	response["status"] = to_string(status);

	// Write result or error
	if (status == JobStatus::Completed) {
		auto result = m_jobService.getResult(jobId);
		response["result"] = json::parse(result->result);
	}
	else if (status == JobStatus::Failed) {
		auto result = m_jobService.getResult(jobId);
		response["error"] = result->errorMessage;
	}

	res.set_content(response.dump(), "application/json");
}

void RestServer::handleShutdown(const httplib::Request& req, httplib::Response& res)
{
	m_server->stop();
}
