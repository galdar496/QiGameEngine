//
//  Allocator.h
//  Qi Game Engine
//
//  Created by Cody White on 8/19/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Base class to use for defining different kinds of memory allocators to the memory system.
/// Only one memory allocator can be in use at any one time.
///

#include "../BaseTypes.h"

namespace Qi
{

class Allocator
{
    public:
    
		///
		/// Override this object to add fields for initialization.
		///
		struct Cinfo
		{
		};

		///
		/// Initialize the memory allocator for use. 
		///
		/// @param info Cinfo struct containing initialization information.
		/// @return Success or not.
		///
		virtual Result Init(const Cinfo *info) = 0;

		///
		/// Deinitialize the allocator. If an attempt to use the allocator is made 
		/// after calling this function, the engine may crash.
		///
		virtual void Deinit() = 0;

		///
		/// Check to see if this allocator has been properly initialized.
		///
		/// @return If true, this allocator has been properly initialized.
		///
		virtual bool IsInitialized() const = 0;

		///
		/// Allocate a user-defined amount of memory.
		///
		/// @param numBytes Number of bytes to allocate.
		/// @return Pointer to an allocated buffer.
		///
		virtual void *Allocate(size_t numBytes) const = 0;

		///
		/// Deallocates an allocated buffer.
		///
		/// @param address Address of the buffer to free. If
		///        null, this function will not do anything.
		///
		virtual void Deallocate(void *address) const = 0;
};

} // namespace Qi
