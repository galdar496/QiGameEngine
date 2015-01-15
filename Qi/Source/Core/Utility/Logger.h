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

#include <fstream>
#include <vector>
#include "FastDelegate.h"

namespace Qi
{

// Convenience defines for logging.
#if DEBUG
    #define Qi_LogInfo(message, args...) Logger::getInstance().log(Logger::kInfo, __LINE__, __FILE__, message, ##args);
    #define Qi_LogDebug(message, args...) Logger::getInstance().log(Logger::kDebug, __LINE__, __FILE__, message, ##args);
#else
    #define Qi_LogInfo(message, args...)
    #define Qi_LogDebug(message, args...)
#endif
#define Qi_LogWarning(message, args...) Logger::getInstance().log(Logger::kWarning, __LINE__, __FILE__, message, ##args);
#define Qi_LogError(message, args...) Logger::getInstance().log(Logger::kError, __LINE__, __FILE__, message, ##args);

class Logger
{
    public:
    
        // Channels for logging messages to. For custom channels, start
        // at 'kCustom' and add any necessary channels there.
        enum Channel
        {
            kInfo,        // Informational messages
            kDebug,       // Debug messages
            kWarning,     // Warnings that will not directly cause problems
            kError,       // Serious errors that will prevent the system from running correctly
            kCustom,      // Other messages
            kNumChannels, // Always keep this last for count reasons.
        };
    
        /**
          * Instance accesor.
          */
        static Logger &getInstance();
    
        /**
          * Initialize the logging system.
          */
        bool initialize();
    
        /**
          * Close any input files and shutdown.
          */
        void deinitialize();
    
        /**
          * Enable/disable a specific channel.
          */
        void enableChannel(Channel channel, bool enable);
    
        /**
         * Log a message.
         * @param channel Channel to log the message to.
         * @param line Line number where the message occured (for debugging).
         * @param filename Filename which generated the message (for debugging).
         * @param message Message to log.
         */
        void log(Channel channel, int line, const char *filename, const char *message, ...);
    
    
        /// Event handler to register for when you want to receive message events.
        /// First argument is the message, the second is the channel ID.
        typedef FastDelegate <void (const char *, Channel)> MessageEvent;
    
        /**
          * Register for a message event for a particular channel.
          */
        void registerForMessages(const MessageEvent &handler, Channel channel);
    
    private:
    
        /**
          * Default constructor.
          */
        Logger();
    
        /**
          * Destructor.
          */
        ~Logger();
    
        // One vector per channel.
        typedef std::vector<const MessageEvent> PerChannelHandlers;
        typedef std::vector<PerChannelHandlers> MessageHandlers;
        MessageHandlers m_message_handlers;
    
        bool m_initialized;
        std::ofstream m_output;
        unsigned int m_channel_filter; // Filter for each channel. Every bit corresponds to a different channel. 1 is on, 0 is off.
};

} // namespace Qi