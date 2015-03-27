//
//  ReflectionData.h
//  Qi Game Engine
//
//  Created by Cody White on 3/23/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include <string>

///
/// Classes which contain reflected data members and
/// classes.
///

namespace Qi
{

// Foward declarations.
class ReflectedMember;

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
    
private:
    
    ReflectedMember *m_members;    ///< Members contained in this type.
    ReflectedMember *m_lastMember; ///< The end of the members list.
    std::string      m_name;       ///< Name of this type.
    size_t           m_size;       ///< Size of this type in bytes.
};

class ReflectedMember
{
    public:
    
        ///
        /// Default constructor.
        ///
        /// @param name Name of the variable.
        /// @param offset Offset (in bytes) from the beginning of the class data.
        /// @param reflectionData Reflected data for this member variable. Could be a POD type
        ///                       or another class/struct.
        ///
        explicit ReflectedMember(const std::string &name, size_t offset, const ReflectionData *reflectionData);
    
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
        /// Get the next member variable for the class.
        ///
        /// @return Next member variable. If null, then there are no
        ///         more member variables for this class.
        ///
        ReflectedMember *const& GetNextMember() const;
        ReflectedMember *& GetNextMember();
    
    private:
    
        const std::string     m_name;       ///< Name of this variable.
        size_t                m_offset;     ///< Offset (in bytes) from the start of the class for this variable.
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
            GetInstance().Init(name, size);
            RegisterReflectedData();
        }
    
        ///
        /// Add a member to this reflected type.
        ///
        /// @param name Name of the member.
        /// @param offset Offset of the member from the beginning of the class (in bytes).
        /// @param data Reflected data for this member.
        ///
        static void AddMember(const std::string &name, size_t offset, const ReflectionData *data)
        {
            GetInstance().AddMember(new ReflectedMember(name, offset, data));
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
        static void RegisterReflectedData();
};

} // namespace Qi


