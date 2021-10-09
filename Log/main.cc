#include "log.h"

using namespace test_log;

void test()
{
    CLog::GetInstance().SetMaxLogLevel(ELogLevel::LOG_LEVEL_DEBUG);

    LOG(DEBUG)("%s", "this is debug");
    LOG(INFO)("%s", "this is info");
    LOG(WARN)("%s", "this is warn");
    LOG(ERROR)("%s", "this is error");
    LOG(FATAL)("%s", "this is fatal");

    return;
}

int main()
{
    test();
    return 0;
}