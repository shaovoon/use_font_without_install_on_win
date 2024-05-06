/*
Font Name Library 1.0.0

Copyright (c) 2009 Wong Shao Voon

The Code Project Open License (CPOL)
http://www.codeproject.com/info/cpol10.aspx
*/

#pragma once

class MemPtrReader
{
public:
	//! constructor
	//!
	//! @param[in]	pOrigin is current pointer
	//! @param[in]	size is size of the buffer
	//! @param[in]	offset is the offset of pOrigin from the starting byte
	MemPtrReader(BYTE *pOrigin, size_t size, size_t offset);
	//! destructor
	~MemPtrReader(void);

	//! Seek to offset
	//!
	//! @param[in]	offset is the offset to move from origin
	//! @param[in]	origin can be one of these SEEK_SET, SEEK_END or SEEK_CUR (Read the fseek docs)
	//! @return zero for success
	int Seek( long offset, int origin );

	//! Read the number of bytes from buffer
	//!
	//! @param[in]	buffer is destination buffer
	//! @param[in]	size of the struct to read to buffer
	//! @param[in]	count is the number of size to read
	//! @return size of the data bytes read
	size_t Read( void *buffer, size_t size, size_t count );
	
	//! Get the current buffer ptr.
	BYTE* GetPtr() { return m_ptr; };
	
	//! Get the size of the buffer
	size_t Size() { return m_size; };

private:
	//! Original pointer
	BYTE *m_pOrigin;
	//! Current pointer
	BYTE *m_ptr;
	//! Number of steps to move
	size_t m_pSteps;
	//! Size of the buffer
	size_t m_size;
	//! Offset of the m_pOrigin from the starting byte
	size_t m_nOffset;
};
