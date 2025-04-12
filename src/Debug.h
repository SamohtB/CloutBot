#pragma once
#include <iostream>
#include <vector>
#include <mutex>
#include <memory>

class Debug
{
public:
	enum Level : int {
		GENERAL = 0,
		FATAL = 1
	};

	static void Log(std::string msg);
	static void LogError(std::string msg);

private:

	static void Write(Level level, std::string message);
	static std::string CurrentTimestamp();
	static std::string LevelToString(Level level);

	static std::mutex logMutex;
};

