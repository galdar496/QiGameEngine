//
//  Logger.h
//  Qi Game Engine
//
//  Created by Cody White on 1/11/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <mutex>
#include "../../ThirdParty/FastDelegate.h"

namespace Qi
{

///
/// Logging system for messages which can be displayed to the user.
/// Logged messages can go to different channels for message filtering.
/// At any time, certain channels can be enabled/disabled by providing
/// a message filter. Note that this class is a singleton.
///

// Convenience defines for logging.
#if DEBUG
    #define Qi_LogInfo(message, args...) Qi::Logger::GetInstance().LogMessage(Qi::Logger::kInfo, __LINE__, __FILE__, message, ##args);
    #define Qi_LogDebug(message, args...) Qi::Logger::GetInstance().LogMessage(Qi::Logger::kDebug, __LINE__, __FILE__, message, ##args);
#else
    #define Qi_LogInfo(message, args...)
    #define Qi_LogDebug(message, args...)
#endif
#define Qi_LogWarning(message, args...) Qi::Logger::GetInstance().LogMessage(Qi::Logger::kWarning, __LINE__, __FILE__, message, ##args);
#define Qi_LogError(message, args...) Qi::Logger::GetInstance().LogMessage(Qi::Logger::kError, __LINE__, __FILE__, message, ##args);

class Logger
{
    public:
    
        ///
        /// Channels for logging messages to. For custom channels, start
        /// at 'kCustom' and add any necessary channels there.
        ///
        enum Channel
        {
            kInfo,        ///< Informational messages
            kDebug,       ///< Debug messages
            kWarning,     ///< Warnings that will not directly cause problems
            kError,       ///< Serious errors that will prevent the system from running correctly
            kCustom,      ///< Other messages
            kNumChannels, ///< Always keep this last for count reasons.
        };
    
        ///
        /// Instance accessor to get to the singleton object.
        /// @return Static instance of Logger.
        ///
        static Logger &GetInstance();
    
        ///
        /// Initialize the logging system for use.
        /// @param flushLogFile If true, flush the log file after each write.
        /// @return Success of initialization.
        ///
        bool Init(bool flushLogFile);
    
        ///
        /// Deinitialize the logging system. The logger cannot be used after
        /// a call to this function without first calling 'initialize()'.
        ///
        void Deinit();
    
        ///
        /// Enable/disable a logging channel.
        /// @param channel Channel to enable/disable.
        /// @param enable Whether or not to enable the specified channel.
        ///
        void EnableChannel(Channel channel, bool enable);
    
        ///
        /// Log a message. A message is automatically written to the log file. Additionally,
        /// any registered message handlers will get invoked should this specific channel be
        /// active.
        /// @param channel Channel to log the message to.
        /// @param line Line number where the message occured (for debugging).
        /// @param filename Filename which generated the message (for debugging).
        /// @param message Message text to log.
        ///
        void LogMessage(Channel channel, int line, const char *filename, const char *message, ...);
    
    
        /// Event handler to register for when you want to receive message events.
        /// First argument is the message, the second is the channel ID.
        typedef FastDelegate <void (const char *, Channel)> MessageEvent;
    
        ///
        /// Register for a message event for a particular channel.
        /// @param handler A message handler of type 'MessageEvent'
        /// @param channel Channel to register for. Any messages going to this channel will invoke
        ///                the specified handler.
        ///
        void RegisterForMessages(const MessageEvent &handler, Channel channel);
    
    private:
    
        Logger();
        ~Logger();
    
        // Purposely unimplemented.
        Logger(const Logger &other) = delete;
        Logger &operator=(const Logger &other) = delete;
    
        // One vector per channel. Note that vectors are in use here instead of Qi::Array
        // to avoid a circular dependency between the logger and the memory manager.
        typedef std::vector<const MessageEvent> PerChannelHandlers;
        typedef std::vector<PerChannelHandlers> MessageHandlers;
        MessageHandlers m_messageHandlers; ///< All registered message handlers ordered by channel.
    
        bool m_initialized;            ///< If true, the logger has been initialized.
        bool m_forceFlush;             ///< If true, flush the log file after each write.
        std::ofstream m_output;        ///< Log file all messages are written to.
        unsigned int m_channelFilter; ///< Filter for each channel. Every bit corresponds to a different channel. 1 is on, 0 is off.
    
        std::mutex m_mutex; ///< Mutex used for locking while writing to the output file.
    
        std::string m_colorTable[kNumChannels]; ///< Color table to use for color coding per-channel output to the log file.
};

} // namespace Qi