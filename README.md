# How to Use a Font Without Installing it

Source code for the CodeProject article of [How to Use a Font Without Installing it](https://www.codeproject.com/Articles/42041/How-to-Use-a-Font-Without-Installing-it)

### Table of Contents


* Introduction
* GDI&#39;s AddFontResourceEx
* GDI&#39;s AddFontMemResourceEx
* GDI+&#39;s PrivateFontCollection&#39;s AddFontFile
* GDI+&#39;s PrivateFontCollection&#39;s AddMemoryFont
* Getting TTF and TTC Font Names
* Add Font File to a Resource
* Conclusion
* Reference
* History


![samplefont.png](/images/samplefont.png)

### Introduction

Many times, a particular font needs to be used in an application due to the in-house graphics designer&#39;s font choice. In order for the application to use the fonts, the font needs to be installed using the installer. Too many fonts on the user machine may slow the system down considerably.

You can actually get away without installing the font: GDI and GDI+ each provide two ways for you, as a programmer, to add a font for an application to use without installing it. I&#39;ll show you how in this article!

### GDI&#39;s AddFontResourceEx

Let me first talk about GDI&#39;s two functions for adding fonts to an application for use. I&#39;ll then talk about GDI+&#39;s own functions. You can use [AddFontResourceEx](http://msdn.microsoft.com/en-us/library/dd183327%28VS.85%29.aspx) to add a physical font file for an application to use.

```Cpp
int AddFontResourceEx(
  LPCTSTR lpszFilename, 	// font file name
  DWORD fl,             	// font characteristics
  PVOID pdv             	// reserved
);
```

Here is an example of how to use `AddFontResourceEx`:

```Cpp
CString szFontFile = "D:\\SkiCargo.ttf";

int nResults = AddFontResourceEx(
    m_szFontFile, 		// font file name
    FR_PRIVATE,    	// font characteristics
    NULL);
```

To use the font you&#39;ve added, just specify its name in the `CreateFont` or `CreateFontIndirect` function like any other installed font. To know the name of the font, just right click on the TTF extension file in Windows Explorer and select "Open" and you will see its actual name. Or, you can use the `TTF` and `TTC` classes which I wrote, to know the font name.

__Note__: The font file name ("_SkiCargo.ttf_") in this article is actually its font name, "SkiCargo"; this is usually not the case! To be on the safe side, use the Windows Explorer right click method, or the `TTF` and `TTC` classes I just mentioned, to find out the name!

```Cpp
CClientDC dc(this);

dc.SetBkMode(TRANSPARENT);

LOGFONT lf;
memset(&lf, 0, sizeof(lf));
lf.lfHeight = -MulDiv(24, pDC->GetDeviceCaps(LOGPIXELSY), 72);
lf.lfWeight = FW_NORMAL;
lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
wcscpy_s(lf.lfFaceName, L"SkiCargo");

// create and select it
CFont newFont;
if (!newFont.CreateFontIndirect(&lf))
    return;
CFont* pOldFont = dc.SelectObject(&newFont);

// use a path to record how the text was drawn
wchar_t buf[] = _T("The quick brown fox jumps over the lazy dog!");
dc.TextOut( 10, 10, buf, wcslen(buf));

// Put back the old font
dc.SelectObject(pOldFont);
```

You must remember to call [RemoveFontResourceEx](http://msdn.microsoft.com/en-us/library/dd162923%28VS.85%29.aspx) before the application exits. You should note that the parameters must be the same as the ones that you fed into `AddFontResourceEx`!

```Cpp
BOOL RemoveFontResourceEx(
  LPCTSTR lpFileName,  	// name of font file
  DWORD fl,            	// font characteristics
  PVOID pdv            	// Reserved.
);

CString szFontFile = "D:\\SkiCargo.ttf";

BOOL b = RemoveFontResourceEx(
    m_szFontFile, 		// name of font file
    FR_PRIVATE,   		// font characteristics
    NULL         		// Reserved.
    );
```

### GDI&#39;s AddFontMemResourceEx

If our font is in a resource DLL, cabinet file, or archival compressed file, you can extract it into memory and then use [AddFontMemResourceEx](http://msdn.microsoft.com/en-us/library/dd183325%28VS.85%29.aspx) to read it from memory.

```Cpp
HANDLE AddFontMemResourceEx(
  PVOID pbFont,       	// font resource
  DWORD cbFont,       	// number of bytes in font resource 
  PVOID pdv,          	// Reserved. Must be 0.
  DWORD *pcFonts      	// number of fonts installed
);
```

Here is an example of how to use `AddFontMemResourceEx` on a font file embedded in a resource. __Note__: To learn how to add a font file to a resource, you can refer to this section</a> later in the article.

```Cpp
HINSTANCE hResInstance = AfxGetResourceHandle( );

HRSRC res = FindResource(hResInstance,
    MAKEINTRESOURCE(IDR_MYFONT),L"BINARY");
if (res) 
{
    HGLOBAL mem = LoadResource(hResInstance, res);
    void *data = LockResource(mem);
    size_t len = SizeofResource(hResInstance, res);

    DWORD nFonts;
    m_fonthandle = AddFontMemResourceEx(
        data,       	// font resource
        len,       	// number of bytes in font resource 
        NULL,          	// Reserved. Must be 0.
        &nFonts      	// number of fonts installed
        );

    if(m_fonthandle==0)
    {
        MessageBox(L"Font add fails", L"Error");
    }
}
```

To use the font you have added, please refer to the previous `AddFontResourceEx` example. They are the same. Just use it like any other installed font. You should call [RemoveFontMemResourceEx](http://msdn.microsoft.com/en-us/library/dd162921%28VS.85%29.aspx) before the application exits. When the process goes away, the system will unload the fonts, even if you don&#39;t call `RemoveFontMemResourceEx`. __Note__: the parameters must be the same as the ones you feed into `AddFontResourceEx`!

```Cpp
BOOL RemoveFontMemResourceEx(
  HANDLE fh   // handle to the font resource
);

if(m_fonthandle)
{
    BOOL b = RemoveFontMemResourceEx(m_fonthandle);
    if(b==0)
    {
        MessageBox(L"Font remove fails", L"Error");
    }
}
```

### GDI+&#39;s PrivateFontCollection&#39;s AddFontFile

For GDI+, you can use its `PrivateFontCollection` class member [AddFontFile](http://msdn.microsoft.com/en-us/library/ms534992%28VS.85%29.aspx) to add a physical font file.

```Cpp
Status AddFontFile(const WCHAR* filename);
```

Here is how to use `AddFontFile` to add a font file:

```Cpp
Gdiplus::PrivateFontCollection m_fontcollection;
//...
CString szFontFile = szExePath + L"SkiCargo.ttf";

Gdiplus::Status nResults = m_fontcollection.AddFontFile(szFontFile);
```

Here is how to use the font we have just added to the `PrivateFontCollection` object, `m_fontcollection`.

```Cpp
// When painting the text
FontFamily fontFamily;
int nNumFound=0;
m_fontcollection.GetFamilies(1,&fontFamily,&nNumFound);

if(nNumFound>0)
{
    Font font(&fontFamily,28,FontStyleRegular,UnitPixel);

    StringFormat strformat;
    wchar_t buf[] = L"The quick brown fox jumps over the lazy dog!";
    graphics.DrawString(buf,wcslen(buf),&font, 
             PointF(10.0f,10.0f),&strformat,&brush);
}
```

__Note__: Unlike GDI&#39;s `AddFontResourceEx` and `AddFontMemResourceEx`, there is no `RemoveFontFile` for `AddFontFile`. All added fonts will be removed by `PrivateFontCollection`&#39;s destructor.

### GDI+&#39;s PrivateFontCollection&#39;s AddMemoryFont

For GDI+, you can use its `PrivateFontCollection` class member [AddMemoryFont](http://msdn.microsoft.com/en-us/library/ms534993%28VS.85%29.aspx) to add a font in memory.

```Cpp
Status AddMemoryFont(const VOID *memory, INT length);
```

Here is how to use `AddMemoryFont` on a font file embedded in the resource. Similar to `AddFontFile`, there is no `RemoveMemoryFont` to call. Everything will be taken care of by `PrivateFontCollection`&#39;s destructor. __Note__: To learn how to add a font file to a resource, you can refer to this section</a> later in the article.

```Cpp
HINSTANCE hResInstance = AfxGetResourceHandle( );

HRSRC res = FindResource(hResInstance,
    MAKEINTRESOURCE(IDR_MYFONT),L"BINARY");
if (res) 
{
    HGLOBAL mem = LoadResource(hResInstance, res);
    void *data = LockResource(mem);
    size_t len = SizeofResource(hResInstance, res);

    Gdiplus::Status nResults = m_fontcollection.AddMemoryFont(data,len);

    if(nResults!=Gdiplus::Ok)
    {
        MessageBox(L"Font add fails", L"Error");
    }
}
```

As to how to use the font you have just added to the `PrivateFontCollection` object, `m_fontcollection`, please refer to the previous `AddFontFile` example, they are the same.

### Getting TTF and TTC Font Names

I have written two classes, namely `TTF` and `TTC` to read the font name from the TTF/OTF and TTC font files, respectively. To support Matroska (_mkv_) file font reading or embedded font resource reading, my `TTF` and `TTC` classes support parsing the font file in memory. For your information, these Matroska files usually contain video channels, audio channels for multiple languages, subtitles, and the fonts used for the subtitles in the video. My classes are really easy to use. Below is an example to read a TTF file physically or in memory and display its information:

```Cpp
void TestReadTtfFromFile(const std::wstring& szFile)
{
    TTF ttf;
    ttf.Parse(szFile);
    Display(ttf);
}

void TestReadTtfFromMemory(const std::wstring& szFile)
{
    struct _stat bufferStat;
    int nRet = _wstat(szFile.c_str(), &bufferStat);
    FILE* pFile = _wfopen(szFile.c_str(), L"rb");
    if(pFile == NULL)
    {
        std::wcout<<L"Failed to create file"<<std::endl;
        return;
    }
    BYTE* buf = new BYTE[bufferStat.st_size];
    fread(buf,bufferStat.st_size,1,pFile);
    fclose(pFile);
    TTF ttf;
    ttf.Parse(buf, bufferStat.st_size);

    delete [] buf;

    Display(ttf);
}

void Display(TTF& ttf)
{
    std::wcout<<L"FontName : "<<ttf.GetFontName()<<std::endl;
    std::wcout<<L"Copyright : "<<ttf.GetCopyright()<<std::endl;
    std::wcout<<L"FontFamilyName : "<<ttf.GetFontFamilyName()<<std::endl;
    std::wcout<<L"FontSubFamilyName : "<<ttf.GetFontSubFamilyName()<<std::endl;
    std::wcout<<L"FontID : "<<ttf.GetFontID()<<std::endl;
    std::wcout<<L"Version : "<<ttf.GetVersion()<<std::endl;
    std::wcout<<L"PostScriptName : "<<ttf.GetPostScriptName()<<std::endl;
    std::wcout<<L"Trademark : "<<ttf.GetTrademark()<<std::endl;

    std::wstring szBold = ttf.IsBold() ? L"true" : L"false"; 
    std::wstring szItalic = ttf.IsItalic() ? L"true" : L"false"; 
    std::wstring szRegular = ttf.IsRegular() ? L"true" : L"false"; 

    std::wcout<<L"Bold : "<<szBold<<std::endl;
    std::wcout<<L"Italic : "<<szItalic<<std::endl;
    std::wcout<<L"Regular : "<<szRegular<<std::endl;

    std::wcout<<std::endl;
}
```

TTC is a font file which contains a collection of TTF fonts. Below is an example to read a TTC file physically or in memory and display its information.

```Cpp
void TestReadTtcFromFile(const std::wstring& szFile)
{
    TTC ttc;
    ttc.Parse(szFile);
    Display(ttc);
}

void TestReadTtcFromMemory(const std::wstring& szFile)
{
    struct _stat bufferStat;
    int nRet = _wstat(szFile.c_str(), &bufferStat);
    FILE* pFile = _wfopen(szFile.c_str(), L"rb");
    if(pFile == NULL)
    {
        std::wcout<<L"Failed to create file"<<std::endl;
        return;
    }
    BYTE* buf = new BYTE[bufferStat.st_size];
    fread(buf,bufferStat.st_size,1,pFile);
    fclose(pFile);
    TTC ttc;
    ttc.Parse(buf, bufferStat.st_size);

    delete [] buf;

    Display(ttc);
}

void Display(TTC& ttc)
{
    for(size_t i=0; i<ttc.Size(); ++i )
    {
        std::wcout<<L"FontName : "<<ttc.GetFontName(i)<<std::endl;
        std::wcout<<L"Copyright : "<<ttc.GetCopyright(i)<<std::endl;
        std::wcout<<L"FontFamilyName : "<<ttc.GetFontFamilyName(i)<<std::endl;
        std::wcout<<L"FontSubFamilyName : "<<ttc.GetFontSubFamilyName(i)<<std::endl;
        std::wcout<<L"FontID : "<<ttc.GetFontID(i)<<std::endl;
        std::wcout<<L"Version : "<<ttc.GetVersion(i)<<std::endl;
        std::wcout<<L"PostScriptName : "<<ttc.GetPostScriptName(i)<<std::endl;
        std::wcout<<L"Trademark : "<<ttc.GetTrademark(i)<<std::endl;

        std::wstring szBold = ttc.IsBold(i) ? L"true" : L"false"; 
        std::wstring szItalic = ttc.IsItalic(i) ? L"true" : L"false"; 
        std::wstring szRegular = ttc.IsRegular(i) ? L"true" : L"false"; 

        std::wcout<<L"Bold : "<<szBold<<std::endl;
        std::wcout<<L"Italic : "<<szItalic<<std::endl;
        std::wcout<<L"Regular : "<<szRegular<<std::endl;

        std::wcout<<std::endl;
    }
}
```

__Note__: You should always call the `GetFontFamilyName` method to get the font name, not the `GetFontName` method. Most fonts belong to a font family. For example, under the Arial font family, there are several Arial fonts whose font names are "Arial Bold", "Arial Bold Italic", and so on. Below is an example on how to use `TTF`&#39;s `GetFontFamilyName` method with the `AddFontResourceEx` function:

```Cpp
TTF m_Ttf;

//... During Initialization
CString szFontFile = "D:\\SkiCargo.ttf";

int nResults = AddFontResourceEx(
    m_szFontFile, 		// font file name
    FR_PRIVATE,           	// font characteristics
    NULL);

m_Ttf.Parse((LPCWSTR)(m_szFontFile));    
    
//... In the OnPaint method    
    
CClientDC dc(this);

dc.SetBkMode(TRANSPARENT);

LOGFONT lf;
memset(&lf, 0, sizeof(lf));
lf.lfHeight = -MulDiv(24, pDC->GetDeviceCaps(LOGPIXELSY), 72);
lf.lfWeight = FW_NORMAL;
lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
//wcscpy_s(lf.lfFaceName, L"SkiCargo");
wcscpy_s(lf.lfFaceName, m_Ttf.GetFontFamilyName().c_str());

// create and select it
CFont newFont;
if (!newFont.CreateFontIndirect(&lf))
    return;
CFont* pOldFont = dc.SelectObject(&newFont);

// use a path to record how the text was drawn
wchar_t buf[] = _T("The quick brown fox jumps over the lazy dog!");
dc.TextOut( 10, 10, buf, wcslen(buf));

// Put back the old font
dc.SelectObject(pOldFont);
```

__Note__: I could not find enough information on the web to parse a fon file which is a font file with the "fon" extension. I tried reverse engineering to get the file name, but failed. However, I will continue trying.

### Add Font File to a Resource

To add a font file to a resource section, please follow my walkthrough example. Please note my method is to edit the resource file directly, instead of adding it though the IDE&#39;s resource editor because in my experience, the resource editor has the tendency to mess up the resource&#39;s rc file, rendering the WYSIWYG dialog editor unusable. __Note__: The latest resource editor may be more robust and stable now. To add a font file, we must assign a resource ID to refers to the font. To do this, close your solution or project in concern, if they are opened. To assign a resource ID, open up _Resource.h_:

```Cpp
//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by TestGDI_AddFontMem.RC
//
#define IDR_MAINFRAME			128
#define IDD_TESTGDI_ADDFONTMEM_DIALOG	102

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS

#define _APS_NEXT_RESOURCE_VALUE		129
#define _APS_NEXT_CONTROL_VALUE		1000
#define _APS_NEXT_SYMED_VALUE		101
#define _APS_NEXT_COMMAND_VALUE		32771
#endif
#endif
```

I believe you have many more resource IDs than this simple project of mine. Let&#39;s name our defined ID, "`IDR_MYFONT`". Of course, you can name it any way you deem appropriate. We assign `IDR_MYFONT`, the current value of `_APS_NEXT_RESOURCE_VALUE `which is `129`. And then we will increment `_APS_NEXT_RESOURCE_VALUE`; This is important we must do this or else the next resource will share the same numeric ID as your font. Below is what the manually edited _Resource.h_ looks like:

```Cpp
//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by TestGDI_AddFontMem.RC
//
#define IDR_MAINFRAME			128
#define IDD_TESTGDI_ADDFONTMEM_DIALOG	102
#define IDR_MYFONT				129

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS

#define _APS_NEXT_RESOURCE_VALUE		130
#define _APS_NEXT_CONTROL_VALUE		1000
#define _APS_NEXT_SYMED_VALUE		101
#define _APS_NEXT_COMMAND_VALUE		32771
#endif
#endif
```

Next we will edit the rc extension file. Open the file in your favorite text editor. __Note__: Your Visual Studio should not be opening the project which this rc file is in. Search for the section listed below:

```Cpp
/////////////////////////////////////////////////////////////////////////////
//
// BINARY
//
```

If you cannot find this section, you can add it yourself. Next add the font id and its font file. Your binary section may look like this.

```Cpp
/////////////////////////////////////////////////////////////////////////////
//
// BINARY
//
IDR_MYFONT              BINARY                  "res\\SkiCargo.ttf"
```

As the RC code shows, `IDR_MYFONT `is a binary resource which references a _SkiCargo.ttf_ file in a "_res_" subfolder of the project folder.

If you find adding a font to resource is a hassle, you can rename the font filename and its extension, so that nobody will know that file is a font and mess with it. You can even encrypt or compress it. Just decrypt it or uncompress it in memory before reading file in memory.

### Conclusion

You have seen two methods from GDI and GDI+ each, to either load font files physically or from memory and use them. I hope this may remove the need for programmers to install fonts on user machine to use them. I have introduced two classes to read TTF and TTC font files for their font names. Anything you do like or not like about this article, please let me know, so that I can improve on this article. I do hope you enjoy reading my article!

### Reference


* [Microsoft Typography](http://www.microsoft.com/typography/default.mspx)


### History


* 14<sup>th</sup> September, 2009 - Added a section on how to add the font to the resource
* 31<sup>st</sup> August, 2009 - First release on CodeProject


