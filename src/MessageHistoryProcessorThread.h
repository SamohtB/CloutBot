#pragma once
#include <dpp/dpp.h>
#include <vector>
#include <memory>
#include "IETThread.h"
#include "IExecutionEvent.h"

class ThreadPool;
class FlagMonitor;

class MessageHistoryProcessorThread : public IETThread, IExecutionEvent
{
public:
	using GuildList = std::vector<dpp::snowflake>;
	MessageHistoryProcessorThread(std::shared_ptr<dpp::cluster> bot);
	~MessageHistoryProcessorThread();

	void addGuild(dpp::snowflake guildId);

private:

	// Inherited via IETThread
	void run() override;

	// Inherited via IExecutionEvent
	void onFinishedExecution() override;

	std::unique_ptr<ThreadPool> threadPool;
	std::unique_ptr<FlagMonitor> flagMonitor;
	std::shared_ptr<dpp::cluster> bot;
	GuildList guildList;
};

