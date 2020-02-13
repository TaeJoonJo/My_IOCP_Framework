
#ifndef __LOGSYSTEM_H__
#define __LOGSYSTEM_H__

#include "../Include/Headers.h"

#include "Singleton.h"

#define DEFALUT_LOG_PATH "..\serverlog.log"

constexpr uint8_t LOG_TYPE_NUM = 3;

class CTextFileWriter;

class CLogSystem :
    public Singleton::CDynamicSingleton<CLogSystem>
{
public:
    enum ELogType {
        INFO_ = 0,
        WARNING_,
        ERROR_
    };
public:
    explicit CLogSystem();
    ~CLogSystem();
    CLogSystem(const CLogSystem& c) = delete;
    CLogSystem& operator=(const CLogSystem& c) = delete;
public:
    const bool Initalize(const char* path);

    static void Log(const std::string& str, ELogType type = INFO_);
    static void Log(const std::ostringstream& oss, ELogType type = INFO_);
    static void Log(const char* str, ELogType type = INFO_);
    static void Info(const char* str, ...);
    static void Warning(const char* str, ...);
    static void Error(const char* str, ...);
private:
    std::string                             FileName_;
    uint32_t                                MaxCharLength_;

    std::array<std::string, LOG_TYPE_NUM>   LogTypes_;
    const std::string                       UnknownLogType_;
public:
    CTextFileWriter*                        FileWriter_;
};

#define _ASSEMBLESTRING     va_list vl;\
                            char tmpStr[50]{};\
                            va_start(vl, str);\
                            vsprintf(tmpStr, str, vl);\
                            va_end(vl);\
                            str = tmpStr;

#define _LINFO(str, ...)  CLogSystem::Info(str, __VA_ARGS__);
#define _LWARNING(str, ...)  CLogSystem::Warning(str, __VA_ARGS__);
#define _LERROR(str, ...)  CLogSystem::Error(str, __VA_ARGS__);

#endif // !__LOGSYSTEM_H__
