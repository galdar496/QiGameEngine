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

namespace Qi
{

/**
 * Default constructor.
 */
Logger::Logger() :
    m_initialized(false)
{
}

/**
 * Destructor.
 */
Logger::~Logger()
{
    m_output.close();
}

/**
 * Instance accesor.
 */
Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

/**
  * Initialize the logging system.
  */
bool Logger::initialize()
{
    // Default to all channels on.
    m_channel_filter = ~0;
    
    m_message_handlers.resize(kNumChannels);
    
    m_output.open("QiRunLog.txt");
    if (m_output.good())
    {
        m_output << BOLD << "Qi Game Engine Runtime Log" << RESET << std::endl;
        m_output << std::setfill('-') << std::setw(80) << "-" << std::endl;
        m_output << std::setfill(' ');
        m_initialized = true;
    }
    
    return m_initialized;
}

/**
 * Close any input files and shutdown.
 */
void Logger::deinitialize()
{
    m_output.close();
    m_initialized = false;
}

/**
 * Enable/disable a specific channel.
 */
void Logger::enableChannel(Channel channel, bool enable)
{
    unsigned int filter = 1 << channel;
    m_channel_filter &= enable ? filter : ~filter;
}

/**
 * Log a message.
 * @param channel Channel to log the message to.
 * @param line Line number where the message occured (for debugging).
 * @param filename Filename which generated the message (for debugging).
 * @param message Message to log.
 */
void Logger::log(Channel channel, int line, const char *filename, const char *message, ...)
{
    assert(m_initialized && "Logger attempted to be used without being initialized");
    
    va_list args;
    va_start( args, message );
    char final_message[256];
    vsprintf(final_message, message, args);
    va_end( args );
    
    // Write the message to the file before invoking any message handlers. First color the
    // output for the file.
    std::string color = COLOR_HEADER;
    switch (channel)
    {
        case kInfo:
            color += DIM;
            color += WHITE;
            break;
            
        case kDebug:
            color += WHITE;
            break;
        
        case kWarning:
            color += YELLOW;
            break;
        
        case kError:
            color += RED;
            break;
            
        default:
            assert(0 && "Invalid channel specified");
            break;
    }
    m_output << color << filename << "(" << line << "): " << final_message << RESET << std::endl;
    
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

/**
 * Register for a message event for a particular channel.
 */
void Logger::registerForMessages(const MessageEvent &handler, Channel channel)
{
    m_message_handlers[channel].push_back(handler);
}

} // namespace Qi
