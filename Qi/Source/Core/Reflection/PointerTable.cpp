//
//  PointerTable.cpp
//  Qi Game Engine
//
//  Created by Cody White on 4/23/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "PointerTable.h"
#include "ReflectionDataManager.h"
#include "../Defines.h"
#include <istream>

namespace Qi
{

const std::string PointerTable::m_tableHeader = "**PointerTableBegin**";
const std::string PointerTable::m_tableFooter = "**PointerTableEnd**";

PointerTable::PointerTable() :
	m_updateTable(true)
{

}

PointerTable::~PointerTable()
{

}

PointerTable::TableIndex PointerTable::AddPointer(ReflectedVariable pointer, bool needsSerialization)
{
	if (m_updateTable)
	{
		PointerAddress address = reinterpret_cast<PointerAddress>(pointer.GetInstanceData());
		LookupTable::iterator iter = m_lookupTable.find(address);
		if (iter != m_lookupTable.end())
		{
			// This pointer already exists in the table, return its index.
			TableIndex index = iter->second;

			// If this pointer already exists and also wants to be serialized, update its
			// serialization status. There may be the case where the pointer does not need
			// to be serialized anymore as what it points to has already been serialized.
			if (m_dataTable[index].second == true)
			{
				m_dataTable[index].second = needsSerialization;
			}

			return index;
		}

		TableIndex index = m_dataTable.size();
		TableRecord record(pointer, needsSerialization);
		m_dataTable.push_back(record);

		m_lookupTable[address] = index;
		return index;
	}

	return 0;
}

void PointerTable::AddPointer(ReflectedVariable pointer, PointerTable::TableIndex index)
{
	QI_ASSERT(index >= 0 && index < m_dataTable.size());

	m_dataTable[index].first = pointer;

	PointerAddress address = reinterpret_cast<PointerAddress>(pointer.GetInstanceData());
	m_lookupTable[address] = index;
}

ReflectedVariable PointerTable::GetPointer(TableIndex index)
{
	QI_ASSERT(index >= 0 && index < m_dataTable.size());
	return m_dataTable[index].first;
}

void PointerTable::Serialize(std::ostream &stream)
{
	stream << m_tableHeader << std::endl;
	stream << m_dataTable.size() << std::endl;

	// Disable writes to the table, we don't want the serialization code trying to add new entries to the
	// table that don't need to be there.
	m_updateTable = false;

	for (size_t ii = 0; ii < m_dataTable.size(); ++ii)
	{
		if (m_dataTable[ii].second) // Does this data need to be serialized?
		{
			// Write out the index in the table for this pointer.
			stream << ii << std::endl;

			const ReflectedVariable *tableVariable = &(m_dataTable[ii].first);

			void *pointerData = &(*(tableVariable->GetValue<char *>()));
			const ReflectionData *reflectionData = tableVariable->GetReflectionData();

			ReflectedVariable variable(tableVariable->GetReflectionData(), pointerData);
			reflectionData->Serialize(&variable, stream, *this);
		}
	}
	stream << m_tableFooter << std::endl;

	m_updateTable = true;
}

void PointerTable::Deserialize(std::istream &stream)
{
	std::string streamInput;

	// First read in the table header.
	stream >> streamInput;
	QI_ASSERT(streamInput == m_tableHeader);

	size_t tableSize = 0;
	stream >> tableSize;
	QI_ASSERT(tableSize >= 0);

	// Resize the data table to get ready for deserialization.
	m_dataTable.resize(tableSize);

	ReflectionDataManager &manager = ReflectionDataManager::GetInstance();

	if (tableSize > 0)
	{
		// Read the entire table, deserializing all data.
		while (streamInput != m_tableFooter)
		{
			TableIndex index;
			stream >> index;
			QI_ASSERT(index >= 0 && index < tableSize);

			// Read in the type.
			stream >> streamInput;

			const ReflectionData *reflectionData = manager.GetReflectionData(streamInput);
			char *instanceData = new char[reflectionData->GetSize()];
			ReflectedVariable variable(reflectionData, static_cast<void *>(instanceData));

			reflectionData->Deserialize(&variable, stream, *this);

			m_dataTable[index].first = variable;
		}
	}
}

} // namespace Qi