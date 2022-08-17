#include "../pch.h"

#include "DayNightCycle.h"

float DayNightCycle::computeSunBrightness(float seconds)
{
    // the formula of the day-night cycle:
    // brightness = (tanh(10 * sin(pi / 43200 * (x - 23000)) + 3.2) + 1) / 2
    return (std::tanh(10.f * std::sin(glm::pi<float>() / 43200.f * (seconds - 23000.f)) + 3.2f) + 1.f) / 2.f;
}
