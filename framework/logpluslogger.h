
#ifndef FRAMEWORK_LOG4PLUS_H_
#define FRAMEWORK_LOG4PLUS_H_

#include <log4cplus/logger.h>  
#include <log4cplus/configurator.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/loggingmacros.h>


#include "framework/logger.h"

class LogPlusLogger : public Logger
{
public:
	LogPlusLogger() {}
	~LogPlusLogger() {}

	virtual void Log(LogLevel level, const char* filename, int line, const char* message,bool ifSysLog); 
	virtual void Log(const LogMessage & msg);

public:
	static log4cplus::Logger  server_logger_;
	static log4cplus::Logger  sys_server_logger_;
	static log4cplus::Logger  run_logger_;

};







#endif  // FRAMEWORK_BASE_LOGGING_H_
