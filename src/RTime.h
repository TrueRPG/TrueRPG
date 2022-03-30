#include <GLFW/glfw3.h>

#define MAX_DELTA_TIME 0.4

class RTime
{
    float m_prevFrameTime;
    float m_frameTime;
    float m_deltaTime;
public:
    RTime(float prevFrameTime, float frameTime, float deltaTime) :
            m_prevFrameTime(prevFrameTime),
            m_frameTime(frameTime),
            m_deltaTime(deltaTime)
    { }

    void setNewFrameTime()
    {
        m_frameTime = (float) glfwGetTime();
        m_deltaTime = m_frameTime - m_prevFrameTime;
        m_prevFrameTime = m_frameTime;
    }

    float getDeltaTime()
    {   
        setNewFrameTime();
        if (m_deltaTime > MAX_DELTA_TIME) {
            return MAX_DELTA_TIME;
        }
        return m_deltaTime;
    }
};
