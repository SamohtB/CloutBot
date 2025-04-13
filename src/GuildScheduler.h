#pragma once
#include "IETThread.h"
#include "dpp/dpp.h"

class ThreadPool;

class GuildScheduler :   public IETThread
{
public:
	GuildScheduler(dpp::snowflake guildId, std::string name, std::vector<dpp::channel> channels, std::shared_ptr<ThreadPool> sharedThreadPool);

private:

	// Inherited via IETThread
	void run() override;

	dpp::snowflake guildId;
	std::string name;
	std::vector<dpp::channel> channels;
	std::shared_ptr<ThreadPool> sharedThreadPool;
};

