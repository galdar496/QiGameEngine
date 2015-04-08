//
//  Logger.cpp
//  Qi Game Engine
//
//  Logging system for messages which can be displayed to the user.
//  Logged messages can go to different channels for filtering.
//
//  Created by Cody White on 1/11/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "Logger.h"
#include "HTMLLogFileWriter.h"
#include "../../Defines.h"
#include <stdarg.h>
#include <iomanip>
#include <sstream>

#define LOGFILE_NAME "QiLogfile"

namespace Qi
{

Logger::Logger() :
    m_initialized(false)
{
}

Logger::~Logger()
{
    if (m_initialized)
    {
        Deinit();
    }
}

Logger &Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

Result Logger::Init(LogFileType fileType, bool flushLogFile)
{
    QI_ASSERT(!m_initialized);
    
    Result result;
    
    // Default to all channels on.
    m_channelFilter = ~0;
    
    m_messageHandlers.resize(kNumChannels);
    
    // Determine the type of logfile to create and initialize
    // it.
    std::string outputFilename(LOGFILE_NAME);
    switch (fileType)
    {
        case kHTML:
        {
            outputFilename += ".html";
            m_fileWriter = new HTMLLogFileWriter();
        }
        break;
        
        default:
            QI_ASSERT(0 && "Unsupported logfile type");
            result.code = ReturnCode::kUnknownFileType;
            return result;
            break;
    }

    result = m_fileWriter->OpenFile(LOGFILE_NAME, flushLogFile);
    m_initialized = result.IsValid();
    
    return result;
}

void Logger::Deinit()
{
    m_fileWriter->CloseFile();
    
    delete m_fileWriter;
    m_fileWriter = nullptr;
    m_initialized = false;
}

void Logger::EnableChannel(LogChannel channel, bool enable)
{
    unsigned int filter = 1 << channel;
    m_channelFilter &= enable ? filter : ~filter;
}

void Logger::LogMessage(LogChannel channel, int line, const char *filename, const char *message, ...)
{
    QI_ASSERT(m_initialized && "Logger attempted to be used without being initialized");
    
    va_list args;
    va_start(args, message);
    char finalMessage[256];
    vsprintf(finalMessage, message, args);
    va_end(args);
    
    // Write the message to the file before invoking any message handlers. First color the
    // output for the file.
    std::stringstream logMessage;
    logMessage << filename << "(" << line << "): " << finalMessage;
    std::string messageCopy(logMessage.str());
    m_fileWriter->WriteOutput(channel, messageCopy);
    
    // Notify the handlers if we pass the channel filter test. Otherwise this message
    // will be ignored.
    unsigned int filter = 1 << channel;
    if (m_channelFilter & filter)
    {
        const PerChannelHandlers *handlers = &m_messageHandlers[channel];
        for (size_t ii = 0; ii < handlers->size(); ++ii)
        {
            handlers->at(ii)(finalMessage, channel);
        }
    }
}

void Logger::RegisterForMessages(const MessageEvent &handler, LogChannel channel)
{
    m_messageHandlers[channel].push_back(handler);
}

} // namespace Qi
