#pragma once
#include "IETThread.h"
#include "dpp/dpp.h"
#include "IWorkerAction.h"

class ThreadPool;

class GuildScheduler :   public IETThread
{
public:
	GuildScheduler(dpp::snowflake guildId, std::string name, std::vector<dpp::channel> channels, std::shared_ptr<ThreadPool> sharedThreadPool, std::shared_ptr<dpp::cluster> bot);

	void stop();
private:
	// Inherited via IETThread
	void run() override;

	dpp::snowflake guildId;
	std::string name;
	std::vector<dpp::channel> channels;
	std::shared_ptr<ThreadPool> sharedThreadPool;
	std::shared_ptr<dpp::cluster> bot;
	bool isRunning = false;
	std::vector<dpp::snowflake> lastMessagesList;
	dpp::guild_member_map members;

	
};

