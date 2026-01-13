#pragma once

#include <grpcpp/grpcpp.h>

#include "proto/jobs.grpc.pb.h"
#include "proto/jobs.pb.h"

#include "intermediate/job_submission_service.h"

// This may be bad practice, and it should go only into .cpp, but for better readability I will shortern the namespace, since this project is demonstration only.
namespace MyGrpc = SimpleBackendServiceCppGrpc;

class GrpcJobService : public MyGrpc::JobService::Service
{
public:
	GrpcJobService(JobSubmissionService& jobService)
		: m_jobService(jobService) {
	}

	grpc::Status SubmitJob(grpc::ServerContext* context, const MyGrpc::SubmitJobRequest* request, MyGrpc::SubmitJobResponse* response) override;
	grpc::Status GetJobStatus(grpc::ServerContext* context, const MyGrpc::GetJobStatusRequest* request, MyGrpc::GetJobStatusResponse* response) override;
	grpc::Status GetJobResult(grpc::ServerContext* context, const MyGrpc::GetJobResultRequest* request, MyGrpc::GetJobResultResponse* response) override;

private:
	JobSubmissionService& m_jobService;

	static MyGrpc::JobStatus mapStatus(JobStatus status);
};

