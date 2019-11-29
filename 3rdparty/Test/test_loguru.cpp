#include "loguru.hpp"

int main(int argc, char** argv)
{
    loguru::init(argc, argv);
    // Put every log message in "everything.log":
    loguru::add_file("/tmp/everything.log", loguru::Append, loguru::Verbosity_MAX);

    // Only log INFO, WARNING, ERROR and FATAL to "latest_readable.log":
    loguru::add_file("/tmp/latest_readable.log", loguru::Truncate, loguru::Verbosity_INFO);

    {
        LOG_SCOPE_F(INFO, "This is a scope");
        LOG_F(INFO, "I'm hungry for some {0}!", 3.14159);
        LOG_F(ERROR, "In Scope");
    }
    LOG_F(INFO, "Other logmessage");
    DCHECK_F(false); // Only checked #if !NDEBUG
    DLOG_F(INFO, "Only written in debug-builds");

    return 0;
}