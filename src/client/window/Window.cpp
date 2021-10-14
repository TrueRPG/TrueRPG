#include "Window.h"

#include <iostream>
#include <cassert>

Window& Window::getInstance(int width, int height, const std::string& title)
{
    static Window window(width, height, title);
    return window;
}

Window::Window(int width, int height, const std::string &title) : m_keys()
{
    assert(glfwInit());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    m_inputCallback = [](Window *, int, int, int, int) {};
    m_resizeCallback = [](Window *, int, int) {};

    if (m_window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    assert(gladLoadGL(glfwGetProcAddress));
    glfwSwapInterval(0); // It's useful to see max fps, so I turned off vsync
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, glfwKeyCallback);
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

void Window::setInputCallback(InputCallback inputCallback)
{
    m_inputCallback = inputCallback;
}

void Window::setResizeCallback(ResizeCallback resizeCallback)
{
    m_resizeCallback = resizeCallback;
}

void Window::onKey(int key, int scancode, int actions, int mods)
{
    m_inputCallback(this, key, scancode, actions, mods);
}

void Window::onResize(int width, int height)
{
    m_resizeCallback(this, width, height);
}

void Window::glfwKeyCallback(GLFWwindow *window, int key, int scancode, int actions, int mods)
{
    auto *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
    win->onKey(key, scancode, actions, mods);

    if (key < 0) return;

    switch (actions)
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

void Window::glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    auto *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
    win->onResize(width, height);
}
