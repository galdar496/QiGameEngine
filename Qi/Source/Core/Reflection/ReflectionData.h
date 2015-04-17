//
//  ReflectionData.h
//  Qi Game Engine
//
//  Created by Cody White on 3/23/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include "ReflectionDataManager.h"
#include <ostream>
#include <string>

///
/// Classes which contain reflected data members and
/// classes.
///

namespace Qi
{

// Foward declarations.
class ReflectedMember;
class ReflectedVariable;

class ReflectionData
{
    public:
        
        ///
        /// Default constructor.
        ///
        /// @param name Name of this type.
        /// @param size Size of this type (in bytes).
        ///
        ReflectionData(const std::string name = "", size_t size = 0);
        
        ~ReflectionData();
        
        ///
        /// Initialize this object for use.
        ///
        /// @param name Name of this type.
        /// @param size Size of this type (in bytes).
		/// @param parentTypeName Name of this type's parent (inherited types only).
        ///
        void Init(const std::string &name, size_t size, const std::string &parentTypeName);
        
        ///
        /// Get the name of this type.
        ///
        /// @return Name of the type.
        ///
        const std::string &GetName() const;
        
        ///
        /// Get the size of this type (in bytes).
        ///
        /// @return Size of this type (in bytes).
        ///
        size_t GetSize() const;
        
        ///
        /// Add a member to this type.
        ///
        /// @param member New member info to add to this type.
        ///
        void AddMember(const ReflectedMember *member);
        
        ///
        /// Determine if this type has members (a class/struct) or
        /// doesn't (a POD type).
        ///
        /// @return If true, this type has members.
        ///
        bool HasDataMembers() const;


		///
		/// Find a specific member by name for this object. 
		///
		/// @param name Name of the member.
		/// @return A pointer to the found member, nullptr if not found.
		/// 
		const ReflectedMember *GetMember(const std::string &name) const;
        
        ///
        /// Get access to the members of this type. If members exist
        /// the first one is returned.
        ///
        /// @return First member if members exist, null otherwise.
        ///
        const ReflectedMember *GetMembers() const;
        
        ///
        /// Print the members of a class to the debug console.
        ///
        void PrintMembers() const;
        
        ///
        /// Serialize the reflected variable to the stream.
        ///
        /// @param variable Reflected variable to serialize.
        /// @param stream Output stream to serialize to.
        ///
        void Serialize(const ReflectedVariable *variable, std::ostream &stream, uint32 padding = 0) const;

		///
		/// Deserialize the reflected variable from the stream.
		///
		/// @param variable Reflected variable to deserialize.
		/// @param stream Input stream to serialize from.
		///
		void Deserialize(ReflectedVariable *variable, std::istream &stream) const;
        
        typedef void (*SerializeFunction)(const ReflectedVariable *variable, std::ostream &stream);
		typedef void (*DeserializeFunction)(ReflectedVariable *variable, std::istream &stream);
    
        ///
        /// Set the serialization function. Some types (such as the primitive types defined in ReflectionPrimitiveTypes.h) know
        /// how to serialize themselves. This provides a function callback to use for serialization of known types.
        ///
        /// @param function Function to use for serialization of this type.
        ///
        void SetSerializeFunction(SerializeFunction function = nullptr);

		///
		/// Set the deserialization function. Some types (such as the primitive types defined in ReflectionPrimitiveTypes.h) know
		/// how to deserialize themselves. This provides a function callback to use for deserialization of known types.
		///
		/// @param function Function to use for deserialization of this type.
		///
		void SetDeserializeFunction(DeserializeFunction function = nullptr);
        
    private:
        
        ReflectedMember       *m_members;    ///< Members contained in this type.
        ReflectedMember       *m_lastMember; ///< The end of the members list.
        std::string            m_name;       ///< Name of this type.
        size_t                 m_size;       ///< Size of this type in bytes.
		const ReflectionData  *m_parent;     ///< Parent object to this type (only populated if this is an inherited type).
        
        SerializeFunction   m_serializeFunction;   ///< Serialization function to use if this type is a primitive type defined in ReflectionPrimitiveTypes.h
		DeserializeFunction m_deserializeFunction; ///< Deserialization function to use if this type is a primitive type defined in ReflectionPrimitiveTypes.h
};

class ReflectedMember
{
    public:
    
        ///
        /// Default constructor.
        ///
        /// @param name Name of the variable.
        /// @param offset Offset (in bytes) from the beginning of the class data.
		/// @param size Size of this member (in bytes).
        /// @param reflectionData Reflected data for this member variable. Could be a POD type
        ///                       or another class/struct.
        ///
        explicit ReflectedMember(const std::string &name, size_t offset, size_t size, const ReflectionData *reflectionData);
    
        ~ReflectedMember();
    
        ///
        /// Get the name of the variable.
        ///
        /// @return Name of the member variable.
        ///
        const std::string &GetName() const;
    
        ///
        /// Get the offset of the variable relative to the beginning
        /// of the class data (in bytes).
        ///
        /// @return Offset of the variable.
        ///
        size_t GetOffset() const;
    
        ///
        /// Get the reflection data for this member variable.
        ///
        /// @return Reflection data for this variable.
        ///
        const ReflectionData *GetData() const;

		///
		/// Get the size of this member variable (in bytes). If this member
		/// variable is an array, the size will be the size of the entire
		/// array.
		///
		/// @return Size of this member variable (in bytes).
		///
		size_t GetSize() const;
    
        ///
        /// Get the next member variable for the class.
        ///
        /// @return Next member variable. If null, then there are no
        ///         more member variables for this class.
        ///
        ReflectedMember *const& GetNextMember() const;
        ReflectedMember *& GetNextMember();
    
        ///
        /// Check to see if this type has any member variables under it (is a class).
        ///
        /// @return If true, this type has members.
        ///
        bool HasMembers() const;

		///
		/// Check to see if this member variable represents an array. If so, each element of the array will be
		/// serialized independently.
		///
		/// @return If true, this member variable is an array type.
		///
		bool IsArray() const;
    
    private:
    
        const std::string     m_name;       ///< Name of this variable.
        size_t                m_offset;     ///< Offset (in bytes) from the start of the class for this variable.
		size_t                m_size;       ///< Size of this variable (in bytes).
        const ReflectionData *m_data;       ///< Reflected data for this variable.
        ReflectedMember      *m_nextMember; ///< Pointer to the next member variable for the reflected class.
};

template<class T>
class ReflectionDataCreator
{
    public:
    
        ///
        /// Create a new instance of reflection data.
        ///
        /// @param name Name of the type.
        /// @param size Size of the type (in bytes).
        ///
        ReflectionDataCreator(const std::string &name, size_t size, const std::string &parentTypeName)
        {
            Init(name, size, parentTypeName);
        }
    
        ///
        /// This class is static per-type.
        ///
        static ReflectionData &GetInstance()
        {
            static ReflectionData instance;
            return instance;
        }
    
        ///
        /// Initialize this type.
        ///
        /// @param name Name of the type.
        /// @param size Size of the type in bytes.
		/// @param parentTypeName Name of this type's parent (inherited types only).
        ///
        static void Init(const std::string &name, size_t size, const std::string &parentTypeName)
        {
            ReflectionData &data = GetInstance();
            data.Init(name, size, parentTypeName);
            
			RegisterReflectionData();
            ReflectionDataManager::GetInstance().AddReflectedData(&data);
        }
    
        ///
        /// Add a member to this reflected type.
        ///
        /// @param name Name of the member.
        /// @param offset Offset of the member from the beginning of the class (in bytes).
		/// @param size Size of this memebr (in bytes).
        /// @param data Reflected data for this member.
        ///
        static void AddMember(const std::string &name, size_t offset, size_t size, const ReflectionData *data)
        {
            GetInstance().AddMember(new ReflectedMember(name, offset, size, data));
        }
    
        ///
        /// Perform a null cast which is specific to this type.
        ///
        static T *NullCast()
        {
            return reinterpret_cast<T *>(nullptr);
        }
    
        ///
        /// Automatically implemented via macros defined in Reflection.h per type.
        /// This function is used to populate the reflected member variables per-class.
        ///
		static void RegisterReflectionData();
};

} // namespace Qi


