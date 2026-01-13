#pragma once

#include <cstdint>
#include <string>
#include "grpc_job_service.h"

class GrpcServer
{
public:
	GrpcServer(JobSubmissionService& jobService)
		: m_grpcService(GrpcJobService(jobService)) {
	}

	void run(uint16_t port);

private:
	GrpcJobService m_grpcService;

	// Forcing localhost only, this is a demonstration project anyway.
	const std::string m_ip = "0.0.0.0";
};