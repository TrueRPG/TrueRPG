//
// Created by archweeb on 11/19/22.
//

#ifndef RPG_LOGGER_H
#define RPG_LOGGER_H

#include <iostream>
namespace logger
{

template<typename ...Args>
inline void log(Args &&...args)
{
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
    log("\033[0;32m[DEBUG]\033[0m ", args...);
}

template<typename ...Args>
inline void warning(Args &&...args)
{
    log("\033[0;33m[WARN]\033[0m ", args...);
}

template<typename ...Args>
inline void error(Args &&...args)
{
    log("\033[0;31m[ERROR]\033[0m ", args...);
}

}

#endif // RPG_LOGGER_H
