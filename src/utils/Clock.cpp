#include "../pch.h"
#include "Clock.h"

Clock::Clock(float seconds)
    : m_seconds(seconds)
{
}

static std::string numberToString(long number) {
    return (number < 10 ? "0" : "") + std::to_string(number);
}

std::string Clock::toString() const
{
    long seconds = (long)m_seconds % 60;
    long minutes = ((long)m_seconds / 60) % 60;
    long hours = ((long)m_seconds / 3600) % 24;

    std::string hoursString = (hours < 10 ? "0" : "") + std::to_string(hours);

    return numberToString(hours) + ":"  + numberToString(minutes) + ":" + numberToString(seconds);
}

float Clock::getSeconds() const
{
    return m_seconds;
}

Clock &Clock::operator+=(float time)
{
    m_seconds += time;
    return *this;
}
