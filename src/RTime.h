#ifndef RPG_RTIME_H
#define RPG_RTIME_H

#include <GLFW/glfw3.h>

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
        m_frameTime = glfwGetTime();
        m_deltaTime = m_frameTime - m_prevFrameTime;
        m_prevFrameTime = m_frameTime;
    }

    float getDeltaTime()
    {   
        setNewFrameTime(); 
        return m_deltaTime; 
    }
};

#endif // RPG_RTIME_H
