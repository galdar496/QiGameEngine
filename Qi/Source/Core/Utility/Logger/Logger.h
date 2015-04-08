//
//  Logger.h
//  Qi Game Engine
//
//  Created by Cody White on 1/11/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include <string>
#include <vector>
#include "../../../ThirdParty/FastDelegate.h"
#include "../../BaseTypes.h"
#include "LogChannels.h"

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
    #define Qi_LogInfo(message, ...) Qi::Logger::GetInstance().LogMessage(Qi::LogChannel::kInfo, __LINE__, __FILE__, message, ##__VA_ARGS__);
    #define Qi_LogDebug(message, ...) Qi::Logger::GetInstance().LogMessage(Qi::LogChannel::kDebug, __LINE__, __FILE__, message, ##__VA_ARGS__);
#else
    #define Qi_LogInfo(message, ...)
    #define Qi_LogDebug(message, ...)
#endif
#define Qi_LogWarning(message, ...) Qi::Logger::GetInstance().LogMessage(Qi::LogChannel::kWarning, __LINE__, __FILE__, message, ##__VA_ARGS__);
#define Qi_LogError(message, ...) Qi::Logger::GetInstance().LogMessage(Qi::LogChannel::kError, __LINE__, __FILE__, message, ##__VA_ARGS__);

// Forward declarations.
class LogFileWriter;

class Logger
{
    public:
    
        ///
        /// Instance accessor to get to the singleton object.
        ///
        /// @return Static instance of Logger.
        ///
        static Logger &GetInstance();
    
        ///
        /// File types the logger supports creating. Pass the type you want
        /// to write to 'Init'.
        ///
        enum LogFileType
        {
            kHTML, ///< Create an HTML log file.
        };
    
        ///
        /// Initialize the logging system for use.
        ///
        /// @param fileType Type of file to create for the log file. See 'enum LogFileType' for
        ///                 the available options.
        /// @param flushLogFile If true, flush the log file after each write.
        /// @return Success of initialization.
        ///
        Result Init(LogFileType fileType, bool flushLogFile);
    
        ///
        /// Deinitialize the logging system. The logger cannot be used after
        /// a call to this function without first calling 'Init()'.
        ///
        void Deinit();
    
        ///
        /// Enable/disable a logging channel.
        ///
        /// @param channel Channel to enable/disable.
        /// @param enable Whether or not to enable the specified channel.
        ///
        void EnableChannel(LogChannel channel, bool enable);
    
        ///
        /// Log a message. A message is automatically written to the log file. Additionally,
        /// any registered message handlers will get invoked should this specific channel be
        /// active.
        ///
        /// @param channel Channel to log the message to.
        /// @param line Line number where the message occured (for debugging).
        /// @param filename Filename which generated the message (for debugging).
        /// @param message Message text to log.
        ///
        void LogMessage(LogChannel channel, int line, const char *filename, const char *message, ...);
    
    
        ///
        /// Event handler to register for when you want to receive message events.
        /// First argument is the message, the second is the channel ID.
        ///
        typedef FastDelegate <void (const char *, LogChannel)> MessageEvent;
    
        ///
        /// Register for a message event for a particular channel.
        ///
        /// @param handler A message handler of type 'MessageEvent'
        /// @param channel Channel to register for. Any messages going to this channel will invoke
        ///                the specified handler.
        ///
        void RegisterForMessages(const MessageEvent &handler, LogChannel channel);
    
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
        unsigned int m_channelFilter;  ///< Filter for each channel. Every bit corresponds to a different channel. 1 is on, 0 is off.
    
        LogFileWriter *m_fileWriter; ///< Handles all writing of log messages to a file.
};

} // namespace Qi