#include "grpc_job_service.h"

grpc::Status GrpcJobService::SubmitJob(grpc::ServerContext* context, const MyGrpc::SubmitJobRequest* request, MyGrpc::SubmitJobResponse* response)
{
	// This is validated at job service level too, but we can catch it earlier here, and get more correct gRPC status code.
	if (request->type().empty())
	{
		return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Job type cannot be empty");
	}

	try
	{
		uint64_t jobId = m_jobService.submit(ApiJobRequest(request->type(), request->payload()));
		response->set_job_id(jobId);
		return grpc::Status::OK;
	}
	catch (const std::exception& e)
	{
		return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
	}
}

grpc::Status GrpcJobService::GetJobStatus(grpc::ServerContext* context, const MyGrpc::GetJobStatusRequest* request, MyGrpc::GetJobStatusResponse* response)
{
	JobStatus jobStatus = m_jobService.getStatus(request->job_id());
	response->set_status(mapStatus(jobStatus));

	return grpc::Status::OK;
}

grpc::Status GrpcJobService::GetJobResult(grpc::ServerContext* context, const MyGrpc::GetJobResultRequest* request, MyGrpc::GetJobResultResponse* response)
{
	const uint64_t jobId = request->job_id();

	JobStatus jobStatus = m_jobService.getStatus(jobId);
	response->set_status(mapStatus(jobStatus));

	if (jobStatus == JobStatus::Completed || jobStatus == JobStatus::Failed)
	{
		auto jobResult = m_jobService.getResult(jobId);

		if (!jobResult.has_value())
		{
			// This should not happen, notifying that there's some error in the service.
			response->set_status(MyGrpc::JOB_STATUS_NO_RESULT_FOUND);
		}
		else
		{
			if (jobStatus == JobStatus::Completed)
				response->set_result(jobResult->result);
			else if (jobStatus == JobStatus::Failed)
				response->set_error_message(jobResult->errorMessage);
		}
	}

	return grpc::Status::OK;
}

MyGrpc::JobStatus GrpcJobService::mapStatus(JobStatus status)
{
	switch (status)
	{
	case JobStatus::Pending:
		return MyGrpc::JobStatus::JOB_STATUS_PENDING;
	case JobStatus::Running:
		return MyGrpc::JobStatus::JOB_STATUS_RUNNING;
	case JobStatus::Completed:
		return MyGrpc::JobStatus::JOB_STATUS_COMPLETED;
	case JobStatus::Failed:
		return MyGrpc::JobStatus::JOB_STATUS_FAILED;
	default:
		return MyGrpc::JobStatus::JOB_STATUS_UNKNOWN;
	}
}
