#include "executor.h"
#include <iostream>

void Executor::workerLoop()
{
	while (m_running)
	{
		auto jobOpt = m_queue.pop();
		if (!jobOpt.has_value()) break; // queue is shutting down

		auto& job = *jobOpt;

		JobHandler* handler = nullptr;
		auto it = m_handlers.find(job.type());
		if (it != m_handlers.end())
		{
			handler = it->second.get();
		}

		JobResult result;

		if (handler)
		{
			result = handler->handle(job);
		}
		else
		{
			result = JobResult::Fail("No handler registered for job type: " + job.type());
		}

		std::cout << result.log();
	}
}

void Executor::start(std::size_t numThreads)
{
	if (m_running)
		throw std::runtime_error("Executor already running");

	if (numThreads == 0)
		throw std::invalid_argument("Must start at least one worker thread");

	m_running = true;

	m_workers.reserve(numThreads);
	for (std::size_t i = 0; i < numThreads; ++i)
	{
		m_workers.emplace_back([this](std::stop_token token) {
			workerLoop();
			});
	}
}

void Executor::shutdown()
{
	if (!m_running) return;

	// Stop the queue first so worker threads unblock from pop()
	m_queue.shutdown();

	// Mark executor as not running
	m_running = false;

	// Join all threads
	for (auto& worker : m_workers)
	{
		worker.join();
	}

	m_workers.clear();
}
