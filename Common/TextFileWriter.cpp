#include "TextFileWriter.h"

CTextFileWriter::CTextFileWriter(const char* path)
	: FilePath_(path)
{
	_BEGINLOCKGUARD(FileLock_.GetMutex())
	
		OS_.open(FilePath_, std::ios::out | std::ios::app);
	
	_ENDLOCKGUARD
}

CTextFileWriter::~CTextFileWriter()
{
	_BEGINLOCKGUARD(FileLock_.GetMutex())
	
		if (true == OS_.is_open()) {
			OS_.close();
		}
	
	_ENDLOCKGUARD
}

const bool CTextFileWriter::ChangeFilePath(const char* path)
{
	if (nullptr == path) {
		return false;
	}

	_BEGINLOCKGUARD(FileLock_.GetMutex())
	
		if (true == OS_.is_open())
			OS_.close();
		OS_.open(FilePath_, std::ios::out | std::ios::app);
	
	_ENDLOCKGUARD

	return true;
}

const bool CTextFileWriter::Write(const char* str)
{
	_BEGINLOCKGUARD(FileLock_.GetMutex())
	
		if (false == OS_.is_open()) {
			FileLock_.UnLock();
			return false;
		}
		OS_ << str;
	
	_ENDLOCKGUARD

	return true;
}
