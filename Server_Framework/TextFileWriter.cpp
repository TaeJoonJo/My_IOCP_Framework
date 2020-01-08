#include "TextFileWriter.h"

CTextFileWriter::CTextFileWriter(const char* path)
	: filePath_(path)
{
	fileLock_.Lock();
	oS_.open(filePath_, std::ios::out | std::ios::app);
	fileLock_.UnLock();
}

CTextFileWriter::~CTextFileWriter()
{
	fileLock_.Lock();
	if (true == oS_.is_open())
		oS_.close();
	fileLock_.UnLock();
}

const bool CTextFileWriter::ChangeFilePath(const char* path)
{
	if (nullptr == path)
		return false;

	fileLock_.Lock();
	if (true == oS_.is_open())
		oS_.close();
	oS_.open(filePath_, std::ios::out | std::ios::app);
	fileLock_.UnLock();

	return true;
}

const bool CTextFileWriter::Write(const char* str)
{
	fileLock_.Lock();
	if (false == oS_.is_open()) {
		fileLock_.UnLock();
		return false;
	}

	oS_ << str;
	fileLock_.UnLock();

	return true;
}
