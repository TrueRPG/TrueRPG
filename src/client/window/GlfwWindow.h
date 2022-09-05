#ifndef RPG_GLFWWINDOW_H
#define RPG_GLFWWINDOW_H

#include "../graphics/Graphics.h"
#include <string>
#include "IWindow.h"

#include "../../utils/Event.hpp"
#include "glm/vec2.hpp"
#include "../input/GlfwKeyMapper.h"

class GlfwWindow;

class GlfwWindow : public IWindow
{
private:
    GLFWwindow *m_window;

    InputEvent m_onInput;
    ResizeEvent m_onResize;

    GlfwKeyMapper m_glfwKeyMapper;

    bool m_keys[GLFW_KEY_LAST + 1];
    bool m_mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];
public:
    GlfwWindow(int width, int height, const std::string &title);

    void makeContextCurrent() const override;

    void loadGL() const override;

    void swapInterval(int interval) const override;
    
    bool isOpen() const override;

    void close() const override;

    void destroy() const override;

    void swapBuffers() const override;

    void pollEvents() const override;

    int getWidth() const override;

    int getHeight() const override;

    bool getKey(Key key) override;

    bool getMouseButton(int mouseButton) override;

    glm::vec2 getCursorPosition() override;

    ResizeEvent::IType &getOnResize() override;
    InputEvent::IType &getOnInput() override;
private:
    GlfwWindow(const GlfwWindow &) = delete;
    GlfwWindow & operator= (const GlfwWindow &) = delete;

    static void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int actions, int mods);

    static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    static void glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height);
};

#endif // RPG_GLFWWINDOW_H
