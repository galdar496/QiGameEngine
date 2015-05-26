//
//  PointerTable.h
//  Qi Game Engine
//
//  Created by Cody White on 4/23/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Define a pointer table to use during serialization/deserialization. This table
/// will be searchable by address and will store the the actual address of the variables
/// to serialize out.
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
		/// Add a pointer to the table. If this pointer already exists in the table,
		/// the existing index is returned.
		///
		/// @param pointer Pointer to store in the table as reflection data.
		/// @param needsSerialization If true, this variable will be serialized with the pointer table.
		/// @return Index in the table for this pointer.
		///
		TableIndex AddPointer(ReflectedVariable pointer, bool needsSerialization);

		///
		/// Add a pointer to a specific index of the table. Warning: table
		/// must be appropriately sized to handle the incoming index.
		///
		/// @param pointer Pointer to store in the table as reflection data.
		/// @param index Index to place the pointer at.
		///
		void AddPointer(ReflectedVariable pointer, TableIndex index);

		///
		/// Get access to a specific pointer in the table by index.
		///
		/// @param index Location of the pointer in the table.
		/// @return The reflection data of the pointer requested.
		///
		ReflectedVariable GetPointer(TableIndex index);

		///
		/// Serialize this pointer table to an output stream.
		///
		/// @param stream The output stream to serialize the pointer table to.
		///
		void Serialize(std::ostream &stream);

		///
		/// Deserialize the table from an input stream.
		///
		/// @param stream The input stream containing a serialized table for reading.
		///
		void Deserialize(std::istream &stream);

	private:

		///
		/// Underlying pointer data stored in a linear table.
		///
		typedef bool NeedsSerializationFlag;
		typedef std::pair<ReflectedVariable, NeedsSerializationFlag> TableRecord;
		typedef std::vector<TableRecord> Pointers;

		///
		/// Lookup table to map pointer addresses to indices in the 'Pointers' table.
		/// First argument is the pointer address, second is the table index.
		///
		typedef std::unordered_map<PointerAddress, TableIndex> LookupTable;

		Pointers m_dataTable;      ///< Pointer data stored linearly by index.
		LookupTable m_lookupTable; ///< Lookup table storing correlations between pointer addresses and table indices.

		bool m_updateTable; ///< If true, writes to the table are enabled.

		static const std::string m_tableHeader; // Text prepended to the start of the table while writing out to denote the start of the table.
		static const std::string m_tableFooter; // Text appended to the end of the table while writing out to denote the end of the table.
};

} // namespace Qi


