/*
Font Name Library 1.0.0

Copyright (c) 2009 Wong Shao Voon

The Code Project Open License (CPOL)
http://www.codeproject.com/info/cpol10.aspx
*/


#pragma once
#include <vector>
#include <string>
#include "TTF.h"

class TTC
{
public:
	//! default constructor
	TTC(void);
	//! destructor
	~TTC(void);

	struct stTTCHeader
	{
		CHAR tag[4];
		UINT Version;
		ULONG numFonts;
	};


	//! Parse file for font information
	bool Parse(const std::wstring& szFilePath);

	//! Parse the memory buffer for font information
	//!
	//! @param[in]	pFile is the buffer pointer
	//! @param[in]	size is the size of the buffer
	//! @return true for success
	bool Parse(BYTE* pFile, size_t size);

	//! Parse the memory buffer for font information
	//!
	//! @param[in]	pFile is the buffer pointer
	//! @param[in]	size is the size of the buffer
	//! @param[in]	offset is the offset of pFile from the starting byte
	//! @return true for success
	bool Parse(BYTE* pFile, size_t size, size_t offset);
	
	//! Size of the collection of fonts
	size_t Size() { return m_vecTtfPtr.size(); }

	//! Clear all information
	void Clear();

	std::wstring GetFontName(size_t nIndex) {return m_vecTtfPtr.at(nIndex)->GetFontName();};
	std::wstring GetCopyright(size_t nIndex) {return m_vecTtfPtr.at(nIndex)->GetCopyright();};
	std::wstring GetFontFamilyName(size_t nIndex) {return m_vecTtfPtr.at(nIndex)->GetFontFamilyName();};
	std::wstring GetFontSubFamilyName(size_t nIndex) {return m_vecTtfPtr.at(nIndex)->GetFontSubFamilyName();};
	std::wstring GetFontID(size_t nIndex) {return m_vecTtfPtr.at(nIndex)->GetFontID();};
	std::wstring GetVersion(size_t nIndex) {return m_vecTtfPtr.at(nIndex)->GetVersion();};
	std::wstring GetPostScriptName(size_t nIndex) {return m_vecTtfPtr.at(nIndex)->GetPostScriptName();};
	std::wstring GetTrademark(size_t nIndex) {return m_vecTtfPtr.at(nIndex)->GetTrademark();};

	bool IsBold(size_t nIndex) { return m_vecTtfPtr.at(nIndex)->IsBold(); };
	bool IsItalic(size_t nIndex) { return m_vecTtfPtr.at(nIndex)->IsItalic(); };
	bool IsRegular(size_t nIndex) { return m_vecTtfPtr.at(nIndex)->IsRegular(); };

	//! Converts a wide character string to narrow character string
	static std::string WideToNarrow(const std::wstring& szWide);
	//! Converts a narrow character string to wide character string
	static std::wstring NarrowToWide(const std::string& szNarrow);

private:
	//! Parse the font using the file
	bool ParseFont();
	//! Parse the font in memory
	bool ParseFontMem(BYTE* pFile, size_t size, size_t offset);

	//! File pointer
	FILE* m_pFile;
	//! The font file
	std::wstring m_szFilePath;
	//! Vector of TTF pointers
	std::vector<TTF*> m_vecTtfPtr;

};
