//
//  RefCountedObject.h
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include "../Reflection/Reflection.h"
#include "../Defines.h"
#include <atomic>

///
/// Base class to add automatic ref-counting to objects. Any objects that reach a 
/// refcount of 0 will be automatically deleted. Upon creation of this object, the 
/// reference count will be set to 1.
///

namespace Qi
{

class RefCountedObject
{
	public:

		QI_DECLARE_REFLECTED_CLASS(RefCountedObject);

		RefCountedObject();
		RefCountedObject(const RefCountedObject &other);
		RefCountedObject &operator=(const RefCountedObject &other);

		///
		/// Add a reference to this object (increment the reference count).
		///
		void AddReference();

		///
		/// Release a reference from this object (decrement the reference count). If
		/// the reference count gets to 0, this object will be deleted.
		///
		void ReleaseReference();

		///
		/// Get the current reference count for this object. Note that while this function is 
		/// threadsafe, the current reference count may have changed by the time it is read
		/// by the calling function.
		///
		uint32 GetReferenceCount() const;

	protected:
    
        ///
        /// Destructor is under the 'protected' section of the class in order to prevent
        /// accidental deletion of ref-counted objects.
        ///
        virtual ~RefCountedObject();

		///
		/// Delete this object. Called by RemoveReference() if the object's reference count gets
		/// to 0. This function is virtual so that any deriving type may override how an object is
		/// deleted.
		///
		virtual void DeleteThisObject();

	private:

		std::atomic<uint32> m_refCount; ///< Current reference count for this object.
};

} // namespace Qi

