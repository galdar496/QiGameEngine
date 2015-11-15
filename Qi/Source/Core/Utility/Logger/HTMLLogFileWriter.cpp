//
//  HTMLLogFileWriter.cpp
//  Qi Game Engine
//
//  Created by Cody White on 3/26/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "HTMLLogFileWriter.h"
#include "../../Defines.h"
#include <ctime>
#include <chrono>

// If we're in windows, map _snprintf_s to snprintf for
// cross-platformability.
#ifdef QI_WINDOWS
	#define snprintf _snprintf_s
#endif

namespace Qi
{

HTMLLogFileWriter::HTMLLogFileWriter() :
    m_forceFlush(false)
{
}

HTMLLogFileWriter::~HTMLLogFileWriter()
{
    if (m_stream)
    {
        CloseFile();
    }
}

Result HTMLLogFileWriter::OpenFile(const std::string &filename, unsigned int flags)
{
    QI_ASSERT(!m_stream.is_open());
    
    // Append the html extension to the filename and open it.
    m_stream.open(filename + ".html");
    if (!m_stream)
    {
        return Result();
    }
    
    // Check for different file flag features.
    if (flags & kForceFlush)
    {
        m_forceFlush = true;
    }
    
    // Write out the HTML header stuff to mark this file as containing HTML.
    m_stream << "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><title>Qi Game Engine Log</title></head></body>" << std::endl;
	
	// Get the current time to write into the begging of the log.
	std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	char timeOutputBuffer[1024];
	ctime_s(timeOutputBuffer, 1024, &currentTime);
	m_stream << "<font color=\"#000000\"><br><br>LOG BEGIN -- " << timeOutputBuffer << " <br></font></body></html>" << std::endl;
    
    return Result(ReturnCode::kSuccess);
}

Result HTMLLogFileWriter::CloseFile()
{
    QI_ASSERT(m_stream);
    
    // Write out the HTML footer stuff to mark this as the end of the HTML file.
    m_stream << "<font color=\"#000000\"><br><br>LOG END<br></font></body></html>" << std::endl;
    
    m_stream.flush();
    m_stream.close();
    m_forceFlush = false;
    
    return Result(ReturnCode::kSuccess);
}

void ReplaceAll(std::string &message, const std::string& search, const std::string& replace)
{
    size_t pos = 0;
    while ((pos = message.find(search, pos)) != std::string::npos)
    {
        message.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

Result HTMLLogFileWriter::WriteOutput(LogChannel channel, std::string &message)
{
    char buffer[256];
    
    ReplaceAll(message, "&", "&amp");
    ReplaceAll(message, "<", "&lt");
    ReplaceAll(message, ">", "&gt");
    ReplaceAll(message, "\n", "<br>");
    
    switch (channel)
    {
        case kInfo:
			snprintf(buffer, 256, "<br><font color=\"#778899\">INFO: %s</front>", message.c_str());
			break;
            
        case kDebug:
			snprintf(buffer, 256, "<br><font color=\"#0000FF\">DEBUG: %s</front>", message.c_str());
            break;
            
        case kWarning:
            snprintf(buffer, 256, "<br><font color=\"#FFD700\"><b>WARNING: %s</b></front>", message.c_str());
            break;
            
        case kError:
            snprintf(buffer, 256, "<br><font color=\"#FF0000\"><b>ERROR: %s</b></front>", message.c_str());
            break;
            
        default:
            break;
    }
    
    m_mutex.lock();
    m_stream << buffer << std::endl;
    m_mutex.unlock();
    
    return Result(ReturnCode::kSuccess);
}

} // namespace Qi

