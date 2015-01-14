//
//  Logger.h
//  Qi Game Engine
//
//  Logging system for messages which can be displayed to the user.
//  Logged messages can go to different channels for filtering.
//
//  Created by Cody White on 1/11/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include <string>
#include <vector>

namespace Qi
{

// Convenience defines for logging.
#define LOG_INFO(message, args...) Logger::getInstance().log(Logger::Channel::kInfo, __LINE__, __FILE__, message, ##args);
#define LOG_WARNING(message, args...) Logger::getInstance().log(Logger::Channel::kWarning, __LINE__, __FILE__, message, ##args);
#define LOG_ERROR(message, args...) Logger::getInstance().log(Logger::Channel::kError, __LINE__, __FILE__, message, ##args);

class Logger
{
    public:
    
        // Channels for logging messages to. For custom channels, start
        // at 'kCustom' and add any necessary channels there.
        enum class Channel
        {
            kInfo,        // Informational messages
            kWarning,     // Warnings that will not directly cause problems
            kError,       // Serious errors that will prevent the system from running correctly
            kCustom,      // Other messages
            kNumChannels, // Always keep this last for count reasons.
        };
    
        typedef std::vector<std::string> MessageBuffer;
    
        /**
          * Instance accesor.
          */
        static Logger &getInstance();
    
        /**
          * Initialize the logging system.
          */
        bool initialize();
    
        /**
         * Log a message.
         * @param channel Channel to log the message to.
         * @param line Line number where the message occured (for debugging).
         * @param filename Filename which generated the message (for debugging).
         * @param message Message to log.
         */
        void log(Channel channel, int line, const char *filename, const char *message, ...);
    
        /**
          * Get the most recently logged message for a particular channel.
          */
        const std::string &getLastMessage(Channel channel);
    
        /**
          * Get all messages for a particular channel.
          */
        const MessageBuffer &getAllMessages(Channel channel);
    
    private:
    
        /**
          * Default constructor.
          */
        Logger();
    
        /**
          * Destructor.
          */
        ~Logger();
    
        // Messages are stored according to channel.
        typedef std::vector<MessageBuffer> Messages;
        Messages m_messages;
        bool m_initialized; 
};

} // namespace Qi