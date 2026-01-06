#include "job_repository.h"
#include "job_submission_service.h"

JobSubmissionService::JobSubmissionService(Executor& executor)
	: m_executor(executor) 
{
	m_executor.setJobStartedNotification([this](uint64_t jobId) {
		m_repo.markAsRunning(jobId);
		});

	m_executor.setJobCompletedNotification([this](uint64_t jobId, JobResult result) {
		m_repo.sendResult(jobId, std::move(result));
		});
}

uint64_t JobSubmissionService::submit(const ApiJobRequest& apiRequest)
{
	// Create a job and record to keep track of it
	Job job = createJobFromRequest(apiRequest);
	JobRecord record{ job.id(), job.type() };

	m_repo.add(record);

	uint64_t id = job.id();
	m_executor.addJob(std::move(job));

	return id;
}

JobStatus JobSubmissionService::getStatus(uint64_t jobId)
{
	auto record = m_repo.get(jobId);
	return record ? record->status : JobStatus::Unknown;
}

std::optional<JobResult> JobSubmissionService::getResult(uint64_t jobId) {
	auto record = m_repo.get(jobId);
	if (record && (record->status == JobStatus::Completed || record->status == JobStatus::Failed))
		return record->result;
	return std::nullopt;
}

Job JobSubmissionService::createJobFromRequest(const ApiJobRequest& request)
{
	return Job{ generateJobId(), request.type, request.payload };
}

uint64_t JobSubmissionService::generateJobId()
{
	return m_nextJobId.fetch_add(1);
}
