/*
Font Name Library 1.0.0

Copyright (c) 2009 Wong Shao Voon

The Code Project Open License (CPOL)
http://www.codeproject.com/info/cpol10.aspx
*/

#include "StdAfx.h"
#include "TTF.h"

TTF::TTF(void)
{
	m_pFile = NULL;
	m_nInfo = 0;
	m_bExternalFilePtr = false;
	m_pMemPtrReader = NULL;
	m_bBold = false;
	m_bRegular = false;
	m_bItalic = false;
}

TTF::~TTF(void)
{
	Clear();
}

bool TTF::Parse(const std::wstring& szFilePath)
{
	Clear();
	m_szFilePath = szFilePath;
	m_pFile = _wfopen(szFilePath.c_str(), L"rb");

	if(NULL==m_pFile)
		return false;

	return ParseOffsetTable();
}

bool TTF::Parse(FILE* pFile, size_t nOffsetFromStart)
{
	Clear();

	if(NULL==pFile)
		return false;

	m_pFile = pFile;


	m_bExternalFilePtr = true;

	return ParseOffsetTable();
}

bool TTF::ParseOffsetTable()
{
	stOffsetTable OffsetTable;
	memset(&OffsetTable, 0, sizeof(OffsetTable));
	fread(&OffsetTable, sizeof(OffsetTable), 1, m_pFile);
	EndianSwap::Switch(OffsetTable.numTables);

	for(USHORT i=0; i<OffsetTable.numTables; ++i)
	{
		stTableDirectory TableDirectory;
		memset(&TableDirectory, 0, sizeof(TableDirectory));
		fread(&TableDirectory, sizeof(TableDirectory), 1, m_pFile);
		std::string strTag = "";
		strTag += TableDirectory.tag[0];
		strTag += TableDirectory.tag[1];
		strTag += TableDirectory.tag[2];
		strTag += TableDirectory.tag[3];

		if(ToLower(strTag)=="name")
		{
			return ParseName(TableDirectory);
		}
	}

	if(m_pFile&&!m_bExternalFilePtr)
		fclose(m_pFile);

	m_pFile = NULL;

	return false;
}

bool TTF::Parse(BYTE* pFile, size_t size)
{
	return Parse(pFile, size, 0);
}

bool TTF::Parse(BYTE* pFile, size_t size, size_t offset)
{
	Clear();
	if(pFile == NULL)
		return false;

	m_pMemPtrReader = new MemPtrReader(pFile, size, offset);

	if(m_pMemPtrReader == NULL)
		return false;

	return ParseOffsetTableMem();
}

bool TTF::ParseOffsetTableMem()
{
	stOffsetTable OffsetTable;
	memset(&OffsetTable, 0, sizeof(OffsetTable));
	m_pMemPtrReader->Read(&OffsetTable, sizeof(OffsetTable), 1);
	EndianSwap::Switch(OffsetTable.numTables);

	for(USHORT i=0; i<OffsetTable.numTables; ++i)
	{
		stTableDirectory TableDirectory;
		memset(&TableDirectory, 0, sizeof(TableDirectory));
		m_pMemPtrReader->Read(&TableDirectory, sizeof(TableDirectory), 1);
		std::string strTag = "";
		strTag += TableDirectory.tag[0];
		strTag += TableDirectory.tag[1];
		strTag += TableDirectory.tag[2];
		strTag += TableDirectory.tag[3];

		if(ToLower(strTag)=="name")
		{
			return ParseNameMem(TableDirectory);
		}

	}

	if(m_pMemPtrReader)
		delete m_pMemPtrReader;

	m_pMemPtrReader = NULL;

	return false;
}

bool TTF::ParseName(stTableDirectory& TableDirectory)
{
	EndianSwap::Switch(TableDirectory.offset);

	fseek(m_pFile, TableDirectory.offset, SEEK_SET);

	stNamingTable NamingTable;
	memset(&NamingTable, 0, sizeof(NamingTable));
	fread(&NamingTable, sizeof(NamingTable), 1, m_pFile);
	EndianSwap::Switch(NamingTable.NumberOfNameRecords);
	EndianSwap::Switch(NamingTable.OffsetStartOfStringStorage);
	EndianSwap::Switch(NamingTable.FormatSelector);

	std::vector<stNameRecord> vec;
	for(USHORT i=0; i<NamingTable.NumberOfNameRecords; ++i)
	{
		stNameRecord NameRecord;
		memset(&NameRecord, 0, sizeof(NameRecord));
		fread(&NameRecord, sizeof(NameRecord), 1, m_pFile);
		EndianSwap::Switch(NameRecord.NameID);
		EndianSwap::Switch(NameRecord.EncodingID);
		EndianSwap::Switch(NameRecord.OffsetFromStorageArea);
		EndianSwap::Switch(NameRecord.StringLength);
		EndianSwap::Switch(NameRecord.PlatformID);
		EndianSwap::Switch(NameRecord.LanguageID);
		vec.push_back(NameRecord);
	}

	for(size_t a=0; a<vec.size(); ++a)
	{
		if(vec[a].PlatformID==0||vec[a].PlatformID==3)
		{
			fseek(m_pFile, TableDirectory.offset+NamingTable.OffsetStartOfStringStorage+vec[a].OffsetFromStorageArea, SEEK_SET);
			size_t size = vec[a].StringLength/2+1;
			WCHAR* pwBuf = new WCHAR[size];
			memset(pwBuf,0, size*sizeof(WCHAR));
			fread(pwBuf, vec[a].StringLength, 1, m_pFile);
			EndianSwap::Switch(pwBuf, size);
			std::wstring szValue = pwBuf;
			SetNameIDValue(vec[a].NameID, szValue);
			memset(pwBuf,0, size*sizeof(WCHAR));
			delete [] pwBuf;
			pwBuf = NULL;
			if(m_nInfo==0xFF)
				break;
		}
		else
		{
			fseek(m_pFile, TableDirectory.offset+NamingTable.OffsetStartOfStringStorage+vec[a].OffsetFromStorageArea, SEEK_SET);
			size_t size = vec[a].StringLength+1;
			CHAR* pwBuf = new CHAR[size];
			memset(pwBuf,0, size*sizeof(CHAR));
			fread(pwBuf, vec[a].StringLength, 1, m_pFile);
			std::string szValue = pwBuf;
			SetNameIDValue(vec[a].NameID, szValue);
			memset(pwBuf,0, size*sizeof(CHAR));
			delete [] pwBuf;
			pwBuf = NULL;
			if(m_nInfo==0xFF)
				break;
		}
	}

	if(m_pFile&&!m_bExternalFilePtr)
		fclose(m_pFile);

	m_pFile = NULL;

	return true;
}

bool TTF::ParseNameMem(stTableDirectory& TableDirectory)
{
	EndianSwap::Switch(TableDirectory.offset);

	m_pMemPtrReader->Seek(TableDirectory.offset, SEEK_SET);

	stNamingTable NamingTable;
	memset(&NamingTable, 0, sizeof(NamingTable));
	m_pMemPtrReader->Read(&NamingTable, sizeof(NamingTable), 1);
	EndianSwap::Switch(NamingTable.NumberOfNameRecords);
	EndianSwap::Switch(NamingTable.OffsetStartOfStringStorage);
	EndianSwap::Switch(NamingTable.FormatSelector);

	std::vector<stNameRecord> vec;
	for(USHORT i=0; i<NamingTable.NumberOfNameRecords; ++i)
	{
		stNameRecord NameRecord;
		memset(&NameRecord, 0, sizeof(NameRecord));
		m_pMemPtrReader->Read(&NameRecord, sizeof(NameRecord), 1);
		EndianSwap::Switch(NameRecord.NameID);
		EndianSwap::Switch(NameRecord.EncodingID);
		EndianSwap::Switch(NameRecord.OffsetFromStorageArea);
		EndianSwap::Switch(NameRecord.StringLength);
		EndianSwap::Switch(NameRecord.PlatformID);
		EndianSwap::Switch(NameRecord.LanguageID);
		vec.push_back(NameRecord);
	}

	for(size_t a=0; a<vec.size(); ++a)
	{
		if(vec[a].PlatformID==0||vec[a].PlatformID==3)
		{
			m_pMemPtrReader->Seek(TableDirectory.offset+NamingTable.OffsetStartOfStringStorage+vec[a].OffsetFromStorageArea, SEEK_SET);
			size_t size = vec[a].StringLength/2+1;
			WCHAR* pwBuf = new WCHAR[size];
			memset(pwBuf,0, size*sizeof(WCHAR));
			m_pMemPtrReader->Read(pwBuf, vec[a].StringLength, 1);
			EndianSwap::Switch(pwBuf, size);
			std::wstring szValue = pwBuf;
			SetNameIDValue(vec[a].NameID, szValue);
			memset(pwBuf,0, size*sizeof(WCHAR));
			delete [] pwBuf;
			pwBuf = NULL;
			if(m_nInfo==0xFF)
				break;
		}
		else
		{
			m_pMemPtrReader->Seek(TableDirectory.offset+NamingTable.OffsetStartOfStringStorage+vec[a].OffsetFromStorageArea, SEEK_SET);
			size_t size = vec[a].StringLength+1;
			CHAR* pwBuf = new CHAR[size];
			memset(pwBuf,0, size*sizeof(CHAR));
			m_pMemPtrReader->Read(pwBuf, vec[a].StringLength, 1);
			std::string szValue = pwBuf;
			SetNameIDValue(vec[a].NameID, szValue);
			memset(pwBuf,0, size*sizeof(CHAR));
			delete [] pwBuf;
			pwBuf = NULL;
			if(m_nInfo==0xFF)
				break;
		}
	}

	if(m_pMemPtrReader)
		delete m_pMemPtrReader;

	m_pMemPtrReader = NULL;

	return true;
}

std::wstring TTF::GetNameID(USHORT uNameID)
{
	switch(uNameID)
	{
	case 0:
		return L"Copyright notice";
	case 1:
		return L"Font Family name";
	case 2:
		return L"Font Subfamily name";
	case 3:
		return L"Unique font identifier";
	case 4:
		return L"Full font name";
	case 5:
		return L"Version string";
	case 6:
		return L"Postscript name for the font";
	case 7:
		return L"Trademark";
	case 8:
		return L"Manufacturer Name";
	case 9:
		return L"Designer";
	case 10:
		return L"Description";
	case 11:
		return L"URL Vendor";
	case 12:
		return L"URL Designer";
	case 13:
		return L"License Description";
	case 14:
		return L"License Info URL";
	case 15:
		return L"Reserved";
	case 16:
		return L"Preferred Family";
	case 17:
		return L"Preferred Subfamily";
	case 18:
		return L"Compatible Full (Macintosh only)";
	case 19:
		return L"Sample text";
	case 20:
		return L"PostScript CID findfont name";
	case 21:
		return L"WWS Family Name";
	case 22:
		return L"WWS Subfamily Name";
	default:
		return L"Unknown Name ID";
	}

	return L"Unknown Name ID";
}

void TTF::SetNameIDValue(USHORT uNameID, const std::wstring& str)
{
	switch(uNameID)
	{
	case 0:
		if(!(m_nInfo & (1<<0)))
			m_szCopyright = str;
		m_nInfo |= (1<<0);
		break;
	case 1:
		if(!(m_nInfo & (1<<1)))
			m_szFontFamilyName = str;
		m_nInfo |= (1<<1);
		break;
	case 2:
		if(!(m_nInfo & (1<<2)))
		{
			m_szFontSubFamilyName = str;
			ParseStyles();
		}
		m_nInfo |= (1<<2);
		break;
	case 3:
		if(!(m_nInfo & (1<<3)))
			m_szFontID = str;
		m_nInfo |= (1<<3);
		break;
	case 4:
		if(!(m_nInfo & (1<<4)))
			m_szFontName = str;
		m_nInfo |= (1<<4);
		break;
	case 5:
		if(!(m_nInfo & (1<<5)))
			m_szVersion = str;
		m_nInfo |= (1<<5);
		break;
	case 6:
		if(!(m_nInfo & (1<<6)))
			m_szPostScriptName = str;
		m_nInfo |= (1<<6);
		break;
	case 7:
		if(!(m_nInfo & (1<<7)))
			m_szTrademark = str;
		m_nInfo |= (1<<7);
		break;
	}
}

void TTF::SetNameIDValue(USHORT uNameID, const std::string& str)
{
	SetNameIDValue(uNameID, NarrowToWide(str));
}

std::wstring TTF::NarrowToWide(const std::string& szNarrow)
{
	int size = MultiByteToWideChar(
		CP_ACP,           // code page
		0,    // character-type options
		szNarrow.c_str(), // string to map
		szNarrow.size(),  // number of bytes in string
		NULL,  // wide-character buffer
		0      // size of buffer
		);

	size++;

	WCHAR* pBuf = new WCHAR[size];
	memset(pBuf,0,size*sizeof(WCHAR));

	int size2 = MultiByteToWideChar(
		CP_ACP,           // code page
		0,    // character-type options
		szNarrow.c_str(), // string to map
		szNarrow.size(),  // number of bytes in string
		pBuf,  // wide-character buffer
		size      // size of buffer
		);

	std::wstring szWide;
	if(size2>0)
		szWide = pBuf;

	memset(pBuf,0,size*sizeof(WCHAR));
	delete [] pBuf;
	pBuf = NULL;

	return szWide;
}

std::string TTF::WideToNarrow(const std::wstring& szWide)
{
	BOOL bUsedDefaultChar=FALSE;
	int size = WideCharToMultiByte(
		CP_ACP,           // code page
		0,    // character-type options
		szWide.c_str(), // string to map
		szWide.size(),  // number of bytes in string
		NULL,  // character buffer
		0,      // size of buffer
		NULL,
		&bUsedDefaultChar);

	size++;

	CHAR* pBuf = new CHAR[size];
	memset(pBuf,0,size*sizeof(CHAR));

	bUsedDefaultChar=FALSE;
	int size2 = WideCharToMultiByte(
		CP_ACP,           // code page
		0,    // character-type options
		szWide.c_str(), // string to map
		szWide.size(),  // number of bytes in string
		pBuf,  // character buffer
		size,      // size of buffer
		NULL,
		&bUsedDefaultChar);

	std::string szNarrow;
	if(size2>0)
		szNarrow = pBuf;

	memset(pBuf,0,size*sizeof(CHAR));
	delete [] pBuf;
	pBuf = NULL;

	return szNarrow;
}

void TTF::Clear()
{
	if(m_pFile&&!m_bExternalFilePtr)
		fclose(m_pFile);

	if(m_pMemPtrReader)
		delete m_pMemPtrReader;

	m_pFile = NULL;
	m_pMemPtrReader = NULL;
	m_szFontName = L"";
	m_szFilePath = L"";
	m_szCopyright = L"";
	m_szFontFamilyName = L"";
	m_szFontSubFamilyName = L"";
	m_szFontID = L"";
	m_szVersion = L"";
	m_szPostScriptName = L"";
	m_szTrademark = L"";
	m_nInfo = 0;
	m_bExternalFilePtr = false;
	m_bBold = false;
	m_bRegular = false;
	m_bItalic = false;
}


void TTF::ParseStyles()
{
	m_bBold = false;
	if(ToLower(m_szFontSubFamilyName).find(L"bold")!=-1)
		m_bBold = true;

	m_bItalic = false;
	if(ToLower(m_szFontSubFamilyName).find(L"italic")!=-1)
		m_bItalic = true;
	
	m_bRegular = false;
	if(ToLower(m_szFontSubFamilyName).find(L"regular")!=-1)
		m_bRegular = true;
	else if(ToLower(m_szFontSubFamilyName).find(L"normal")!=-1)
		m_bRegular = true;
	else if(ToLower(m_szFontSubFamilyName).find(L"standard")!=-1)
		m_bRegular = true;
}

bool TTF::IsBold()
{
	return m_bBold;
}

bool TTF::IsItalic()
{
	return m_bItalic;
}

bool TTF::IsRegular()
{
	return m_bRegular;
}

std::wstring TTF::ToLower(const std::wstring& szNormal)
{
	std::wstring szLower = L"";
	for(size_t i=0; i<szNormal.size() ; ++i)
	{
		szLower += (wchar_t)(towlower(szNormal.at(i)));
	}

	return szLower;
}

std::string TTF::ToLower(const std::string& szNormal)
{
	std::string szLower = "";
	for(size_t i=0; i<szNormal.size() ; ++i)
	{
		szLower += (char)(towlower(szNormal.at(i)));
	}

	return szLower;
}
