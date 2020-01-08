
#ifndef __TEXTFILEWRITER_H__
#define __TEXTFILEWRITER_H__

#include "Headers.h"
#include <fstream>
#include "Lock.h"

class CTextFileWriter
{
public:
	explicit CTextFileWriter(const char* path);
	~CTextFileWriter();
	CTextFileWriter(const CTextFileWriter& o) = delete;
	CTextFileWriter& operator=(const CTextFileWriter& o) = delete;
public:
	const bool ChangeFilePath(const char* path);
	const bool Write(const char* str);
private:
	std::string filePath_;
	CMutexLock fileLock_;
	std::ofstream oS_;
};

#endif // !__TEXTFILEWRITER_H__
