/*
Font Name Library 1.0.0

Copyright (c) 2009 Wong Shao Voon

The Code Project Open License (CPOL)
http://www.codeproject.com/info/cpol10.aspx
*/

#include "StdAfx.h"
#include "TTC.h"
#include "MemPtrReader.h"

TTC::TTC(void)
{
	m_pFile = NULL;
}

TTC::~TTC(void)
{
	Clear();
}

bool TTC::Parse(const std::wstring& szFilePath)
{
	Clear();
	m_szFilePath = szFilePath;
	m_pFile = _wfopen(szFilePath.c_str(), L"rb");

	if(NULL==m_pFile)
		return false;

	return ParseFont();
}

bool TTC::Parse(BYTE* pFile, size_t size)
{
	return Parse(pFile, size, 0);
}

bool TTC::Parse(BYTE* pFile, size_t size, size_t offset)
{
	Clear();
	if(pFile == NULL)
		return false;

	return ParseFontMem( pFile, size, offset);
}

bool TTC::ParseFontMem(BYTE* pFile, size_t size, size_t offset)
{
	stTTCHeader TTCHeader;
	memset(&TTCHeader, 0, sizeof(TTCHeader));
	MemPtrReader memPtrReader(pFile, size, offset);
	memPtrReader.Read(&TTCHeader, sizeof(TTCHeader), 1);
	EndianSwap::Switch(TTCHeader.numFonts);

	std::string strTag = "";
	strTag += TTCHeader.tag[0];
	strTag += TTCHeader.tag[1];
	strTag += TTCHeader.tag[2];
	strTag += TTCHeader.tag[3];

	if(strTag!="ttcf")
		return false;

	std::vector<UINT> vecOffset;
	for(size_t i=0; i<TTCHeader.numFonts; ++i)
	{
		UINT nOffset=0;
		memset(&nOffset, 0, sizeof(nOffset));
		memPtrReader.Read(&nOffset, sizeof(nOffset), 1);
		EndianSwap::Switch(nOffset);
		vecOffset.push_back(nOffset);
	}

	for(size_t i=0; i<vecOffset.size(); ++i)
	{
		TTF* pTtf = new TTF();
		memPtrReader.Seek(vecOffset.at(i),SEEK_SET);
		bool b = pTtf->Parse(memPtrReader.GetPtr(), memPtrReader.Size(), vecOffset.at(i));

		if(b)
		{
			m_vecTtfPtr.push_back(pTtf);
		}
		else
		{
			delete pTtf;
			pTtf = NULL;
		}
	}

	if(TTCHeader.numFonts>0)
		return true;

	return false;
}

bool TTC::ParseFont()
{
	stTTCHeader TTCHeader;
	memset(&TTCHeader, 0, sizeof(TTCHeader));
	fread(&TTCHeader, sizeof(TTCHeader), 1, m_pFile);
	EndianSwap::Switch(TTCHeader.numFonts);

	std::string strTag = "";
	strTag += TTCHeader.tag[0];
	strTag += TTCHeader.tag[1];
	strTag += TTCHeader.tag[2];
	strTag += TTCHeader.tag[3];

	if(strTag!="ttcf")
		return false;

	std::vector<UINT> vecOffset;
	for(size_t i=0; i<TTCHeader.numFonts; ++i)
	{
		UINT nOffset=0;
		memset(&nOffset, 0, sizeof(nOffset));
		fread(&nOffset, sizeof(nOffset), 1, m_pFile);
		EndianSwap::Switch(nOffset);
		vecOffset.push_back(nOffset);
	}

	for(size_t i=0; i<vecOffset.size(); ++i)
	{
		TTF* pTtf = new TTF();
		fseek(m_pFile,vecOffset.at(i),SEEK_SET);
		bool b = pTtf->Parse(m_pFile, vecOffset.at(i));

		if(b)
		{
			m_vecTtfPtr.push_back(pTtf);
		}
		else
		{
			delete pTtf;
			pTtf = NULL;
		}
	}

	if(TTCHeader.numFonts>0)
		return true;

	return false;
}

void TTC::Clear()
{
	if(m_pFile)
		fclose(m_pFile);

	m_pFile = NULL;
	m_szFilePath = L"";
	for(size_t i=0; i<m_vecTtfPtr.size(); ++i)
	{
		if(m_vecTtfPtr.at(i))
		{
			delete m_vecTtfPtr.at(i);
			m_vecTtfPtr.at(i) = NULL;
		}
	}
	m_vecTtfPtr.clear();
}

std::wstring TTC::NarrowToWide(const std::string& szNarrow)
{
	return TTF::NarrowToWide(szNarrow);
}

std::string TTC::WideToNarrow(const std::wstring& szWide)
{
	return TTF::WideToNarrow(szWide);
}
