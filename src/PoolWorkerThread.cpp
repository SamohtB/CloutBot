#include "PoolWorkerThread.h"
#include "IWorkerAction.h"
#include "IFinishedTask.h"
#include "ThreadPool.h"

PoolWorkerThread::PoolWorkerThread(int id, IFinishedTask* finished_task) : id(id), finishedTask(finished_task) {}

PoolWorkerThread::~PoolWorkerThread() {}

int PoolWorkerThread::getThreadID()
{
	return this->id;
}

void PoolWorkerThread::assignTask(std::shared_ptr<IWorkerAction> action)
{
	this->action = action;
}

void PoolWorkerThread::run()
{
	this->action->onStartTask();
	this->finishedTask->onFinishedTask(this->id);
}
