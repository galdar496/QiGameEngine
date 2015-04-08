//
//  HTMLLogFileWriter.h
//  Qi Game Engine
//
//  Created by Cody White on 3/26/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Implementation of LogFileWriter to write HTML log files.
///

#include "LogFileWriter.h"
#include <fstream>
#include <mutex>

namespace Qi
{

class HTMLLogFileWriter : public LogFileWriter
{
    public:
    
        HTMLLogFileWriter();
        virtual ~HTMLLogFileWriter();
    
        /// LogFilterWriter overrides ---------------
        virtual Result OpenFile(const std::string &filename, unsigned int flags) override;
        virtual Result CloseFile() override;
        virtual Result WriteOutput(LogChannel channel, std::string &message) override;
    
    private:
    
        bool m_forceFlush;      ///< If true, flush the file after every write.
        std::ofstream m_stream; ///< Output filestream to write to.
        std::mutex m_mutex;     ///< Mutex to use for ensuring output is properly ordered.
};

} // namespace Qi
