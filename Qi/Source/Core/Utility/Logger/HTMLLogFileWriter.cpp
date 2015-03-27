//
//  HTMLLogFileWriter.cpp
//  Qi Game Engine
//
//  Created by Cody White on 3/26/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "HTMLLogFileWriter.h"
#include "../../../Defines.h"

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

bool HTMLLogFileWriter::OpenFile(const std::string &filename, unsigned int flags)
{
    QI_ASSERT(!m_stream.is_open());
    
    // Append the html extension to the filename and open it.
    m_stream.open(filename + ".html");
    if (!m_stream)
    {
        return false;
    }
    
    // Check for different file flag features.
    if (flags & kForceFlush)
    {
        m_forceFlush = true;
    }
    
    // Write out the HTML header stuff to mark this file as containing HTML.
    m_stream << "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><title>Qi Game Engine Log</title></head></body>" << std::endl;
    
    return true;
}

bool HTMLLogFileWriter::CloseFile()
{
    QI_ASSERT(m_stream);
    
    // Write out the HTML footer stuff to mark this as the end of the HTML file.
    m_stream << "<font color=\"#000000\"><br><br>LOG END<br></font></body></html>" << std::endl;
    
    m_stream.flush();
    m_stream.close();
    m_forceFlush = false;
    
    return true;
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

bool HTMLLogFileWriter::WriteOutput(LogChannel channel, std::string &message)
{
    char buffer[256];
    
    ReplaceAll(message, "&", "&amp");
    ReplaceAll(message, "<", "&lt");
    ReplaceAll(message, ">", "&gt");
    ReplaceAll(message, "\n", "<br>");
    
    switch (channel)
    {
        case kInfo:
            sprintf(buffer, "<br><font color=\"#778899\">INFO: %s</front>", message.c_str());
            break;
            
        case kDebug:
            sprintf(buffer, "<br><font color=\"#0000FF\">DEBUG: %s</front>", message.c_str());
            break;
            
        case kWarning:
            sprintf(buffer, "<br><font color=\"#FFD700\"><b>WARNING: %s</b></front>", message.c_str());
            break;
            
        case kError:
            sprintf(buffer, "<br><font color=\"#FF0000\"><b>ERROR: %s</b></front>", message.c_str());
            break;
            
        default:
            break;
    }
    
    m_mutex.lock();
    m_stream << buffer << std::endl;
    m_mutex.unlock();
    
    return true;
}

} // namespace Qi

