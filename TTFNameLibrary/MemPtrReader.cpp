/*
Font Name Library 1.0.0

Copyright (c) 2009 Wong Shao Voon

The Code Project Open License (CPOL)
http://www.codeproject.com/info/cpol10.aspx
*/

#include "StdAfx.h"
#include "MemPtrReader.h"
#include <cstdio>

MemPtrReader::MemPtrReader(BYTE *pOrigin, size_t size, size_t offset)
{
	m_pOrigin = pOrigin;
	m_ptr = pOrigin;
	m_pSteps=0;
	m_size = size;
	m_nOffset = offset;
}

MemPtrReader::~MemPtrReader(void)
{
}

int MemPtrReader::Seek( long offset, int origin )
{
	if(origin==SEEK_SET)
	{
		m_ptr = m_pOrigin-m_nOffset;
		m_ptr = m_ptr + offset;
		return 0;
	}
	else if(origin==SEEK_END)
	{
		m_ptr = m_pOrigin-m_nOffset+m_size;
		m_ptr = m_ptr + offset;
		return 0;
	}
	else if(origin==SEEK_CUR)
	{
		m_ptr = m_ptr + offset;
		return 0;
	}

	return 1;
}

size_t MemPtrReader::Read( void *buffer, size_t size, size_t count )
{
	BYTE* buffer2 = (BYTE*)(buffer);
	for(size_t i=0; i<count; ++i)
	{
		memcpy(buffer2,m_ptr,size);
		buffer2 = buffer2 + size;
		m_ptr = m_ptr + size;
	}

	return size * count;
}
