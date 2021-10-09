_Pragma("once");

#include <string>
#include <chrono>
#include <memory>

#include <cstring>
#include <cstdio>
#include <cstdarg>

namespace test_log
{

enum class ELogLevel: uint8_t
{
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_MAX
};

class CLog final
{
public:
    static CLog & GetInstance()
    {
        static CLog instance;
        return instance;
    }

    CLog(const CLog &) = delete;
    CLog &operator=(const CLog &) = delete;

    void SetMaxLogLevel(const ELogLevel &level) { max_log_level_ = level;}
    ELogLevel GetMaxLogLevel() const { return max_log_level_;}

    bool Log(const char *fmt, ...) const
    {
        int log_size = 0;
        std::shared_ptr<char> log_buff;
        va_list ap;

        /* Determine required size */
        va_start(ap, fmt);
        log_size = vsnprintf(log_buff.get(), log_size, fmt, ap);
        va_end(ap);

        if(log_size <= 0)
            return false;

        log_size++; /* For '\0' */
        log_buff.reset(new char[log_size], std::default_delete<char[]>());
        if(!log_buff)
            return false;
        
        va_start(ap, fmt);
        log_size = vsnprintf(log_buff.get(), log_size, fmt, ap);
        va_end(ap);

        printf("%s", log_buff.get());
        return true;
    }

private:
    CLog() = default;
    ~CLog() = default;

private:
    ELogLevel max_log_level_{ELogLevel::LOG_LEVEL_DEBUG};
};

static const std::string GetDateTime()
{
    std::string data_time;

    struct tm tim;
    const auto timep = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
#if defined(OS_WIN) && (OS_WIN == 1)
	const auto ret = localtime_s(&tim, &timep);
	if (ret != 0)
        return data_time;
#else
	const auto tm_pla = std::localtime(&timep);
	if (!tm_pla)
        return data_time;
	tim = *tm_pla;
#endif
    
    char buf[32] = {0};
	snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d", tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday, tim.tm_hour, tim.tm_min, tim.tm_sec);
	buf[sizeof(buf) - 1] = '\0';
    data_time = buf;
    return data_time;
}

#define LOG(level) LOG_##level

#define LOG_DEBUG(fmt, ...) \
    if(CLog::GetInstance().GetMaxLogLevel() <= ELogLevel::LOG_LEVEL_DEBUG) \
        CLog::GetInstance().Log("[imsdk %s] (%s:%s:%d DEBUG) " fmt "\n", GetDateTime().c_str(), __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOG_INFO(fmt, ...) \
    if(CLog::GetInstance().GetMaxLogLevel() <= ELogLevel::LOG_LEVEL_INFO) \
        CLog::GetInstance().Log("[imsdk %s] (%s:%s:%d \033[01;34mINFO\033[00m) " fmt "\n", GetDateTime().c_str(), __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOG_WARN(fmt, ...) \
    if(CLog::GetInstance().GetMaxLogLevel() <= ELogLevel::LOG_LEVEL_WARN) \
        CLog::GetInstance().Log("[imsdk %s] (%s:%s:%d \033[01;33mWARN\033[00m) " fmt "\n", GetDateTime().c_str(), __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOG_ERROR(fmt, ...) \
    if(CLog::GetInstance().GetMaxLogLevel() <= ELogLevel::LOG_LEVEL_ERROR) \
        CLog::GetInstance().Log("[imsdk %s] (%s:%s:%d \033[01;31mERROR\033[00m) " fmt "\n", GetDateTime().c_str(), __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOG_FATAL(fmt, ...) \
    if(CLog::GetInstance().GetMaxLogLevel() <= ELogLevel::LOG_LEVEL_FATAL) \
        CLog::GetInstance().Log("[imsdk %s] (%s:%s:%d \033[01;31mFATAL\033[00m) " fmt "\n", GetDateTime().c_str(), __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);

}//end of namespace test_log