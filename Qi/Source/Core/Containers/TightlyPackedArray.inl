//
//  TightlyPackedArray.inl
//  Qi Game Engine
//
//  Created by Cody White on 3/1/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

namespace Qi
{

template<class T>
TightlyPackedArray<T>::TightlyPackedArray() :
	m_numFreeIndices(0),
	m_numValidElements(0)
{
}

template<class T>
TightlyPackedArray<T>::~TightlyPackedArray()
{
	m_elements.Clear();
}

template<class T>
TightlyPackedArray<T>::TightlyPackedArray(const TightlyPackedArray<T> &other) :
	m_elements(other.m_elements)
{
}

template<class T>
TightlyPackedArray<T> &TightlyPackedArray<T>::operator=(const TightlyPackedArray<T> &other)
{
	if (this != &other)
	{
		m_elements = other.m_elements;
	}

	return *this;
}

template<class T>
T &TightlyPackedArray<T>::operator[](int index) const
{
	QI_ASSERT(index < (int)m_numValidElements);
	return m_elements[index].data;
}

template<class T>
Result TightlyPackedArray<T>::SetSize(uint32 size)
{
	QI_ASSERT(m_elements.GetSize() == 0);

	Result result = m_elements.Resize(size);
	if (result.IsValid())
	{
		result = m_indexMap.Resize(size);
		if (result.IsValid())
		{
			result = m_elementIndexFreeList.Resize(size);

			// Set the ID map values to an invalid index.
			for (uint32 ii = 0; ii < size; ++ii)
			{
				m_indexMap[ii] = INVALID_INDEX;
				m_elementIndexFreeList[ii] = size - ii - 1; // Insert backwards for easier tracking later.
			}

			m_numFreeIndices = size;
		}
	}

	return result;
}

template<class T>
void TightlyPackedArray<T>::Clear()
{
	m_elementIndexFreeList.Clear();
	m_indexMap.Clear();
	m_elements.Clear();

	m_numFreeIndices   = 0;
	m_numValidElements = 0;
}

template<class T>
uint32 TightlyPackedArray<T>::GetValidNumHandles() const
{
	return m_numValidElements;
}

template<class T>
typename TightlyPackedArray<T>::Handle TightlyPackedArray<T>::AquireHandle()
{
	QI_ASSERT(m_numFreeIndices > 0);

	Handle handle = m_elementIndexFreeList[m_numFreeIndices - 1];
	--m_numFreeIndices;

	m_indexMap[handle] = m_numValidElements;
	m_elements[m_numValidElements].uniqueIndex = handle;

	++m_numValidElements;
	return handle;
}

template<class T>
void TightlyPackedArray<T>::ReleaseHandle(const typename TightlyPackedArray<T>::Handle &handle)
{
	QI_ASSERT(handle < m_numValidElements);

	uint32 endElement  = m_numValidElements - 1;
	uint32 mappedIndex = m_indexMap[handle];

	QI_ASSERT(mappedIndex != INVALID_INDEX);

	// Swap the element to be removed with the last element in the list.
	std::swap(m_elements[mappedIndex], m_elements[endElement]);

	// Update the id map so that the newly swapped element knows where it is later on.
	m_indexMap[mappedIndex] = INVALID_INDEX;
	m_indexMap[m_elements[mappedIndex].uniqueIndex] = mappedIndex;

	--m_numValidElements;
	m_elementIndexFreeList[m_numFreeIndices] = handle;
	++m_numFreeIndices;
}

template<class T>
T &TightlyPackedArray<T>::GetElement(const Handle &handle)
{
	Handle mappedIndex = m_indexMap[handle];
	QI_ASSERT(mappedIndex != INVALID_INDEX);

	return m_elements[mappedIndex].data;
}

template<class T>
const T &TightlyPackedArray<T>::GetElement(const Handle &handle) const
{
	Handle mappedIndex = m_indexMap[handle];
	QI_ASSERT(mappedIndex != INVALID_INDEX);

	return m_elements[mappedIndex].data;
}

} // namespace Qi
