#pragma once
class IFinishedTask
{
public:
	virtual void onFinishedTask(int threadID) = 0;
};