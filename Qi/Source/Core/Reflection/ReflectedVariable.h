//
//  ReflectedVariable.h
//  Qi Game Engine
//
//  Created by Cody White on 3/30/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Holds a specific instance of a reflected type along with the actual data of
/// a specific instance of the type.
///

#include <ostream>

namespace Qi
{

// Forward declarations.
class ReflectionData;

class ReflectedVariable
{
    public:
    
        // Constructors/destructor.
        ReflectedVariable();
        ReflectedVariable(const ReflectedVariable &other);
		ReflectedVariable &operator=(const ReflectedVariable &other);
        ~ReflectedVariable();
    
        ///
        /// Construct this object from any reflected type.
        ///
        template<typename T>
        ReflectedVariable(const T &value);
    
        ///
        /// Construct this object from reflected data and the data of an actual instance of the object.
        ///
        /// @param reflectedData Pointer to the reflected data for the type of instance data passed in.
        /// @param instanceData Data for an actual instance of one of the reflected types.
        ///
        ReflectedVariable(const ReflectionData *reflectionData, void *instanceData);
    
        ///
        /// Get the reflected data contained within this reflected variable.
        ///
        /// @return The reflected data.
        ///
        const ReflectionData *GetReflectionData() const;
    
        ///
        /// Get the instance data within this reflected variable.
        ///
        /// @return The instance data.
        ///
        const void *GetInstanceData() const;

		///
		/// Set the instance data for this variable.
		///
		/// @param data Instance data to use.
		///
		void SetInstanceData(const void *data);
    
        ///
        /// Get the instance data cast to a specific type.
        ///
        template<typename T>
        const T &GetValue() const;
		template<typename T>
		T &GetValue();
    
        ///
        /// Serialize this variable to the specified stream.
        ///
        /// @param stream Output stream to write the serialized data to.
        ///
        void Serialize(std::ostream &stream) const;

		///
		/// Deserialize this variable to the specified stream.
		///
		/// @param stream Input stream to read the serialized data from.
		///
		void Deserialize(std::istream &stream);
    
    private:
    
        const ReflectionData *m_reflectionData; ///< Reflection data for this type.
        void				 *m_instanceData;   ///< Data for actual instance of the reflected type.
};


} // namespace Qi

#include "ReflectedVariable.inl"
