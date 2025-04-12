#include "ScoreManager.h"

ScoreManager* ScoreManager::sharedInstance = nullptr;

ScoreManager* ScoreManager::getInstance()
{
    if (sharedInstance == nullptr)
        sharedInstance = new ScoreManager();

    return sharedInstance;
}

ScoreManager::ScoreManager() {}
ScoreManager::~ScoreManager()
{
    delete sharedInstance;
}

bool ScoreManager::AddScore(GuildId guild, UserId user, int score)
{
    bool isNewUser = false;
    ScoreTable scoreTable = this->guildScoreTable[guild];
   
    if (scoreTable.find(user) != scoreTable.end())
    {
        isNewUser = true;
        this->guildScoreTable[guild][user] = 0;
    }

    this->guildScoreTable[guild][user] += score;
    return isNewUser;
}

void ScoreManager::AddGuild(GuildId guild)
{
    this->guildScoreTable.emplace(guild, ScoreTable());
}
