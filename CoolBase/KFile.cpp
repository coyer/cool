#include "stdafx.h"
#include "../include/base/kfile.h"
#include "../include/base/kstringhelp.h"
#include <io.h>
#include <tchar.h>
#ifdef WIN32
#include <shlobj.h>
#endif

#define READ_LINE_MAXLEN		2048
unsigned char g_return_line_flag[4] = {0x0D, 0x00, 0x0A, 0x00};

KFile::KFile()
{
	m_file = 0;
	m_flagLen = 0;
	m_format = CodeFormat::cf_utf8;
}

KFile::~KFile()
{
	if (m_file)
		fclose(m_file);
}

bool KFile::Open(const wchar_t* filename, unsigned int openflag)
{
	Close();

	wchar_t szFlag[20] = {0};
	int flagIndex = 0;
	
	if (openflag & modeNoTruncate)
		szFlag[flagIndex++] = L'a';
	else if (openflag & modeWrite)
		szFlag[flagIndex++] = L'w';
	else
		szFlag[flagIndex++] = L'r';

	if (openflag & modeReadWrite || openflag & modeCreate)
		szFlag[flagIndex++] = L'+';	

	szFlag[flagIndex++] = L'b';
	//if (openflag & typeBinary)
	//	szFlag[flagIndex++] = L'b';
	//else if (openflag & typeText)
	//	szFlag[flagIndex++] = L't';

	m_file = _wfopen(filename, szFlag);
	if (0 == m_file)
		return false;

	m_filename = filename;

#ifdef WIN32
  if (openflag & (~typeBinary))
		_ParseFormat();
#endif
	SeekToBegin();

	return true;
}

bool KFile::Open(const char* filename, unsigned int openflag)
{
	Close();

	char szFlag[20] = {0};
	int flagIndex = 0;
	
	if (openflag & modeNoTruncate)
		szFlag[flagIndex++] = 'a';
	else if (openflag & modeWrite)
		szFlag[flagIndex++] = 'w';
	else
		szFlag[flagIndex++] = 'r';

	if (openflag & modeReadWrite || openflag & modeCreate)
		szFlag[flagIndex++] = '+';	

	szFlag[flagIndex++] = 'b';
	//if (openflag & typeBinary)
	//	szFlag[flagIndex++] = 'b';
	//else
	//	szFlag[flagIndex++] = 't';

	m_file = fopen(filename, szFlag);
	if (0 == m_file)
		return false;

	m_filename = filename;

#ifdef WIN32
	if (openflag & (~typeBinary))
		_ParseFormat();
#endif

	SeekToBegin();

	return true;
}

bool KFile::Open(KString& filename, unsigned int openflag)
{
	return Open(filename.c_str(), openflag);
}

bool KFile::IsOpened()
{
	return m_file != 0;
}

KString	KFile::GetFilename()
{
	return m_filename;
}

FILE* KFile::Detach()
{
	FILE* fp = m_file;
	m_file = 0;

	return fp;
}

void KFile::Attach(FILE* file)
{
	if (m_file)
		fclose(m_file);

	m_file = file;
}

void KFile::Close()
{
	if (m_file)
	{
		fclose(m_file);
		m_file = 0;
		m_format = CodeFormat::cf_ansi;
	}
}

static unsigned char utf8[4] = {0xEF, 0xBB, 0xBF, 0x00};
static unsigned char utf16[3] = {0xFF, 0xFE, 0x00};
static unsigned char utf16_big[3] = {0xFE, 0xFF, 0x00};

void KFile::_ParseFormat()
{
	m_format = cf_ansi;

	int buffer[4] = {0};
	int len = fread(buffer, 1, 3, m_file);
	fseek(m_file, 0, SEEK_SET);
	if (len == 0)
	{
#ifdef WIN32
		SetCodeFormat(cf_utf16);
#else
		SetCodeFormat(cf_ansi);
#endif
		return;
	}

	if (len < 2)
		return;

	if (memcmp(buffer, utf16, 2) == 0)
	{
		m_format = cf_utf16;
		m_flagLen = 2;
	}
	else if (memcmp(buffer, utf16_big, 2) == 0)
	{
		m_format = cf_utf16_be;
		m_flagLen = 2;
	}
	else if (memcmp(buffer, utf8, 3) == 0)
	{
		m_format = cf_utf8;
		m_flagLen = 3;
	}
	else
	{
		m_format = cf_ansi;
		m_flagLen = 0;
		fseek(m_file, 0, SEEK_SET);
	}
}

void UpdateBigToLittle(wchar_t* pStr)
{
	while(*pStr)
	{
		char* p = (char*)(pStr);
		char c = p[0];
		p[0] = p[1];
		p[1] = c;

		pStr++;
	}
}

int	KFile::ReadString(KStringW & strW)
{
	if (m_format == CodeFormat::cf_ansi)
	{
		char buf[READ_LINE_MAXLEN] = {0};
		if (0 == fgets(buf, READ_LINE_MAXLEN, m_file))
			return 0;
		strW = buf;
	}
	else if (m_format == CodeFormat::cf_utf8)
	{
		char buf[READ_LINE_MAXLEN] = {0};
		if (0 == fgets(buf, READ_LINE_MAXLEN, m_file))
			return 0;
		strW.SetUtf8String(buf);
	}
	else if (m_format == CodeFormat::cf_utf16)
	{
		wchar_t buf[READ_LINE_MAXLEN] = {0};
		if (0 == fgetws(buf, READ_LINE_MAXLEN, m_file))
			return 0;

		strW.SetData(buf);
	}
	else if (m_format == CodeFormat::cf_utf16_be)
	{
		wchar_t buf[READ_LINE_MAXLEN] = {0};
		if (0 == fgetws(buf, READ_LINE_MAXLEN, m_file))
			return 0;

		UpdateBigToLittle(buf);

		strW.SetData(buf);
	}

	if (strW.Right(2) == L"\r\n")
		strW = strW.Left(strW.GetLength() - 2);
	return strW.GetLength();
}

int	KFile::ReadString(KStringA & strA)
{
	if (m_format == CodeFormat::cf_ansi)
	{
		char buf[READ_LINE_MAXLEN] = {0};
		if (0 == fgets(buf, READ_LINE_MAXLEN, m_file))
			return 0;
		strA = buf;
	}
	else if (m_format == CodeFormat::cf_utf8)
	{
		char buf[READ_LINE_MAXLEN] = {0};
		if (0 == fgets(buf, READ_LINE_MAXLEN, m_file))
			return 0;
		strA.SetData(buf);
	}
	else if (m_format == CodeFormat::cf_utf16)
	{
		wchar_t buf[READ_LINE_MAXLEN] = {0};
		if (0 == fgetws(buf, READ_LINE_MAXLEN, m_file))
			return 0;

		strA.SetData(buf);
	}
	else if (m_format == cf_utf16_be)
	{
		wchar_t buf[READ_LINE_MAXLEN] = {0};
		if (0 == fgetws(buf, READ_LINE_MAXLEN, m_file))
			return 0;
		UpdateBigToLittle(buf);

		strA.SetData(buf);
	}

	return strA.GetLength();
}

int	KFile::WriteString(KStringW strW)
{
	return WriteString(strW.c_str(), strW.GetLength());
}

int	KFile::WriteString(KStringA strA)
{
	return WriteString(strA.c_str(), strA.GetLength());
}

int	KFile::WriteString(const wchar_t* str, int len)
{
	if (0 == str || 0 == len)
		return 0;

	if (len < 0)
		len = wcslen(str);

	int ret = 0;
	if (m_format == CodeFormat::cf_utf16)
	{
		ret = Write(str, len * sizeof(wchar_t));
		ret += Write(g_return_line_flag, 4);
	}
	else if (m_format == CodeFormat::cf_ansi)
	{
		KStringA strA;
		strA.SetData(str, len);
		strA += "\r\n";
		ret = Write(strA.c_str(), strA.GetLength());
	}
	else if (m_format == CodeFormat::cf_utf8)
	{
		KStringA strA = KStringW2Utf8(str, len);
		strA += "\r\n";
		ret = Write(strA.c_str(), strA.GetLength());
	}
	else if (m_format == CodeFormat::cf_utf16_be)
	{
		KStringW strW;
		strW.SetData(str, len);
		strW += L"\r\n";
		
		wchar_t* pd = strW.GetBuffer();
		UpdateBigToLittle(pd);

		ret = Write(pd, strW.GetLength() * sizeof(wchar_t));
	}

	return ret;
}

int	KFile::WriteString(const char* str, int len)
{
	if (0 == str || 0 == len)
		return 0;

	if (len < 0)
		len = strlen(str);

	int ret = 0;
	if (m_format == CodeFormat::cf_ansi || m_format == CodeFormat::cf_utf8)
	{
		ret = Write(str, len);
		ret += Write("\r\n", 2);
	}
	else if (m_format == CodeFormat::cf_utf16)
	{
		KStringW strW;
		strW.SetData(str, len);
		strW += L"\r\n";
		ret = Write(strW.c_str(), strW.GetLength() * sizeof(wchar_t));
	}
	else if (m_format == CodeFormat::cf_utf16_be)
	{
		KStringW strW;
		strW.SetData(str, len);
		strW += L"\r\n";

		wchar_t* pd = strW.GetBuffer();
		UpdateBigToLittle(pd);

		ret = Write(pd, strW.GetLength() * sizeof(wchar_t));
	}

	return ret;
}

int	KFile::GetLength()
{
	long tmp = ftell(m_file);
	fseek(m_file, 0, SEEK_END);

	long ret = ftell(m_file);
	fseek(m_file, tmp, SEEK_END);

	return ret;
}

int	KFile::Seek(int offset, int startPos)
{
	return fseek(m_file, offset, startPos);
}

int	KFile::SeekToBegin()
{
	int ret = fseek(m_file, m_flagLen, SEEK_SET);
	//if (m_format == cf_utf8)
	//{
	//	fseek(m_file, 3, SEEK_SET);
	//}
	//else if (m_format == cf_utf16 || m_format == cf_utf16_be)
	//{
	//	fseek(m_file, 2, SEEK_SET);
	//}
	return ret;
}

int	KFile::SeekToEnd()
{
	return fseek(m_file, 0, SEEK_END);
}

int	KFile::Write(const void* data, int len)
{
	return fwrite(data, 1, len, m_file);
}

int	KFile::Read(void* data, int len)
{
	return fread(data, 1, len, m_file);
}

KFile::CodeFormat KFile::GetCodeFormat()
{
	return m_format;
}

void KFile::SetCodeFormat(KFile::CodeFormat cf)
{
	fseek(m_file, 0, SEEK_SET);
	
	m_format = cf;
	if (m_format == CodeFormat::cf_utf8)
	{
		fwrite(utf8, 1, 3, m_file);
	}
	else if (m_format == CodeFormat::cf_utf16)
	{
		fwrite(utf16, 1, 2, m_file);
	}
	else if (m_format == CodeFormat::cf_utf16_be)
	{
		fwrite(utf16_big, 1, 2, m_file);
	}
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

