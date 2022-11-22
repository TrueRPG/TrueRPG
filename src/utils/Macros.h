#ifndef RPG_MACROS_H
#define RPG_MACROS_H

#ifndef NDEBUG
#define DEBUG
constexpr bool DEBUG_ENABLED = true;
#else
constexpr bool DEBUG_ENABLED = false;
#endif

#endif // RPG_MACROS_H
