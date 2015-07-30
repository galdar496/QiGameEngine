//
//  TightlyPackedArray.h
//  Qi Game Engine
//
//  Created by Cody White on 3/1/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Create and store a tightly packed array which re-orders itself if any of its
/// elements have been removed. All data should be accessed via handles which are 
/// maintained by this object. This object is sized once and will never grow/shrink.
///

#include "../Defines.h"
#include "../BaseTypes.h"
#include "Array.h"

namespace Qi
{

template<class T>
class TightlyPackedArray
{
	public:

		TightlyPackedArray();
		~TightlyPackedArray();
		TightlyPackedArray(const TightlyPackedArray &other);
		TightlyPackedArray &operator=(const TightlyPackedArray &other);
		inline T &operator[](int index) const;

		///
		/// Set the size of the array. Should only be called once during initialization.
		///
		/// @param size Size (in elements) to set the array.
		/// @return Status.
		///
		inline Result SetSize(uint32 size);

		///
		/// Free the internal data. SetSize() may be called again after a call
		/// to this function.
		///
		inline void Clear();

		///
		/// Get the total number of valid handles being maintained in the array.
		///
		/// @return Number of valid handles.
		///
		inline uint32 GetValidNumHandles() const;

		///
		/// Handle to use when querying this container. A handle is always guaranteed to be unique
		/// and never change until being released back to the container.
		///
		typedef uint32 Handle;

		///
		/// Get a new handle from the container. This handle is guaranteed to be unique.
		///
		/// @return Unique handle.
		///
		inline Handle AquireHandle();

		///
		/// Give a handle back to the system. The object that links with this handle will no longer
		/// be valid. Note that this function will perform data packing to ensure that the underlying
		/// data isn't fragmented.
		///
		/// @param handle Handle to release.
		///
		inline void ReleaseHandle(const Handle &handle);

		///
		/// Get an element from the container using a handle. The handle must be valid.
		///
		/// @param handle Valid handle referencing an object within the container.
		/// @return Reference to the object referenced by the handle.
		///
		inline T &GetElement(const Handle &handle);
		inline const T &GetElement(const Handle &handle) const;

	private:

		///
		/// Record to use for associated a unique index with a specific data element.
		///
		struct Record
		{
			Handle uniqueIndex; ///< Index that is unique to only this data element.
			T data;             ///< Data itself
		};

		Array<Record> m_elements; ///< All entities contained within this container. This array is allocated once and handles
		                          ///  to the internal objects are then used to interact with them. All objects are guaranteed
		                          ///  to be tightly packed together.
		Array<Handle> m_indexMap; ///< Map of element handles to indices within 'm_elements'. The index of an element may change
		                          ///  during removal of released handles to handle fragmentation.
		Array<Handle> m_elementIndexFreeList; ///< List of free unique indices for new elements to use.

		uint32 m_numValidElements; ///< Number of live (valid) objects within the 'm_elements' array (objects with valid handles).
		uint32 m_numFreeIndices;   ///< Number of free indices in the freelist.

		static const uint32 INVALID_INDEX = UINT_MAX;
};

} // namespace Qi

#include "TightlyPackedArray.inl"