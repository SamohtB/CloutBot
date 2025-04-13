#include "GuildScheduler.h"

GuildScheduler::GuildScheduler(dpp::snowflake guildId, std::string name, std::vector<dpp::channel> channels, std::shared_ptr<ThreadPool> sharedThreadPool)
	: IETThread(), guildId(guildId), name(name), channels(channels), sharedThreadPool(sharedThreadPool)
{
}

void GuildScheduler::run()
{
}
