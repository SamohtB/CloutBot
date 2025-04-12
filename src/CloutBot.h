#pragma once
#include <string>
#include <dpp/dpp.h>
#include <memory>

#include "Debug.h"

class CloutBot
{
public:
	CloutBot(std::string bot_token);

	void RegisterGuilds();

private:
	std::shared_ptr<dpp::cluster> bot;
};

