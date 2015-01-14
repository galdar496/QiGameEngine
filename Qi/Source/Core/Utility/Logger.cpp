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
#include <sstream>
#include <assert.h>
#include <stdarg.h>

namespace Qi
{

/**
 * Default constructor.
 */
Logger::Logger() :
    m_initialized(false)
{
    m_messages.resize(static_cast<int>(Channel::kNumChannels));
}

/**
 * Destructor.
 */
Logger::~Logger()
{
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
    m_messages.resize(static_cast<int>(Channel::kNumChannels));
    m_initialized = true;
    return m_initialized;
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
    va_list args;
    va_start( args, message );
    char final_message[256];
    vsprintf(final_message, message, args);
    va_end( args );
    
    std::stringstream stream;
    #if DEBUG
        stream << filename << "(" << line << "): ";
    #endif
    stream << final_message;
    m_messages[static_cast<int>(channel)].push_back(stream.str());
}

/**
 * Get the most recently logged message for a particular channel.
 */
const std::string &Logger::getLastMessage(Channel channel)
{
    assert(m_initialized && "Logger is being used before being initialized");
    MessageBuffer &messages = m_messages[static_cast<int>(channel)];
    assert(messages.size() && "No messages to get");
    return messages[messages.size() - 1];
}

/**
 * Get all messages for a particular channel.
 */
const Logger::MessageBuffer &Logger::getAllMessages(Channel channel)
{
    assert(m_initialized && "Logger is being used before being initialized");
    return m_messages[static_cast<int>(channel)];
}


} // namespace Qi
