//
//  BaseTypes.h
//  Qi Game Engine
//
//  Created by Cody White on 4/7/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Definition of all types that are specific to the Qi Game Engine.
///

#include <stdint.h>

namespace Qi
{

// Type definitions.
typedef uint32_t uint32;

///
/// Return codes exposed by the engine.
///
enum class ReturnCode
{
    kSuccess,              ///< No error occurred.
    kUnknownError,         ///< An unknown error occured.
    kUnknownFileType,      ///< An unknown filetype was found.
    kOutOfMemory,          ///< The system has ran out of memory.
	kWindowCreationFailed, ///< Creation of the rendering window failed.
    kMissingConfigNode     ///< Missing a config node in the engine XML config file.
};

///
/// Definition of return code results for use with the engine.
///
struct Result
{
    explicit Result() : code(ReturnCode::kUnknownError) {}
    Result(ReturnCode returnCode) : code(returnCode) {}
    
    ///
    /// If true, then the result of the called function is valid (passed).
    ///
    /// @return True/false for validity.
    ///
    bool IsValid() const { return code == ReturnCode::kSuccess; }
    
    ReturnCode code; ///< Return code set by the called function.
};
    
} // namespace Qi