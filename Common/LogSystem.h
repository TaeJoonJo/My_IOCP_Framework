
#ifndef __LOGSYSTEM_H__
#define __LOGSYSTEM_H__

#include "Singleton.h"
#include "../Include/Headers.h"

#define DEFALUT_LOG_PATH "serverlog.log"

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
    std::string fileName_;
    uint32_t maxCharLength_;

    std::array<std::string, LOG_TYPE_NUM> logTypes_;
    const std::string unknownLogType_;
public:
    CTextFileWriter* fileWriter_;
};

#define _LINFO(str, ...) CLogSystem::Info(str);
#define _LWARNING(str, ...) CLogSystem::Warning(str);
#define _LERROR(str, ...) CLogSystem::Error(str);

#endif // !__LOGSYSTEM_H__
