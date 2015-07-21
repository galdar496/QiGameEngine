//
//  ReflectionDataManager.h
//  Qi Game Engine
//
//  Created by Cody White on 3/30/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Hold all defitions of reflected types for later retrieval.
///

#include "../Defines.h"
#include "../BaseTypes.h"
#include <unordered_map>
#include <string>

namespace Qi
{

// Forward declarations.
class ReflectionData;

class ReflectionDataManager
{
    public:
    
        ///
        /// This class is static and can only be accessed through this instance.
        ///
        static ReflectionDataManager &GetInstance();
    
        ///
        /// Add a reflected type to the manager.
        ///
        /// @param data Data for the reflected type.
        ///
        void AddReflectedData(const ReflectionData *data);
    
        ///
        /// Get a reflected type based on the type name.
        ///
        /// @param name Name of the reflected type to lookup. Type must be declared with QI_DECLARE_REFLECTION_CLASS or QI_DECLARE_REFLECTION_POD.
        /// @return The reflection data, or nullptr if this type was not found.
        ///
        const ReflectionData *GetReflectionData(const std::string &name);
    
        ///
        /// Same as 'GetReflectionData' except the hashed name id can be used directly.
        ///
        /// @param hashedName The result of hashing the name of a type after calling Qi::StringHash().
        /// @return The reflection data, or nullptr if this type was not found.
        ///
        const ReflectionData *GetReflectionData(uint32 hashedName);

		///
		/// Get all type names stored in the reflection system.
		///
		/// @param typenames List of typenames to populate for the calling function.
		///
		typedef std::vector<std::string> Typenames;
		void GetAllTypenames(Typenames &typenames) const;

    
    private:
    
        // This class is a singleton and cannot be copied.
        ReflectionDataManager();
        ~ReflectionDataManager();
        ReflectionDataManager(const ReflectionDataManager &other) = delete;
        ReflectionDataManager &operator=(const ReflectionDataManager &other) = delete;
    
        typedef std::unordered_map<uint32, const ReflectionData *> ReflectionTable;
        ReflectionTable m_reflectedData; ///< All reflected objects stored by a string key. Use Qi::StringHash() to generate a key for to classname to lookup.
};

} // namespace Qi
