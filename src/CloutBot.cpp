#include "CloutBot.h"
#include "ScoreManager.h"

CloutBot::CloutBot(std::string bot_token)
{
	this->bot = std::shared_ptr<dpp::cluster>(bot_token);

	this->bot->on_log(dpp::utility::cout_logger());

	this->bot->on_ready([this](const dpp::ready_t& event)
	{

		Debug::Log("CloutBot Ready!");

		dpp::slashcommand ping_cmd("ping", "Replies with the username of the caller", this->bot->me.id);

		this->bot->global_command_create(ping_cmd);

		Debug::Log("Slash command /ping registered.");		
	});

	RegisterGuilds();

	this->bot->on_slashcommand([this](const dpp::slashcommand_t& event)
		{
			if (event.command.get_command_name() == "ping")
			{
				dpp::user caller = event.command.usr;

				std::string username = caller.format_username(); // username#discriminator
				std::string mention = caller.get_mention();      // <@user_id>

				event.reply("Command invoked by " + username + " (" + mention + ")");
				
			}
		});

	this->bot->start(dpp::start_type::st_wait);
}

void CloutBot::RegisterGuilds()
{
	this->bot->on_guild_create([this](const dpp::guild_create_t& event)
		{
			const dpp::guild& g = event.created;
			ScoreManager::getInstance()->AddGuild(g.id);
			Debug::Log("Joined guild: " + g.name + " (ID: " + g.id.str() + ")");
		});
}
