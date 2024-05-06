/*
Font Name Library 1.0.0

Copyright (c) 2009 Wong Shao Voon

The Code Project Open License (CPOL)
http://www.codeproject.com/info/cpol10.aspx
*/

#pragma once

class EndianSwap
{
public:
	//! default constructor
	EndianSwap(void);
	//! destructor
	~EndianSwap(void);

	union unionUshort
	{
		USHORT usNum;
		BYTE bytes[2];
	};

	union unionShort
	{
		SHORT nNum;
		BYTE bytes[2];
	};

	union unionUint
	{
		UINT nNum;
		BYTE bytes[4];
	};

	union unionInt
	{
		INT nNum;
		BYTE bytes[4];
	};

	union unionUlong
	{
		ULONG nNum;
		BYTE bytes[4];
	};

	union unionLong
	{
		LONG nNum;
		BYTE bytes[4];
	};

	union unionString
	{
		CHAR charr[1024];
		WCHAR wcharr[512];
	};

	//! Switch the endianness of USHORT
	static void Switch(USHORT& SrcDest);
	//! Switch the endianness of SHORT
	static void Switch(SHORT& SrcDest);
	//! Switch the endianness of UINT
	static void Switch(UINT& SrcDest);
	//! Switch the endianness of INT
	static void Switch(INT& SrcDest);
	//! Switch the endianness of ULONG
	static void Switch(ULONG& SrcDest);
	//! Switch the endianness of LONG
	static void Switch(LONG& SrcDest);
	//! Switch the endianness of WCHAR
	static void Switch(WCHAR* SrcDest, size_t Size);
};
