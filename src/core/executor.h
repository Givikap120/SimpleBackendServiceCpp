#pragma once

#include "job_queue.h"
#include "job_handler.h"

#include <thread>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>
#include <atomic>
#include <type_traits>
#include <iostream>

class Executor
{
public:
	~Executor()
	{
		if (m_running) shutdown();
	}

	Executor() = default;
	Executor(const Executor&) = delete;
	Executor& operator=(const Executor&) = delete;

	// Register a handler
	template<typename THandler, typename... Args>
		requires std::is_base_of_v<JobHandler, THandler> // We're using c++20 way of doing here, stick for static_assert if you prefer older style
	void registerHandler(Args&&... args)
	{
		if (m_running)
			throw std::runtime_error("Cannot register handler after start");

		auto handler = std::make_unique<THandler>(std::forward<Args>(args)...);
		if (handler->type().empty())
			throw std::runtime_error("Handler job type cannot be empty");

		std::cout << "[Executor] Registering handler for job type: " << handler->type() << "\n";

		auto [it, inserted] = m_handlers.emplace(handler->type(), std::move(handler));
		if (!inserted)
			throw std::runtime_error("Handler for this job type already registered");
	}

	// Takes the R-value reference explicitly to show that the job is moved into the executor
	void addJob(Job&& job);

	void start(std::size_t numThreads);
	void shutdown();

	// Notification callbacks
	void setJobStartedNotification(std::function<void(uint64_t)> callback)
	{
		m_notifyJobStarted = std::move(callback);
	}

	void setJobCompletedNotification(std::function<void(uint64_t, JobResult&&)> callback)
	{
		m_notifyJobCompleted = std::move(callback);
	}

private:
	void workerLoop();

	JobQueue m_queue{};
	std::unordered_map<std::string, std::unique_ptr<JobHandler>> m_handlers{};
	std::vector<std::jthread> m_workers{};
	std::atomic<bool> m_running { false };

	std::function<void(uint64_t)> m_notifyJobStarted{};
	std::function<void(uint64_t, JobResult&&)> m_notifyJobCompleted{};
};
