#pragma once
#include <memory>
#include <dpp/dpp.h>
#include "IWorkerAction.h"

class MessagesProcessor : public IWorkerAction
{
public:
	MessagesProcessor(std::shared_ptr<dpp::cluster> bot, dpp::snowflake guildId, dpp::snowflake channelId, std::string channelName, 
		dpp::snowflake lastMessage, dpp::guild_member_map members);

private:
	void fetchChannelMessagesCallBack(const dpp::confirmation_callback_t& callback);
	bool isValidReactor(dpp::user user, dpp::message message);
	// Inherited via IWorkerAction
	void onStartTask() override;

	std::shared_ptr<dpp::cluster> bot;
	dpp::snowflake guildId;
	dpp::snowflake channelId;
	std::string channelName;
	dpp::snowflake lastMessage;
	dpp::guild_member_map members;
};

