//
//  PointerTable.h
//  Qi Game Engine
//
//  Created by Cody White on 4/23/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Define a table which will contain the address of all objects to serialize
/// during serialization. The objects are loosely related based on pointers
/// defined in the objects which reference other objects in the table.
///

#include "ReflectedVariable.h"
#include <vector>
#include <unordered_map>

namespace Qi
{

class PointerTable
{
	public:

		PointerTable();
		~PointerTable();

		// This table is not copyable.
		PointerTable(const PointerTable &other) = delete;
		PointerTable &operator=(const PointerTable &other) = delete;

		///
		/// Address used to search for existing pointers in the table.
		///
		typedef size_t PointerAddress;

		///
		/// Indices used to reference specific pointers in the table.
		///
		typedef size_t TableIndex;

		///
		/// Populate the table with the addresses of every child object that comes from the reflected variable.
		/// This table will then contain the relationships between all children under the first entry.
		///
		/// @param reflectedVariable Variable to add to the table. All member variables under this variable will
		///                          be added to the table as well.
		/// @param needsSerialization If true, this variable needs to be serialized manually by the table.
		///
		void Populate(const ReflectedVariable &reflectedVariable, bool needsSerialization);

		///
		/// Get access to a specific pointer in the table by index.
		///
		/// @param index Location of the pointer in the table.
		/// @return The reflection data of the pointer requested.
		///
		ReflectedVariable &GetPointer(TableIndex index);

		///
		/// Get the index in the table for a particular pointer.
		///
		/// @param variable Variable in the table to get the index for.
		/// @return The index of the variable in the table.
		///
		TableIndex GetIndex(const ReflectedVariable &variable) const;

		///
		/// Serialize this pointer table to an output stream.
		///
		/// @param stream The output stream to serialize the pointer table to.
		///
		void Serialize(std::ostream &stream);

		///
		/// Deserialize the table from an input stream. The deserialization process works by first allocating a pointer
		/// table that is large enough to hold references to all of the objects to be deserialized. Then each element in the table
		/// is read from the stream. While reading, if a pointer is encountered, it's index is saved for later patching after
		/// the entire table has been read in.
		///
		/// @param stream The input stream containing a serialized table for reading.
		///
		void Deserialize(std::istream &stream);

		///
		/// Add a pointer to the patch table. Any pointers added here will have their instance data set to
		/// the corresponding table index data after deserialization of the table.
		///
		/// @param index Index location in the table to patch this pointer to.
		/// @param pointer Pointer to patch.
		///
		void AddPatchPointer(PointerTable::TableIndex index, ReflectedVariable &pointer);

	private:

		///
		/// Add a pointer to the table. If this pointer already exists in the table,
		/// the existing index is returned.
		///
		/// @param pointer Pointer to store in the table as reflection data.
		/// @param needsSerialization If true, this variable will be serialized with the pointer table.
		/// @return Index in the table for this pointer.
		///
		TableIndex AddPointer(const ReflectedVariable &pointer, bool needsSerialization);

		///
		/// Determine if a specific instance is located in the pointer table.
		///
		/// @return If true, the table already includes this variable.
		///
		bool HasPointer(const ReflectedVariable &variable) const;

		///
		/// Underlying pointer data stored in a linear table.
		///
		typedef bool NeedsSerializationFlag;
		typedef std::pair<ReflectedVariable, NeedsSerializationFlag> TableRecord;
		typedef std::vector<TableRecord> Pointers;

		///
		/// Lookup table to map pointer addresses to indices in the 'Pointers' table.
		/// First argument is the pointer address, second is the table index per type that sits
		/// at that address. Assume an object looks like this:
		/// class Foo { int x; };
		/// Foo f;
		/// In this case, &f == &x because of how memory is laid out by the compiler. Therefore, two different
		/// objects can report the same address, hence the list of pairs in the lookup table.
		///
		typedef std::pair<const ReflectionData *, TableIndex> Instance;
		typedef std::list<Instance> Objects;
		typedef std::unordered_map<PointerAddress, Objects> LookupTable;

		Pointers m_dataTable;      ///< Pointer data stored linearly by index.
		LookupTable m_lookupTable; ///< Lookup table storing correlations between pointer addresses and table indices.

		typedef std::vector<std::pair<PointerTable::TableIndex, ReflectedVariable> > PointerPatchTable;
		PointerPatchTable m_pointersToPatch; ///< Pointers to patch-up after deserializing the entire table.
};

} // namespace Qi


