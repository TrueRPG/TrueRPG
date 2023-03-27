#ifndef RPG_LOGGER_H
#define RPG_LOGGER_H

#include <iostream>
#include <chrono>
#include <iomanip>
#include "Macros.h"

namespace logger
{
#if !defined(RPG_DEBUG) && !defined(USE_DEBUGLOG)
constexpr bool LOG_DEBUG_ENABLED = false;
#else
constexpr bool LOG_DEBUG_ENABLED = true;
#endif

template<typename ...Args>
inline void log(Args &&...args)
{
    using clock = std::conditional_t<std::chrono::system_clock::is_steady,
        std::chrono::steady_clock,
        std::chrono::system_clock>;

    const auto now = clock::to_time_t(clock::now());

    std::clog << "\033[0;32m[" << std::put_time(std::gmtime(&now), "%x %X") << "]\033[0m";
    ((std::clog << args), ... );
    std::clog << std::endl;
}

template<typename ...Args>
inline void info(Args &&...args)
{
    log("\033[0;32m[INFO]\033[0m ", args...);
}

template<typename ...Args>
inline void debug(Args &&...args)
{
    if constexpr (LOG_DEBUG_ENABLED)
        log("\033[0;32m[DEBUG]\033[0m ", args...);
}

template<typename ...Args>
inline void warning(Args &&...args)
{
    if constexpr (LOG_DEBUG_ENABLED)
        log("\033[0;33m[WARN]\033[0m ", args...);
}

template<typename ...Args>
inline void error(Args &&...args)
{
    if constexpr (LOG_DEBUG_ENABLED)
        log("\033[0;31m[ERROR]\033[0m ", args...);
}

}

#endif // RPG_LOGGER_H
