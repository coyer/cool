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

	bool	Open(const wchar_t* filename, bool writemode);
	bool	Open(const char* filename, bool writemode);
	bool	Open(const KString& filename, bool writemode);
	bool	IsOpened();

	FILE* Detach();
	void	Attach(FILE* file);

	KString	GetFilename();
	__int64 GetFileSize();

	void	Close();


	__int64		ReadFileLength();
	int		Seek(int offset, int startPos);
	int		SeekToBegin();
	int		SeekToEnd();

	int		Write(const void* data, int len, int type_size = 1);
	int		Read(void* data, int len, int type_size = 1);

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
	FILE*		m_file;
	KString		m_filename;
	int			m_flagLen;
	__int64		m_fileSize;
};


class KAPI KTextFile
{
public:
	enum CodeFormat {
		cf_bin = 0,
		cf_ansi,
		cf_utf8,
		cf_utf16,
		cf_utf16_be,
	};

	KTextFile();
	~KTextFile();

	bool	Open(const wchar_t* filename, bool writemode);
	bool	Open(const char* filename, bool writemode);
	bool	Open(const KString& filename, bool writemode);
	bool	IsOpened();
	bool	Create(const char* filename, CodeFormat fmt);
	bool	Create(const wchar_t* filename, CodeFormat fmt);
	bool	Create(const KString& filename, CodeFormat fmt);

	KString	GetFilename();
	__int64 GetFileSize();

	void	Close();

	int		ReadString(KStringW& strW);
	int		ReadString(KStringA& strA);
	int		WriteString(KStringW strW);
	int		WriteString(KStringA strA);
	int		WriteString(const wchar_t* str, int len = -1);
	int		WriteString(const char* str, int len = -1);
	bool	ReadAllText(KString& strOut);

	__int64		ReadFileLength();
	int		Seek(int offset, int startPos);
	int		SeekToBegin();
	int		SeekToEnd();

	int		Write(const void* data, int len, int type_size = 1);
	int		Read(void* data, int len, int type_size = 1);

	CodeFormat	GetCodeFormat();
	//this function will reset file content, do not use this function.
	void	SetCodeFormat(CodeFormat cf);

protected:
	void	_ParseFormat();
	int		_WriteFormat();

protected:
	FILE*		m_file;
	CodeFormat	m_format;
	KString		m_filename;
	int			m_flagLen;
	__int64		m_fileSize;
};