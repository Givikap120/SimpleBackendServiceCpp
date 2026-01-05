#pragma once

#include "job.h"

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

class JobQueue
{
public:
	JobQueue() = default;
	JobQueue(const JobQueue&) = delete;
	JobQueue& operator=(const JobQueue&) = delete;

	void push(Job job);
	std::optional<Job> pop();

	void shutdown();
	bool isShutdown() const noexcept; // This function requires mutex. Don't call it in functions that already hold the mutex to avoid deadlocks.

private:
	mutable std::mutex m_mutex;
	std::condition_variable m_cv;
	std::queue<Job> m_queue; // Mutex is not recursive, because the logic here is simple enough to avoid deadlocks.
	bool m_shutdown{ false }; // Protected by m_mutex. We don't use atomic for simplicity purposes. Replace with std::atomic<bool> if additional performance is needed.
};
