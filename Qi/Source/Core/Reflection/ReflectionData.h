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
#include <list>

///
/// Classes which contain reflected data members and
/// classes.
///

namespace Qi
{

// Foward declarations.
class ReflectedMember;
class ReflectedVariable;
class PointerTable;

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
        ///
        void Init(const std::string &name, size_t size);
        
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
		/// Declare the parent type to this type (for inheritance).
		///
		/// @param parent Parent object to this type.
		///
		void DeclareParent(const ReflectionData *parent);

		///
		/// Does this type have a parent (inheritance) or not?
		///
		/// @return If true, this type has a parent.
		///
		bool HasParent() const;
        
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
		/// Storage list for reflected members of this type.
		///
		typedef std::list<ReflectedMember *> Members;

        ///
        /// Get access to the members of this type. If members exist
        /// the first one is returned.
        ///
        /// @return List of members. If the list is empty then there are no members.
        ///
        const Members &GetMembers() const;
        
        ///
        /// Print the members of a class to the debug console.
        ///
        void PrintMembers() const;
        
        ///
        /// Serialize the reflected variable to the stream.
        ///
        /// @param variable Reflected variable to serialize.
        /// @param stream Output stream to serialize to.
		/// @param pointerTable Table to write to when coming across pointer types.
		/// @param padding Padding to apply to the output (in terms of tabs).
        ///
        void Serialize(const ReflectedVariable *variable, std::ostream &stream, PointerTable &pointerTable, uint32 padding = 0) const;

		///
		/// Deserialize the reflected variable from the stream.
		///
		/// @param variable Reflected variable to deserialize.
		/// @param stream Input stream to serialize from.
		/// @param pointerTable Table to read from when coming across pointer types.
		///
		void Deserialize(ReflectedVariable *variable, std::istream &stream, PointerTable &pointerTable, std::vector<std::pair<size_t, ReflectedVariable> > &pointerFixups) const;
        
		typedef void(*SerializeFunction)(const ReflectedVariable *variable, std::ostream &stream, PointerTable &pointerTable);
		typedef void(*DeserializeFunction)(ReflectedVariable *variable, std::istream &stream, PointerTable &pointerTable);
    
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
        
		Members                m_members;    ///< Members contained in this type.
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
		/// @param size Size of this member (in bytes). In the case of pointers, this size is the size of the base type of the pointer, not the pointer itself.
		/// @param isPointer If true, this variable is a pointer.
        /// @param reflectionData Reflected data for this member variable. Could be a POD type
        ///                       or another class/struct.
        ///
		explicit ReflectedMember(const std::string &name, size_t offset, size_t size, bool isPointer, const ReflectionData *reflectionData);
    
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
        const ReflectionData *GetReflectionData() const;

		///
		/// Get the size of this member variable (in bytes). If this member
		/// variable is an array, the size will be the size of the entire
		/// array.
		///
		/// @return Size of this member variable (in bytes).
		///
		size_t GetSize() const;

		///
		/// Check to see if this member variable represents an array. If so, each element of the array will be
		/// serialized independently.
		///
		/// @return If true, this member variable is an array type.
		///
		bool IsArray() const;

		///
		/// Check to see if this member variable represents pointer. If so, then serialization of this
		/// variable will happen to a separate table.
		///
		/// @return If true, this member variable is a pointer type.
		///
		bool IsPointer() const;
    
    private:
    
        const std::string     m_name;       ///< Name of this variable.
        size_t                m_offset;     ///< Offset (in bytes) from the start of the class for this variable.
		size_t                m_size;       ///< Size of this variable (in bytes).
		bool                  m_isPointer;  ///< If true, this member variable is a pointer to an instance of some other type.
        const ReflectionData *m_data;       ///< Reflected data for this variable.
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
        ReflectionDataCreator(const std::string &name, size_t size)
        {
            Init(name, size);
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
        ///
        static void Init(const std::string &name, size_t size)
        {
            ReflectionData &data = GetInstance();
            data.Init(name, size);
            
			RegisterReflectionData();
            ReflectionDataManager::GetInstance().AddReflectedData(&data);
        }
    
        ///
        /// Add a member to this reflected type.
        ///
        /// @param name Name of the member.
        /// @param offset Offset of the member from the beginning of the class (in bytes).
		/// @param size Size of this member (in bytes). In the case of pointers, this size is the size of the base type of the pointer, not the pointer itself.
		/// @param isPointer If true, this member is a pointer to a different instance of some type.
        /// @param data Reflected data for this member.
        ///
		static void AddMember(const std::string &name, size_t offset, size_t size, bool isPointer, const ReflectionData *data)
        {
            GetInstance().AddMember(new ReflectedMember(name, offset, size, isPointer, data));
        }

		///
		/// Declare the parent type to this type (for inheritance).
		///
		/// @param parent Parent object to this type.
		///
		static void DeclareParent(const ReflectionData *parent)
		{
			GetInstance().DeclareParent(parent);
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


