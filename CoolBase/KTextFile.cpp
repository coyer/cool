#include "stdafx.h"
#include "../include/base/kfile.h"
#include "../include/base/kstringhelp.h"
#include <io.h>
#include <tchar.h>
#ifdef WIN32
#include <shlobj.h>
#endif

#define READ_LINE_MAXLEN		2048

KTextFile::KTextFile()
{
	m_file = 0;
	m_flagLen = 0;
	m_fileSize = 0;
	m_format = CodeFormat::cf_utf8;
}

KTextFile::~KTextFile()
{
	if (m_file)
		fclose(m_file);
}

bool KTextFile::Open(const wchar_t* filename, bool writemode)
{
	if (!filename) return false;
	KString str = filename;
	return Open(str, writemode);
}

bool KTextFile::Open(const char* filename, bool writemode)
{
	if (!filename) return false;
	KString str = filename;
	return Open(str, writemode);
}

bool KTextFile::Open(const KString& filename, bool writemode)
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

#ifdef WIN32
	_ParseFormat();
#endif

	m_fileSize = filelength(fileno(m_file));
	SeekToBegin();

	return true;
}

bool KTextFile::Create(const char* filename, CodeFormat fmt)
{
	KString str = filename;
	return Create(str, fmt);
}

bool KTextFile::Create(const wchar_t* filename, CodeFormat fmt)
{
	KString str = filename;
	return Create(str, fmt);
}

bool KTextFile::Create(const KString& filename, CodeFormat fmt)
{
	Close();

	KString sFlag = _T("w+b");

	m_file = _tfopen(filename, sFlag.c_str());
	if (0 == m_file)
		return false;

	m_filename = filename;
	m_format = fmt;
	_WriteFormat();

	return true;
}

bool KTextFile::IsOpened()
{
	return m_file != 0;
}

KString	KTextFile::GetFilename()
{
	return m_filename;
}

__int64 KTextFile::GetFileSize()
{
	return m_fileSize;
}

void KTextFile::Close()
{
	if (m_file)
	{
		fclose(m_file);
		m_file = 0;
		m_format = CodeFormat::cf_ansi;
	}
}

static unsigned char utf8[4] = { 0xEF, 0xBB, 0xBF, 0x00 };
static unsigned char utf16[3] = { 0xFF, 0xFE, 0x00 };
static unsigned char utf16_big[3] = { 0xFE, 0xFF, 0x00 };

void KTextFile::_ParseFormat()
{
	m_format = cf_ansi;

	int buffer[4] = { 0 };
	int len = fread(buffer, 1, 3, m_file);
	fseek(m_file, 0 - len, SEEK_CUR);

	if (len < 2) {
		return;
	}
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
	}
}

int	KTextFile::_WriteFormat()
{
	if (!m_file) return 0;
	fseek(m_file, 0, SEEK_SET);
	if (m_format == CodeFormat::cf_utf8)
	{
		return fwrite(utf8, 1, 3, m_file);
	}
	else if (m_format == CodeFormat::cf_utf16)
	{
		return fwrite(utf16, 1, 2, m_file);
	}
	else if (m_format == CodeFormat::cf_utf16_be)
	{
		return fwrite(utf16_big, 1, 2, m_file);
	}
	return 0;
}

void UpdateBigToLittle(wchar_t* pStr)
{
	while (*pStr)
	{
		char* p = (char*)(pStr);
		char c = p[0];
		p[0] = p[1];
		p[1] = c;

		pStr++;
	}
}

int	KTextFile::ReadString(KStringW& strW)
{
	if (m_format == CodeFormat::cf_ansi)
	{
		char buf[READ_LINE_MAXLEN] = { 0 };
		if (0 == fgets(buf, READ_LINE_MAXLEN, m_file))
			return 0;
		strW = buf;
	}
	else if (m_format == CodeFormat::cf_utf8)
	{
		char buf[READ_LINE_MAXLEN] = { 0 };
		if (0 == fgets(buf, READ_LINE_MAXLEN, m_file))
			return 0;
		strW.SetUtf8String(buf);
	}
	else if (m_format == CodeFormat::cf_utf16)
	{
		wchar_t buf[READ_LINE_MAXLEN] = { 0 };
		if (0 == fgetws(buf, READ_LINE_MAXLEN, m_file))
			return 0;

		strW.SetData(buf);
	}
	else if (m_format == CodeFormat::cf_utf16_be)
	{
		wchar_t buf[READ_LINE_MAXLEN] = { 0 };
		if (0 == fgetws(buf, READ_LINE_MAXLEN, m_file))
			return 0;

		UpdateBigToLittle(buf);

		strW.SetData(buf);
	}

	if (strW.Right(2) == L"\r\n")
		strW = strW.Left(strW.GetLength() - 2);
	return strW.GetLength();
}

int	KTextFile::ReadString(KStringA& strA)
{
	if (m_format == CodeFormat::cf_ansi)
	{
		char buf[READ_LINE_MAXLEN] = { 0 };
		if (0 == fgets(buf, READ_LINE_MAXLEN, m_file))
			return 0;
		strA = buf;
	}
	else if (m_format == CodeFormat::cf_utf8)
	{
		char buf[READ_LINE_MAXLEN] = { 0 };
		if (0 == fgets(buf, READ_LINE_MAXLEN, m_file))
			return 0;
		strA.SetData(buf);
	}
	else if (m_format == CodeFormat::cf_utf16)
	{
		wchar_t buf[READ_LINE_MAXLEN] = { 0 };
		if (0 == fgetws(buf, READ_LINE_MAXLEN, m_file))
			return 0;

		strA.SetData(buf);
	}
	else if (m_format == cf_utf16_be)
	{
		wchar_t buf[READ_LINE_MAXLEN] = { 0 };
		if (0 == fgetws(buf, READ_LINE_MAXLEN, m_file))
			return 0;
		UpdateBigToLittle(buf);

		strA.SetData(buf);
	}

	return strA.GetLength();
}

int	KTextFile::WriteString(KStringW strW)
{
	return WriteString(strW.c_str(), strW.GetLength());
}

int	KTextFile::WriteString(KStringA strA)
{
	return WriteString(strA.c_str(), strA.GetLength());
}

int	KTextFile::WriteString(const wchar_t* str, int len)
{
	if (0 == str || 0 == len)
		return 0;

	if (len <= 0)
		len = wcslen(str);

	int ret = 0;
	if (m_format == CodeFormat::cf_utf16)
	{
		ret = Write(str, len * sizeof(wchar_t));
		ret += Write(L"\r\n", sizeof(wchar_t) * 2);
	}
	else if (m_format == CodeFormat::cf_utf8)
	{
		KStringA strA = KStringW2Utf8(str, len);
		ret = Write(strA.c_str(), strA.GetLength());
		ret += Write("\r\n", 2);
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
	else {
		KStringA strA;
		strA.SetData(str, len);
		ret = Write(strA.c_str(), strA.GetLength());
		ret += Write("\r\n", 2);
	}

	return ret;
}

int	KTextFile::WriteString(const char* str, int len)
{
	if (0 == str || 0 == len)
		return 0;

	if (len < 0)
		len = strlen(str);

	int ret = 0;
	if (m_format == CodeFormat::cf_utf16)
	{
		KStringW strW;
		strW.SetData(str, len);
		ret = Write(strW.c_str(), strW.GetLength() * sizeof(wchar_t));
		ret += Write(L"\r\n", 2 * sizeof(wchar_t));
	}
	else if (m_format == CodeFormat::cf_utf16_be)
	{
		KStringW strW;
		strW.SetData(str, len);

		wchar_t* pd = strW.GetBuffer();
		UpdateBigToLittle(pd);

		ret = Write(pd, strW.GetLength() * sizeof(wchar_t));
		ret += Write(L"\n\r", 2 * sizeof(wchar_t));
	}
	else {
		ret = Write(str, len);
		ret += Write("\r\n", 2);
	}

	return ret;
}

__int64	KTextFile::ReadFileLength()
{
	long tmp = ftell(m_file);
	fseek(m_file, 0, SEEK_END);

	long ret = ftell(m_file);
	fseek(m_file, tmp, SEEK_SET);

	return ret;
}

int	KTextFile::Seek(int offset, int startPos)
{
	return fseek(m_file, offset, startPos);
}

int	KTextFile::SeekToBegin()
{
	int ret = fseek(m_file, m_flagLen, SEEK_SET);
	return ret;
}

int	KTextFile::SeekToEnd()
{
	return fseek(m_file, 0, SEEK_END);
}

int	KTextFile::Write(const void* data, int len, int type_size)
{
	return fwrite(data, type_size, len, m_file);
}

int	KTextFile::Read(void* data, int len, int type_size)
{
	return fread(data, type_size, len, m_file);
}

bool KTextFile::ReadAllText(KString& strOut)
{
	if (!IsOpened()) return false;
	if (m_fileSize == 0) return true;

	long tmp = ftell(m_file);
	
	SeekToBegin();
	char* buffer = new char(m_fileSize + 2);
	memset(buffer, 0, m_fileSize + 2);

	int readlen = Read(buffer, m_fileSize);
	if (readlen == 0) return true;

	if (m_format == cf_utf16) 
	{
		wchar_t* p = (wchar_t*)buffer;
		strOut.SetData(p, readlen / 2 );
	}
	else if (m_format == cf_utf16_be)
	{
		wchar_t* p = (wchar_t*)buffer;
		UpdateBigToLittle(p);
		strOut.SetData(p, readlen / 2);
	}
	else if (m_format == cf_utf8) {
		KStringA str;
		str.SetData(buffer, readlen);
		strOut = KStringFromUtf8(str);
	}
	else {
		KStringA str;
		str.SetData(buffer, readlen);
		strOut = str;
	}

	return true;
}

KTextFile::CodeFormat KTextFile::GetCodeFormat()
{
	return m_format;
}

void KTextFile::SetCodeFormat(KTextFile::CodeFormat cf)
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

/////////////////////////////////////////////////////////////////////////////////////////////

