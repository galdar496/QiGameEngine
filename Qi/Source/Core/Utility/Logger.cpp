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
#include <assert.h>
#include <stdarg.h>
#include <iomanip>

// ASCII color codes
#define COLOR_HEADER "\x1b["
#define WHITE "37m"
#define YELLOW "33m"
#define RED "31m"
#define DIM "2;"
#define RESET "\x1b[0m"
#define BOLD "\x1b[1m"

#define LOGFILE_NAME "QiLogfile.txt"

namespace Qi
{

Logger::Logger() :
    m_initialized(false),
    m_forceFlush(false),
    m_color_table {std::string(COLOR_HEADER) + std::string(DIM) + std::string(WHITE), // kInfo
                   std::string(COLOR_HEADER) + std::string(WHITE),                    // kDebug
                   std::string(COLOR_HEADER) + std::string(YELLOW),                   // kWarning
                   std::string(COLOR_HEADER) + std::string(RED),                      // kError
                   std::string(COLOR_HEADER) + std::string(WHITE)                     // kCustom
                  }
{
}

Logger::~Logger()
{
    m_output.close();
}

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

bool Logger::init(bool flushLogFile)
{
    assert(!m_initialized);
    
    // Default to all channels on.
    m_channel_filter = ~0;
    
    m_message_handlers.resize(kNumChannels);
    
    m_output.open(LOGFILE_NAME);
    if (m_output.good())
    {
        m_output << BOLD << "Qi Game Engine Runtime Log" << RESET << std::endl;
        m_output << std::setfill('-') << std::setw(80) << "-" << std::endl;
        m_output << std::setfill(' ');
        m_initialized = true;
    }
    
    m_forceFlush = flushLogFile;
    
    return m_initialized;
}

void Logger::deinit()
{
    m_output.flush();
    m_output.close();
    m_initialized = false;
}

void Logger::enableChannel(Channel channel, bool enable)
{
    unsigned int filter = 1 << channel;
    m_channel_filter &= enable ? filter : ~filter;
}

void Logger::logMessage(Channel channel, int line, const char *filename, const char *message, ...)
{
    assert(m_initialized && "Logger attempted to be used without being initialized");
    
    va_list args;
    va_start( args, message );
    char final_message[256];
    vsprintf(final_message, message, args);
    va_end( args );
    
    // Write the message to the file before invoking any message handlers. First color the
    // output for the file.
    m_mutex.lock();
    m_output << m_color_table[channel] << filename << "(" << line << "): " << final_message << RESET << "\n";
    if (m_forceFlush)
    {
        m_output.flush();
    }
    m_mutex.unlock();
    
    // Notify the handlers if we pass the channel filter test. Otherwise this message
    // will be ignored.
    unsigned int filter = 1 << channel;
    if (m_channel_filter & filter)
    {
        const PerChannelHandlers *handlers = &m_message_handlers[channel];
        for (size_t ii = 0; ii < handlers->size(); ++ii)
        {
            handlers->at(ii)(final_message, channel);
        }
    }
}

void Logger::registerForMessages(const MessageEvent &handler, Channel channel)
{
    m_message_handlers[channel].push_back(handler);
}

} // namespace Qi
