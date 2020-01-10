#include "LogSystem.h"

#include <iomanip>
#include <sstream>
#include "TextFileWriter.h"

CLogSystem::CLogSystem() 
    : fileName_()
    , maxCharLength_(0)
    , unknownLogType_("UNKNOWN") 
    , fileWriter_(nullptr)
{
    logTypes_[CLogSystem::ELogType::INFO_] = "INFO";
    logTypes_[CLogSystem::ELogType::WARNING_] = "WARNING";
    logTypes_[CLogSystem::ELogType::ERROR_] = "ERROR";

    maxCharLength_ = static_cast<uint32_t>(unknownLogType_.size());
    for (const std::string& logType : logTypes_) {
        if (maxCharLength_ < logType.size()) {
            maxCharLength_ = static_cast<uint32_t>(logType.size());
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
    oss << std::setfill(' ') << std::setw(plogger->maxCharLength_);

    try {
        oss << plogger->logTypes_.at(type);
    }
    catch (...) {
        oss << plogger->unknownLogType_;
    }

    SYSTEMTIME st;
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
    
    if (false == plogger->fileWriter_->Write(oss.str().c_str())) {

    }
}

const bool CLogSystem::Initalize(const char* path)
{
    fileName_ = path;

    static CTextFileWriter textWriter{ path };
    fileWriter_ = &textWriter;

    SYSTEMTIME st;
    GetLocalTime(&st);

    std::ostringstream oss;

    oss << '\n';

    oss << " [" << st.wYear << '.'
        << std::setfill('0') << std::setw(2) << st.wMonth << '.'
        << std::setfill('0') << std::setw(2) << st.wDay << ']';

    oss << '\n' << '\n';

    fileWriter_->Write(oss.str().c_str());

    return true;
}

void CLogSystem::Info(const char* str, ...)
{
    CLogSystem::Log(str, CLogSystem::ELogType::INFO_);
}

void CLogSystem::Warning(const char* str, ...)
{
    CLogSystem::Log(str, CLogSystem::ELogType::WARNING_);
}

void CLogSystem::Error(const char* str, ...)
{
    CLogSystem::Log(str, CLogSystem::ELogType::ERROR_);
}
