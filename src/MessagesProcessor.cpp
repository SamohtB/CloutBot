#include "MessagesProcessor.h"
#include "ScoreManager.h"
#include "Debug.h"

MessagesProcessor::MessagesProcessor(std::shared_ptr<dpp::cluster> bot, dpp::snowflake guildId, 
	dpp::snowflake channelId, std::string channelName, dpp::snowflake lastMessage,
	dpp::guild_member_map members)
	: bot(bot), guildId(guildId), channelId(channelId), channelName(channelName), 
	lastMessage(lastMessage), members(members)
{
}

void MessagesProcessor::onStartTask()
{
	Debug::Log("Messages Request");
	this->bot->messages_get(
		this->channelId,
		0,					//around 
		this->lastMessage,	//before
		0,					//after
		100,
		[this](const dpp::confirmation_callback_t& cb)
		{
			fetchChannelMessagesCallBack(cb);
		});
}

void MessagesProcessor::fetchChannelMessagesCallBack(const dpp::confirmation_callback_t& callback)
{
	if (callback.is_error())
	{
		Debug::LogError("Failed to get messages from channel " + this->channelName);
		return;
	}

	dpp::message_map messages = std::get<dpp::message_map>(callback.value);

	for (const auto& [message_id, message] : messages)
	{
		for (const auto& reaction : message.reactions)
		{
			int score = static_cast<int>(reaction.count);
			auto authorId = message.author.id;
			std::string emoji_str;
			if (reaction.emoji_id == 0) {
				emoji_str = reaction.emoji_name;  // Unicode emoji
			}
			else {
				emoji_str = "<:" + reaction.emoji_name + ":" + std::to_string(reaction.emoji_id) + ">";
			}

			const std::string& emoji_ref = emoji_str;

			if (score > 0)
			{
				try 
				{
					Debug::Log("Reactions Request");
					auto confirmation = this->bot->co_message_get_reactions(message, reaction.emoji_name, 0, 0, 100).sync_wait();

					if (confirmation.is_error()) 
					{
						Debug::LogError("Failed to fetch reactions for message " + std::to_string(message.id));
						return;
					}

					dpp::user_map users = std::get<dpp::user_map>(confirmation.value);

					for (const auto& [user_id, user] : users)
					{
						if (this->isValidReactor(user, message)) 
						{
							ScoreManager::getInstance()->addScore(this->guildId, authorId, +1);
							Debug::Log(message.author.global_name + " Scored!");
						}
					}
				}
				catch (const std::exception& e)
				{
					Debug::LogError(std::string("Exception during reaction fetch: ") + e.what());
				}
				catch (...) 
				{
					Debug::LogError("Unknown exception occurred during reaction fetch.");
				}				
			}
		}
	}
}

bool MessagesProcessor::isValidReactor(dpp::user user, dpp::message message)
{
	// Skip bots
	if (user.is_bot()) {
		return false;
	}

	// Skip self-reactions
	if (user.id == message.author.id) {
		return false;
	}

	// Check if user is still a guild member
	if (members.find(user.id) == members.end()) {
		return false;
	}

	return true;
}
