#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <dpp/dpp.h>

class ScoreManager
{
public:
	using GuildId = dpp::snowflake;
	using UserId = dpp::snowflake;
	using ScoreTable = std::unordered_map<UserId, int>;
	using GuildTable = std::unordered_map<GuildId, ScoreTable>;

	static ScoreManager* getInstance();

	bool addScore(GuildId guild, UserId user, int score);
	void addGuild(GuildId guild);

private:
	ScoreManager();
	~ScoreManager();
	ScoreManager(ScoreManager const&) {}
	ScoreManager& operator=(ScoreManager const&) {}
	
	static ScoreManager* sharedInstance;
	GuildTable guildScoreTable;
};

