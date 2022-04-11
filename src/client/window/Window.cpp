#include "Window.h"

#include <iostream>
#include <cassert>

Window& Window::getInstance(int width, int height, const std::string& title)
{
    static Window window(width, height, title);
    return window;
}

Window::Window(int width, int height, const std::string &title) : m_keys(), onResize(m_onResize), onInput(m_onInput)
{
    glfwInit();

    static auto errorCallback = [](int error, const char *description)
    {
      // why not cerr?
      // #error is printed in hex because GLFW defines the codes as hex values
      std::cout << "GLFW error 0x" << std::hex << error << ": " << description
                << std::endl;
    };
    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (m_window == nullptr)
    {
        // we'll not print any information, because if GLFW fails, it will print the error.
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(0); // It's useful to see max fps, so I turned off vsync
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, glfwKeyCallback);
    glfwSetMouseButtonCallback(m_window, glfwMouseButtonCallback);
    glfwSetFramebufferSizeCallback(m_window, glfwFramebufferSizeCallback);

    glfwMakeContextCurrent(m_window);
}

bool Window::isOpen() const
{
    return !glfwWindowShouldClose(m_window);
}

void Window::close() const
{
    glfwSetWindowShouldClose(m_window, GL_TRUE);
}

void Window::destroy() const
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::swapBuffers() const noexcept
{
    glfwSwapBuffers(m_window);
}

void Window::pollEvents() const
{
    glfwPollEvents();
}

int Window::getWidth() const
{
    int width;
    glfwGetWindowSize(m_window, &width, nullptr);
    return width;
}

int Window::getHeight() const
{
    int height;
    glfwGetWindowSize(m_window, nullptr, &height);
    return height;
}

bool Window::getKey(int key)
{
    return m_keys[key];
}

glm::vec2 Window::getCursorPosition()
{
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);
    return {x, y};
}

bool Window::getMouseButton(int mouseButton)
{
    return m_mouseButtons[mouseButton];
}

void Window::glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    auto *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (key < 0) return;

    win->m_onInput(key, action);

    switch (action)
    {
    case GLFW_PRESS:
        getInstance().m_keys[key] = true;
        break;
    case GLFW_RELEASE:
        getInstance().m_keys[key] = false;
        break;
    default:
        break;
    }
}

void Window::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    switch (action)
    {
    case GLFW_PRESS:
        getInstance().m_mouseButtons[button] = true;
        break;
    case GLFW_RELEASE:
        getInstance().m_mouseButtons[button] = false;
        break;
    default:
        break;
    }
}

void Window::glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    auto *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
    win->m_onResize(width, height);
}
