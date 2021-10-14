#ifndef RPG_WINDOW_H
#define RPG_WINDOW_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>

class Window;

using InputCallback = void (*)(Window *, int, int, int, int);
using ResizeCallback = void (*)(Window *, int, int);

class Window
{
private:
    GLFWwindow *m_window;
    InputCallback m_inputCallback;
    ResizeCallback m_resizeCallback;
    bool m_keys[GLFW_KEY_LAST + 1];
public:
    bool isOpen() const;

    void close() const;

    void destroy() const;

    void swapBuffers() const noexcept;

    void pollEvents() const;

    int getWidth() const;

    int getHeight() const;

    bool getKey(int key);

    void setInputCallback(InputCallback inputCallback);

    void setResizeCallback(ResizeCallback resizeCallback);

    static Window& getInstance(int width = 0, int height = 0, const std::string& title = "");

private:
    Window(int width, int height, const std::string &title);

    Window(const Window&) = delete;
    Window& operator= (const Window&) = delete;

    void onKey(int key, int scancode, int actions, int mods);

    void onResize(int width, int height);

    static void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int actions, int mods);

    static void glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height);
};

#endif //RPG_WINDOW_H
