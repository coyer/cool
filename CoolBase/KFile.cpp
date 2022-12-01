#include "stdafx.h"
#include "../include/base/kfile.h"
#include "../include/base/kstringhelp.h"
#include <io.h>
#include <tchar.h>
#ifdef WIN32
#include <shlobj.h>
#endif

#define READ_LINE_MAXLEN		2048

KFile::KFile()
{
	m_file = 0;
	m_flagLen = 0;
	m_fileSize = 0;
}

KFile::~KFile()
{
	if (m_file)
		fclose(m_file);
}

bool KFile::Open(const wchar_t* filename, bool writemode)
{
	if (!filename) return false;
	KString str = filename;
	return Open(str, writemode);
}

bool KFile::Open(const char* filename, bool writemode)
{
	if (!filename) return false;
	KString str = filename;
	return Open(str, writemode);
}

bool KFile::Open(const KString& filename, bool writemode)
{
	Close();

	KString sFlag;
	if (writemode) {
		sFlag = _T("w+b");
	}
	else {
		sFlag = _T("rb");
	}

	m_file = _tfopen(filename.c_str(), sFlag.c_str());
	if (0 == m_file)
		return false;

	m_filename = filename;

	m_fileSize = filelength(fileno(m_file));
	SeekToBegin();

	return true;
}

bool KFile::IsOpened()
{
	return m_file != 0;
}

KString	KFile::GetFilename()
{
	return m_filename;
}

__int64 KFile::GetFileSize()
{
	return m_fileSize;
}

FILE* KFile::Detach()
{
	FILE* fp = m_file;
	m_file = 0;

	return fp;
}

void KFile::Attach(FILE* file)
{
	Close();

	m_file = file;
}

void KFile::Close()
{
	if (m_file)
	{
		fclose(m_file);
		m_file = 0;
	}
}

__int64	KFile::ReadFileLength()
{
	long tmp = ftell(m_file);
	fseek(m_file, 0, SEEK_END);

	long ret = ftell(m_file);
	fseek(m_file, tmp, SEEK_SET);

	return ret;
}

int	KFile::Seek(int offset, int startPos)
{
	return fseek(m_file, offset, startPos);
}

int	KFile::SeekToBegin()
{
	int ret = fseek(m_file, m_flagLen, SEEK_SET);
	return ret;
}

int	KFile::SeekToEnd()
{
	return fseek(m_file, 0, SEEK_END);
}

int	KFile::Write(const void* data, int len, int type_size)
{
	return fwrite(data, type_size, len, m_file);
}

int	KFile::Read(void* data, int len, int type_size)
{
	return fread(data, type_size, len, m_file);
}

int KFile::EnumFiles(const TCHAR* filepath, const TCHAR* ext, KFileEnumProc proc, void* procData)
{
	if (0 == filepath || filepath[0] == 0)
		return 0;
	KString sPath = filepath;
	KString sExt = ext;
	return EnumFiles(sPath, sExt, proc, procData);
}

int KFile::EnumFiles(KString& filepath, KString& ext, KFileEnumProc proc, void* procData)
{
	//return KFile::EnumFiles(filepath.c_str(), ext.c_str(), proc, procData);

	if (filepath.GetLength() == 0)
		return 0;

	HANDLE hFind;
	WIN32_FIND_DATA findData;

	KString strfull = filepath;
	TCHAR ch = strfull.GetLast();
	if (ch != _T('/') && ch != _T('\\'))
		strfull.Append(_T('/'));

	if (ext.GetLength() != 0)
		strfull.Append(ext);
	else
		strfull.Append(_T("*.*"));

	hFind = FindFirstFile(strfull.c_str(), &findData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	int count = 0;
	do
	{
		// 忽略"."和".."两个结果 
		if (_tcscmp(findData.cFileName, _T(".")) == 0 || _tcscmp(findData.cFileName, _T("..")) == 0) {
			continue;
		}

		count++;
		EnumFileInfo info;
		info.filePath = filepath;
		info.fileName = findData.cFileName;
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)    // 是否是目录 
		{
			info.isFolder = TRUE;
		}
		else {
			LARGE_INTEGER size;
			size.LowPart = findData.nFileSizeLow;
			size.HighPart = findData.nFileSizeHigh;

			info.filePath = filepath;
			info.isFolder = FALSE;
			info.attrib = findData.dwFileAttributes;
			info.timeCreate = ((__int64)findData.ftCreationTime.dwHighDateTime << 32) + findData.ftCreationTime.dwLowDateTime;
			info.timeWrite = ((__int64)findData.ftLastWriteTime.dwHighDateTime << 32) + findData.ftLastWriteTime.dwLowDateTime;
			info.timeAccess = ((__int64)findData.ftLastAccessTime.dwHighDateTime << 32) + findData.ftLastAccessTime.dwLowDateTime;
			info.fileSize = size.QuadPart;
		}
		proc(info, procData);
	} while (FindNextFile(hFind, &findData));

	::FindClose(hFind);
	return count;
}

typedef struct tag_list_file_void_data {
	bool rescuive;
	KArrayT<EnumFileInfo>* outarray;
	KString sPath;
	KString sExt;
	int count;
}list_file_void_data;

void _list_file_rescure(EnumFileInfo info, void* pVoid)
{
	list_file_void_data * pData = (list_file_void_data *)pVoid;
	pData->outarray->Add(info);
	if (pData->rescuive && info.isFolder) {
		KString strPath = info.filePath + _T('//') + info.fileName;
		KFile::ListFiles(strPath, pData->sExt, true, pData->outarray);
	}
}

bool KFile::ListFiles(const TCHAR* filepath, const TCHAR* ext, bool rescuive, KArrayT<EnumFileInfo>* outarray)
{
	if (!filepath || filepath[0] == 0) return false;

	KString sPath = filepath;
	KString sExt = ext;
	return KFile::ListFiles(sPath, sExt, rescuive, outarray);
}

bool KFile::ListFiles(const KString& filepath, const KString& ext, bool rescuive, KArrayT<EnumFileInfo>* outarray)
{
	list_file_void_data data;
	data.rescuive = rescuive;
	data.outarray = outarray;
	data.sPath = filepath;
	data.sExt = ext;

	KFile::EnumFiles(filepath, ext, _list_file_rescure, &outarray);

	return true;
}

KString KFile::GetFolderPath(KFile::FolderName fn, bool allUsers)
{
	KString strPath;

#ifdef WIN32
	TCHAR szPath[READ_LINE_MAXLEN] = { 0 };
	switch (fn)
	{
	case FolderName::fnSystem:
		SHGetSpecialFolderPath(0, szPath, CSIDL_SYSTEM, 0);
		break;
	case FolderName::fnWindows:
		SHGetSpecialFolderPath(0, szPath, CSIDL_WINDOWS, 0);
		break;
	case FolderName::fnRecycle:
		SHGetSpecialFolderPath(0, szPath, CSIDL_BITBUCKET, 0);
		break;
	case FolderName::fnProgramFiles:
		SHGetSpecialFolderPath(0, szPath, CSIDL_PROGRAM_FILES, 0);
		break;
	case FolderName::fnStartup:
		if (allUsers)
			SHGetSpecialFolderPath(0, szPath, CSIDL_COMMON_STARTUP, 0);
		else
			SHGetSpecialFolderPath(0, szPath, CSIDL_STARTUP, 0);
		break;
	case FolderName::fnRecent:
		SHGetSpecialFolderPath(0, szPath, CSIDL_RECENT, 0);
		break;
	case FolderName::fnSendto:
		SHGetSpecialFolderPath(0, szPath, CSIDL_SENDTO, 0);
		break;
	case FolderName::fnPrograms:
		if (allUsers)
			SHGetSpecialFolderPath(0, szPath, CSIDL_PROGRAMS, 0);
		else
			SHGetSpecialFolderPath(0, szPath, CSIDL_COMMON_PROGRAMS, 0);
		break;
	case FolderName::fnLocalAppData:
		SHGetSpecialFolderPath(0, szPath, CSIDL_LOCAL_APPDATA, 0);
		break;
	case FolderName::fnTemplates:
		if (allUsers)
			SHGetSpecialFolderPath(0, szPath, CSIDL_COMMON_TEMPLATES, 0);
		else
			SHGetSpecialFolderPath(0, szPath, CSIDL_TEMPLATES, 0);
		break;
	case FolderName::fnDesktop:
		if (allUsers)
			SHGetSpecialFolderPath(0, szPath, CSIDL_COMMON_DESKTOPDIRECTORY, 0);
		else
			SHGetSpecialFolderPath(0, szPath, CSIDL_DESKTOPDIRECTORY, 0);
		break;
	case FolderName::fnCommonData:
		if (allUsers)
			SHGetSpecialFolderPath(0, szPath, CSIDL_COMMON_APPDATA, 0);
		else
			SHGetSpecialFolderPath(0, szPath, CSIDL_APPDATA, 0);
		break;
	case FolderName::fnDocument:
		if (allUsers)
			SHGetSpecialFolderPath(0, szPath, CSIDL_COMMON_DOCUMENTS, 0);
		else
			SHGetSpecialFolderPath(0, szPath, CSIDL_MYDOCUMENTS, 0);
		break;
	case FolderName::fnFavorites:
		if (allUsers)
			SHGetSpecialFolderPath(0, szPath, CSIDL_COMMON_FAVORITES, 0);
		else
			SHGetSpecialFolderPath(0, szPath, CSIDL_FAVORITES, 0);
		break;
	case FolderName::fnStartMenu:
		if (allUsers)
			SHGetSpecialFolderPath(0, szPath, CSIDL_COMMON_STARTMENU, 0);
		else
			SHGetSpecialFolderPath(0, szPath, CSIDL_STARTMENU, 0);
		break;
	case FolderName::fnMusic:
		if (allUsers)
			SHGetSpecialFolderPath(0, szPath, CSIDL_COMMON_MUSIC, 0);
		else
			SHGetSpecialFolderPath(0, szPath, CSIDL_MYMUSIC, 0);
		break;
	case FolderName::fnPictures:
		if (allUsers)
			SHGetSpecialFolderPath(0, szPath, CSIDL_COMMON_PICTURES, 0);
		else
			SHGetSpecialFolderPath(0, szPath, CSIDL_MYPICTURES, 0);
		break;
	case FolderName::fnVideo:
		if (allUsers)
			SHGetSpecialFolderPath(0, szPath, CSIDL_COMMON_VIDEO, 0);
		else
			SHGetSpecialFolderPath(0, szPath, CSIDL_MYVIDEO, 0);
		break;
	default:
		break;
	}

	strPath = szPath;
#else
#endif

	return strPath;
}

bool KFile::CheckExist(const char* filename)
{
	return ((_access(filename, 0)) != -1);
}

bool KFile::CheckExist(const wchar_t* filename)
{
	return ((_waccess(filename, 0)) != -1);
}

bool KFile::CheckExist(const KString& filename)
{
	return ((_taccess(filename.c_str(), 0)) != -1);
}
/////////////////////////////////////////////////////////////////////////////////////////////

