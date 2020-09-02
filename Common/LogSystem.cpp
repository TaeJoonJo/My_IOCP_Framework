#include "LogSystem.h"

#include <iomanip>
#include <sstream>
#include "TextFileWriter.h"

CLogSystem::CLogSystem() 
    : FileName_()
    , MaxCharLength_(0)
    , UnknownLogType_("UNKNOWN") 
    , FileWriter_(nullptr)
{
    LogTypes_[CLogSystem::ELogType::INFO_] = "INFO";
    LogTypes_[CLogSystem::ELogType::WARNING_] = "WARNING";
    LogTypes_[CLogSystem::ELogType::ERROR_] = "ERROR";

    MaxCharLength_ = static_cast<uint32_t>(UnknownLogType_.size());
    for (const std::string& logType : LogTypes_) {
        if (MaxCharLength_ < logType.size()) {
            MaxCharLength_ = static_cast<uint32_t>(logType.size());
        }
    }
}

CLogSystem::~CLogSystem() 
{
    
}

void CLogSystem::Log(const std::string& str, ELogType type)
{
    Log(str.c_str(), type);
}

void CLogSystem::Log(const std::ostringstream& oss, ELogType type)
{
    Log(oss.str().c_str(), type);
}

void CLogSystem::Log(const char* text, ELogType type)
{
    static auto plogger = CLogSystem::GetInstance();
    
    std::ostringstream oss;
    oss << std::setfill(' ') << std::setw(plogger->MaxCharLength_);

    try {
        oss << plogger->LogTypes_.at(type);
    }
    catch (...) {
        oss << plogger->UnknownLogType_;
    }

    SYSTEMTIME st{};
    GetLocalTime(&st);

    oss << " [" 
        << std::setfill(' ') << std::setw(2) << st.wHour << ':'
        << std::setfill('0') << std::setw(2) << st.wMinute << ':'
        << std::setfill('0') << std::setw(2) << st.wSecond << '.'
        << std::setfill('0') << std::setw(3) << st.wMilliseconds << "] ";

    oss << text << '\n';

#ifdef _DEBUG
    puts(oss.str().c_str());
#endif
    
    if (false == plogger->FileWriter_->Write(oss.str().c_str())) {

    }
}

const bool CLogSystem::Initalize(const char* path)
{
    FileName_ = path;

    static CTextFileWriter textWriter{ path };
    FileWriter_ = &textWriter;

    SYSTEMTIME st;
    GetLocalTime(&st);

    std::ostringstream oss;

    oss << '\n';

    oss << " [" << st.wYear << '.'
        << std::setfill('0') << std::setw(2) << st.wMonth << '.'
        << std::setfill('0') << std::setw(2) << st.wDay << ']';

    oss << '\n' << '\n';

    FileWriter_->Write(oss.str().c_str());

    return true;
}

void CLogSystem::Info(const char* str, ...)
{
    _ASSEMBLESTRING;

    CLogSystem::Log(str, CLogSystem::ELogType::INFO_);
}

void CLogSystem::Warning(const char* str, ...)
{
    _ASSEMBLESTRING;

    CLogSystem::Log(str, CLogSystem::ELogType::WARNING_);
}

void CLogSystem::Error(const char* str, ...)
{
    _ASSEMBLESTRING;

    CLogSystem::Log(str, CLogSystem::ELogType::ERROR_);
}
