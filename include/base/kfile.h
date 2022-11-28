#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : kfile.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : I love MFC's class CFile, this is same as CFile.

*******************************************************************************************/


#include "kdef.h"

#include <stdio.h>
#include "kstring.h"

typedef struct tagEnumFileInfo
{
	KString filePath;
	KString fileName;
	__int64 fileSize;
	unsigned int attrib;
	time_t timeCreate;
	time_t timeWrite;
	time_t timeAccess;
	BOOL   isFolder;
}EnumFileInfo;

typedef void (*KFileEnumProc)(EnumFileInfo info, void* pVoid);

class KAPI KFile
{
public:
	enum OpenFlags {
		modeRead =         (int) 0x00000,
		modeWrite =        (int) 0x00001,
		modeReadWrite =    (int) 0x00002,
		shareCompat =      (int) 0x00000,
		shareExclusive =   (int) 0x00010,
		shareDenyWrite =   (int) 0x00020,
		shareDenyRead =    (int) 0x00030,
		shareDenyNone =    (int) 0x00040,
		modeNoInherit =    (int) 0x00080,
		modeCreate =       (int) 0x01000,
		modeNoTruncate =   (int) 0x02000,
		typeText =         (int) 0x04000, // typeText and typeBinary are
		typeBinary =       (int) 0x08000, // used in derived classes only
		osNoBuffer =       (int) 0x10000,
		osWriteThrough =   (int) 0x20000,
		osRandomAccess =   (int) 0x40000,
		osSequentialScan = (int) 0x80000,
		};
		
	enum CodeFormat {
		cf_ansi = 0,
		cf_utf8,
		cf_utf16,
		cf_utf16_be,
	};

	enum FolderName {
		fnSystem = 0,
		fnWindows,
		fnStartup,
		fnRecent,
		fnSendto,
		fnPrograms,
		fnLocalAppData,
		fnRecycle,
		fnProgramFiles,
		fnTemplates,
		fnDesktop,
		fnCommonData,
		fnDocument,
		fnFavorites,
		fnStartMenu,
		fnMusic,
		fnPictures,
		fnVideo,
	};

	enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };

	KFile();
	~KFile();

	bool	Open(const wchar_t* filename, unsigned int openflag);
	bool	Open(const char* filename, unsigned int openflag);
	bool	Open(KString& filename, unsigned int openflag);
	bool	IsOpened();

	KString	GetFilename();

	FILE*	Detach();
	void	Attach(FILE* file);

	void	Close();

	int		ReadString(KStringW& strW);
	int		ReadString(KStringA& strA);
	int		WriteString(KStringW strW);
	int		WriteString(KStringA strA);
	int		WriteString(const wchar_t* str, int len = -1);
	int		WriteString(const char* str, int len = -1);

	int		GetLength();
	int		Seek(int offset, int startPos);
	int		SeekToBegin();
	int		SeekToEnd();

	int		Write(const void* data, int len);
	int		Read(void* data, int len);
	
	KFile::CodeFormat	GetCodeFormat();
	//this function will reset file content, do not use this function.
	void	SetCodeFormat(KFile::CodeFormat cf);

public:
	static int  EnumFiles(const TCHAR* filepath, const TCHAR* ext, KFileEnumProc proc, void* procData);
	static int  EnumFiles(KString& filepath, KString& ext, KFileEnumProc proc, void* procData);
	static bool  ListFiles(const TCHAR* filepath, const TCHAR* ext, bool rescuive, KArrayT<EnumFileInfo>* outarray);
	static bool  ListFiles(const KString& filepath, const KString& ext, bool rescuive, KArrayT<EnumFileInfo>* outarray);
	static KString GetFolderPath(KFile::FolderName fn, bool allUsers = false);
	static bool	CheckExist(const char* filename);
	static bool	CheckExist(const wchar_t* filename);
	static bool CheckExist(const KString& filename);

protected:
	void	_ParseFormat();

protected:
	FILE*		m_file;
	CodeFormat	m_format;
	KString		m_filename;
	int			m_flagLen;
};
