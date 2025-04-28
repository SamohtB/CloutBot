#include "GuildScheduler.h"

#include <chrono>
#include "Debug.h"
#include "MessagesProcessor.h"
#include "ThreadPool.h"

GuildScheduler::GuildScheduler(dpp::snowflake guildId, std::string name, std::vector<dpp::channel> channels, 
	std::shared_ptr<ThreadPool> sharedThreadPool, std::shared_ptr<dpp::cluster> bot)
	: IETThread(), guildId(guildId), name(name), channels(channels), 
	sharedThreadPool(sharedThreadPool), bot(bot)
{
	this->isRunning = true;
	for (int i = 0; i < channels.size(); i++)	
	{
		this->lastMessagesList.push_back(0);
	}

	auto cb = this->bot->co_guild_get_members(guildId, 100, 0).sync_wait();

	if (cb.is_error())
	{
		Debug::LogError("Failed to get members");
		return;
	}

	this->members = std::get<dpp::guild_member_map>(cb.value);
}

void GuildScheduler::stop()
{
	this->isRunning = false;
}

void GuildScheduler::run()
{
	while (isRunning)
	{
		for (int i = 0; i < channels.size(); i++)
		{
			auto channelId = channels[i].id;
			auto channelName = channels[i].name;

			std::shared_ptr<IWorkerAction> task = std::make_shared<MessagesProcessor>
				(this->bot, guildId, channelId, channelName, lastMessagesList[i], this->members);
			this->sharedThreadPool->scheduleTask(task);
		}

		this->sleep(5000);
	}
}
