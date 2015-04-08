//
//  LogFileWriter.h
//  Qi Game Engine
//
//  Created by Cody White on 3/26/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Interface for different file writes for the log file. Inherit from this object
/// and override any virtual functions to write a specific type of file.
///

#include <string>
#include "LogChannels.h"
#include "../../BaseTypes.h"

namespace Qi
{

class LogFileWriter
{
    public:
    
        virtual ~LogFileWriter() {}
    
        ///
        /// File opening/operation flags. Set these while
        /// opening a new log file.
        ///
        enum FileFlags
        {
            kForceFlush = (1 << 0), ///< Force the log file to flush after ever write.
        };
    
        ///
        /// Open a file for writing to. The file will contain
        ///
        /// @param filename Path to write the log file to.
        /// @param flags File opening/operation options. See 'enum FileFlags'.
        /// @return If true, the file was successfully opened and ready for use.
        ///
        virtual Result OpenFile(const std::string &filename, unsigned int flags) = 0;
    
        ///
        /// Close an open file log file. Performs any pending flush operations
        /// that may need to run before closing the file to avoid losing any data.
        ///
        /// @return If true, the file was successfully closed.
        ///
        virtual Result CloseFile() = 0;
    
        ///
        /// Write a chunk of text to the output file. The text will be formatted
        /// based on the specific file writer being used.
        ///
        /// @param channel Log channel that reported this message. Used for color-coding
        ///                the messages.
        /// @param message Message to write to the log file.
        /// @return If true, the message was succesfully written to the logfile.
        ///
        virtual Result WriteOutput(LogChannel channel, std::string &message) = 0;
};

} // namespace Qi
