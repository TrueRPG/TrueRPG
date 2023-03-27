#include "../client/graphics/Graphics.h"

// TODO: delete this and add fixed update
#define MAX_DELTA_TIME 0.4

class GameTimer
{
    float m_deltaTime;

    using clock = std::conditional_t<std::chrono::system_clock::is_steady,
        std::chrono::steady_clock,
        std::chrono::system_clock>;

    clock::time_point m_offset;

public:
    explicit inline GameTimer(clock::time_point start = clock::now()) : m_offset(start), m_deltaTime(0.0f)
    {}

    inline float getDeltaTime()
    {
        setNewFrameTime();
        if (m_deltaTime > MAX_DELTA_TIME)
        {
            return MAX_DELTA_TIME;
        }
        return m_deltaTime;
    }

private:
    inline void setNewFrameTime()
    {
        const clock::time_point now = clock::now();
        m_deltaTime = std::chrono::duration<float>(now - m_offset).count();
        m_offset = now;
    }
};
