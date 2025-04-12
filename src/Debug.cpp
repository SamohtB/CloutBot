#include "Debug.h"
#include <chrono>
#include <iomanip>
#include <sstream>

std::mutex Debug::logMutex;

void Debug::Write(Level level, std::string message)
{
	std::string levelColor;

	switch (level)
	{
	case 0:		levelColor = "\033[37m";	break;
	//case WARNING:	levelColor = "\033[33m";	break;
	case 1:		levelColor = "\033[31m";	break;
	default:		levelColor = "\033[0m";		break;
	}

	std::string fullMessage = "[" + CurrentTimestamp() + "] "
						+ levelColor + "[" + LevelToString(level) + "] " + "\033[0m"
						+ message;

	std::lock_guard<std::mutex> lock(logMutex);
	std::cout << fullMessage << std::endl;

}

std::string Debug::CurrentTimestamp()
{
	auto now = std::chrono::system_clock::now();
	time_t now_c = std::chrono::system_clock::to_time_t(now);

	tm time;
	localtime_s(&time, &now_c);

	std::ostringstream ss;
	ss << std::put_time(&time, "%Y-%m-%d %H:%M:%S");

	return ss.str();
}

std::string Debug::LevelToString(Level level)
{
	switch (level)
	{
	case 0:     return "LOG";
	//case WARNING: return "WARNING";
	case 1:   return "ERROR";
	default:      return "UNKNOWN";
	}
}

void Debug::Log(std::string msg)
{
	Write(Level::GENERAL, msg);
}

void Debug::LogError(std::string msg)
{
	Write(Level::FATAL, msg);
}



