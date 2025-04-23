#include "Window.h"
#include <GLFW/glfw3.h>

namespace Lambda{
    Window::Window(const std::string& title, uint32_t width, uint32_t height, bool resizable){
        m_prop.title = title;
        m_prop.width = width;
        m_prop.height = height;
        m_prop.resizable = resizable;
        if(!m_windowCnt){
            glfwSetErrorCallback([](int error, const char* description) -> void {
                LAMBDA_LOG(LogLevel::ERROR, "GLFW Error {}: {}", error, description);
            });
            if(!glfwInit()){
                LAMBDA_LOG(LogLevel::CRITICAL, "Failed initializing GLFW");
            }
            else{
                LAMBDA_LOG(LogLevel::INFO, "Successfully initialized GLFW");
            }
        }
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, (resizable) ? GLFW_TRUE:GLFW_FALSE);

        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            
        #endif
        
        m_nativeWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        
        if(!m_nativeWindow){
            LAMBDA_LOG(LogLevel::ERROR, "Failed creating winodow, \"{}\" width: {} height: {}", title, width, height);
        }
        
        glfwMakeContextCurrent(m_nativeWindow);
        glfwSwapInterval(1);    //Enables VSync
        glfwSetWindowUserPointer(m_nativeWindow, this);

        glfwSetWindowCloseCallback(m_nativeWindow, [](GLFWwindow* m_window) -> void {
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(m_window));
            window->ShutDown();
        });

        glfwSetKeyCallback(m_nativeWindow, [](GLFWwindow* m_window, int key, int scancode, int action, int mods) -> void {
            if(key == GLFW_KEY_ESCAPE){
                glfwSetWindowShouldClose(m_window, true);
                auto window = static_cast<Window*>(glfwGetWindowUserPointer(m_window));
                window->ShutDown();
            }
        });

        glfwSetFramebufferSizeCallback(m_nativeWindow, [](GLFWwindow* window, int width, int height) -> void {
            glViewport(0, 0, width, height);
        });
        
        m_windowCnt++;
        m_prop.running = true;
        LAMBDA_LOG(LogLevel::INFO, "Successfully created winodow, \"{}\" width: {} height: {}", title, width, height);
    }

    std::string Window::getTitle() const{
        return m_prop.title;
    }
    
    int Window::getWidth() const{
        return m_prop.width;
    }

    int Window::getHeight() const{
        return m_prop.height;
    }

    GLFWwindow* Window::getNativeWindow() const{
        return m_nativeWindow;
    }

    bool Window::isRunning() const{
        return m_prop.running;
    }

    void Window::swapBuffer() const{
        if(m_nativeWindow){
            glfwSwapBuffers(m_nativeWindow);
        }
    }

    void Window::pollEvents() const{
        if(m_nativeWindow){
            glfwPollEvents();
        }
    }

    void Window::ShutDown(){
        if (!m_prop.running)
            return;
        m_prop.running = false;
        glfwDestroyWindow(m_nativeWindow);
        m_nativeWindow =nullptr;
        --m_windowCnt;
        LAMBDA_LOG(LogLevel::WARNING, "Destroying window, \"{}\" width: {} height: {}", m_prop.title, m_prop.width, m_prop.height);
    }

    Window::~Window(){
        ShutDown();
        if(!m_windowCnt){
            glfwTerminate();
            LAMBDA_LOG(LogLevel::WARNING, "Terminating GLFW");
        }
    }
}