#ifndef RPG_CLOCK_H
#define RPG_CLOCK_H

class Clock
{
    float m_seconds{};

public:
    explicit Clock(float time = 0);

    std::string toString() const;

    float getSeconds() const;

    Clock& operator+= (float time);
};

#endif // RPG_CLOCK_H
