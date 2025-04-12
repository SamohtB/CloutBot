#include "MessageHistoryProcessorThread.h"
#include "ThreadPool.h"
#include "FlagMonitor.h"
#include "Debug.h"

MessageHistoryProcessorThread::MessageHistoryProcessorThread(std::shared_ptr<dpp::cluster> bot) 
	: IETThread(), bot(bot)
{
	this->threadPool = std::make_unique<ThreadPool>(8);
	this->flagMonitor = std::make_unique<FlagMonitor>(false);
}

MessageHistoryProcessorThread::~MessageHistoryProcessorThread()
{
	this->flagMonitor->reportExit();
}

void MessageHistoryProcessorThread::addGuild(dpp::snowflake guildId)
{
	if (std::find(guildList.begin(), guildList.end(), guildId) == guildList.end()) 
	{
		guildList.push_back(guildId);

		return;
	}

	Debug::Log("Duplicate Guild ID dectected " + guildId.str());
}

void MessageHistoryProcessorThread::run()
{
	this->threadPool->startScheduler();
	this->flagMonitor->tryEnter();
}

void MessageHistoryProcessorThread::onFinishedExecution()
{

}
