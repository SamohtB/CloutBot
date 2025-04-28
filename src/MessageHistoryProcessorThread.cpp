#include "MessageHistoryProcessorThread.h"
#include "ThreadPool.h"
#include "FlagMonitor.h"
#include "GuildScheduler.h"
#include "Debug.h"

MessageHistoryProcessorThread::MessageHistoryProcessorThread(std::shared_ptr<dpp::cluster> bot) 
	: IETThread(), bot(bot)
{
	this->threadPool = std::make_shared<ThreadPool>("Master Processor", 8);
	this->flagMonitor = std::make_unique<FlagMonitor>(false);
}

MessageHistoryProcessorThread::~MessageHistoryProcessorThread()
{
	this->flagMonitor->reportExit();
	for (const auto& scheduler : this->schedulerList)
	{
		scheduler->stop();
	}
}

void MessageHistoryProcessorThread::addGuild(dpp::snowflake guildId, std::string name)
{
	if (std::find(guildList.begin(), guildList.end(), guildId) == guildList.end()) 
	{
		guildList.push_back(guildId);

		Debug::Log("Guild Channels Request");
		/* Retrieve accessible text channels */
		this->bot->channels_get(guildId,
			[this, guildId, name](const dpp::confirmation_callback_t& callback)
			{
				this->fetchGuildChannelsCallback(guildId, name, callback);
			});

		return;
	}

	Debug::Log("Duplicate Guild ID dectected " + guildId.str());
}

void MessageHistoryProcessorThread::fetchGuildChannelsCallback(dpp::snowflake guildId, std::string name, const dpp::confirmation_callback_t & callback)
{
	if (callback.is_error()) 
	{
		Debug::LogError("Failed to get channels for guild " + guildId.str());
		return;
	}

	dpp::channel_map channels = std::get<dpp::channel_map>(callback.value);

	Debug::Log("Bot User Request");
	this->bot->guild_get_member(guildId, this->bot->me.id, [this, guildId, channels, name](const dpp::confirmation_callback_t& member_cb)
		{
			if (member_cb.is_error()) {
				Debug::Log("Failed to get bot member for guild " + guildId.str());
				return;
			}

			std::vector<dpp::channel> textChannels;
			const dpp::guild_member& botMember = std::get<dpp::guild_member>(member_cb.value);

			for (const auto& [channel_id, channel] : channels)
			{
				if (channel.is_text_channel() && (channel.get_user_permissions(botMember) & dpp::p_view_channel))
				{
					textChannels.push_back(channel);
					//Debug::Log("Channel Added: " + channel.name);
				}
				//else
				//{
				//	Debug::Log("No Access: " + channel.name);
				//}
			}

			/* create sheduler to schedule tasks per guild */
			auto guildScheduler = std::make_shared<GuildScheduler>(guildId, name, textChannels, this->threadPool, this->bot);
				this->schedulerList.push_back(guildScheduler);
				guildScheduler->start();

			Debug::Log("Guild Added Successfully: " + name + 
				"\n" + "Channels Managed: " + std::to_string(textChannels.size()));
		});	
}

void MessageHistoryProcessorThread::run()
{
	this->threadPool->startScheduler();
	this->flagMonitor->tryEnter(); // sleep thread so it doesn't exit
}
