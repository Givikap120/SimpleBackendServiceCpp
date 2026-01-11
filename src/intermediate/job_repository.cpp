#include "job_repository.h"

void JobRepository::add(JobRecord record)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_jobs.emplace(record.id, std::move(record));
}

void JobRepository::markAsRunning(uint64_t id)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_jobs.find(id) != m_jobs.end())
	{
		auto& record = m_jobs.at(id);

		if (record.status != JobStatus::Pending)
			throw std::runtime_error("Cannot mark a job as running that is not pending");

		record.status = JobStatus::Running;
	}
}

void JobRepository::sendResult(uint64_t id, JobResult&& result)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_jobs.find(id) != m_jobs.end())
	{
		auto& record = m_jobs.at(id);

		if (record.status != JobStatus::Running)
			throw std::runtime_error("Cannot send result for a job that is not running");

		record.status = result.success ? JobStatus::Completed : JobStatus::Failed;
		record.result = std::move(result);
	}
}

std::optional<JobRecord> JobRepository::get(uint64_t id)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_jobs.find(id) != m_jobs.end() ? std::optional<JobRecord>(m_jobs.at(id)) : std::nullopt;
}
