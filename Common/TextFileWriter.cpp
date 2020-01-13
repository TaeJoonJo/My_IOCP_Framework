#include "TextFileWriter.h"

CTextFileWriter::CTextFileWriter(const char* path)
	: FilePath_(path)
{
	FileLock_.Lock();
	{
		OS_.open(FilePath_, std::ios::out | std::ios::app);
	}
	FileLock_.UnLock();
}

CTextFileWriter::~CTextFileWriter()
{
	FileLock_.Lock();
	{
		if (true == OS_.is_open()) {
			OS_.close();
		}
	}
	FileLock_.UnLock();
}

const bool CTextFileWriter::ChangeFilePath(const char* path)
{
	if (nullptr == path) {
		return false;
	}

	FileLock_.Lock();
	{
		if (true == OS_.is_open())
			OS_.close();
		OS_.open(FilePath_, std::ios::out | std::ios::app);
	}
	FileLock_.UnLock();

	return true;
}

const bool CTextFileWriter::Write(const char* str)
{
	FileLock_.Lock();
	{
		if (false == OS_.is_open()) {
			FileLock_.UnLock();
			return false;
		}
		OS_ << str;
	}
	FileLock_.UnLock();

	return true;
}
