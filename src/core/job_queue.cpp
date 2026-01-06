#include "job_queue.h"

void JobQueue::push(Job&& job)
{
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_shutdown)
			return;

		m_queue.push(std::move(job));
	}

	m_cv.notify_one();
}

std::optional<Job> JobQueue::pop()
{
	std::unique_lock<std::mutex> lock(m_mutex);

	m_cv.wait(lock, [this] {
		return m_shutdown || !m_queue.empty();
		});

	if (m_shutdown && m_queue.empty())
		return std::nullopt;

	Job job = std::move(m_queue.front());
	m_queue.pop();
	return job;
}

void JobQueue::shutdown()
{
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_shutdown = true;
	}

	m_cv.notify_all();
}

bool JobQueue::isShutdown() const noexcept
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_shutdown;
}
