/*
Font Name Library 1.0.0

Copyright (c) 2009 Wong Shao Voon

The Code Project Open License (CPOL)
http://www.codeproject.com/info/cpol10.aspx
*/

#include "StdAfx.h"
#include "EndianSwap.h"

EndianSwap::EndianSwap(void)
{
}

EndianSwap::~EndianSwap(void)
{
}

void EndianSwap::Switch(USHORT& SrcDest)
{
	unionUshort num;
	num.usNum = SrcDest;

	BYTE temp = num.bytes[0];
	num.bytes[0] = num.bytes[1];
	num.bytes[1] = temp;

	SrcDest = num.usNum; 
}

void EndianSwap::Switch(SHORT& SrcDest)
{
	unionShort num;
	num.nNum = SrcDest;

	BYTE temp = num.bytes[0];
	num.bytes[0] = num.bytes[1];
	num.bytes[1] = temp;

	SrcDest = num.nNum;
}

void EndianSwap::Switch(UINT& SrcDest)
{
	unionUint num;
	num.nNum = SrcDest;

	BYTE temp = num.bytes[0];
	num.bytes[0] = num.bytes[3];
	num.bytes[3] = temp;

	temp = num.bytes[1];
	num.bytes[1] = num.bytes[2];
	num.bytes[2] = temp;

	SrcDest = num.nNum; 
}

void EndianSwap::Switch(INT& SrcDest)
{
	unionInt num;
	num.nNum = SrcDest;

	BYTE temp = num.bytes[0];
	num.bytes[0] = num.bytes[3];
	num.bytes[3] = temp;

	temp = num.bytes[1];
	num.bytes[1] = num.bytes[2];
	num.bytes[2] = temp;

	SrcDest = num.nNum; 
}

void EndianSwap::Switch(ULONG& SrcDest)
{
	unionUlong num;
	num.nNum = SrcDest;

	BYTE temp = num.bytes[0];
	num.bytes[0] = num.bytes[3];
	num.bytes[3] = temp;

	temp = num.bytes[1];
	num.bytes[1] = num.bytes[2];
	num.bytes[2] = temp;

	SrcDest = num.nNum; 
}

void EndianSwap::Switch(LONG& SrcDest)
{
	unionLong num;
	num.nNum = SrcDest;

	BYTE temp = num.bytes[0];
	num.bytes[0] = num.bytes[3];
	num.bytes[3] = temp;

	temp = num.bytes[1];
	num.bytes[1] = num.bytes[2];
	num.bytes[2] = temp;

	SrcDest = num.nNum; 
}

void EndianSwap::Switch(WCHAR* SrcDest, size_t Size)
{
	for(size_t i=0; i<Size; ++i)
		Switch((USHORT&)SrcDest[i]);
}
