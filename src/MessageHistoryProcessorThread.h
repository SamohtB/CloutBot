#pragma once
#include <dpp/dpp.h>
#include <vector>
#include <memory>
#include "IETThread.h"
#include "IExecutionEvent.h"

class ThreadPool;
class FlagMonitor;
class GuildScheduler;

class MessageHistoryProcessorThread : public IETThread, IExecutionEvent
{
public:
	using GuildList = std::vector<dpp::snowflake>;
	using SchedulerList = std::vector<std::shared_ptr<GuildScheduler>>;
	MessageHistoryProcessorThread(std::shared_ptr<dpp::cluster> bot);
	~MessageHistoryProcessorThread();

	void addGuild(dpp::snowflake guildId, std::string name);

private:

	void onGuildChannelsFetched(dpp::snowflake guildId, std::string name, const dpp::confirmation_callback_t& callback);
	// Inherited via IETThread
	void run() override;
	// Inherited via IExecutionEvent
	void onFinishedExecution() override;


	std::unique_ptr<FlagMonitor> flagMonitor;
	std::shared_ptr<ThreadPool> threadPool;
	std::shared_ptr<dpp::cluster> bot;

	SchedulerList schedulerList;
	GuildList guildList;
};

