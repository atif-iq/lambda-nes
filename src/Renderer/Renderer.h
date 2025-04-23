#pragma once
#include "../Window/Window.h"


namespace Lambda{
    class Renderer{
    public:
        
        void Init(Window* window){
            if(m_running){
                return;
            }
            m_running = true;
            LAMBDA_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
            LAMBDA_LOG(LogLevel::INFO, "Successfully initialized glad");
            //Quick Workaround. Fix Later? Maybe...
            const std::string& ogl_version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
            LAMBDA_LOG(LogLevel::INFO, "Renderer initialised with Opengl version {}", ogl_version);

            m_window = window;
        }

        inline static Renderer& getRenderer(){
            static Renderer instance;
            return instance;
        }

        void setContext(){
            if(m_window){
                glfwMakeContextCurrent(m_window->m_nativeWindow);
                glViewport(0, 0, m_window->getWidth(), m_window->getHeight());
            }
        }

        void clearBuffer(){
            if(m_window){
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);    
            }
        }

        void Render(){
            
        }

        void frameBufferCallback(uint32_t width, uint32_t height){
            if(m_window){
                glViewport(0, 0, width, height);
            }
        };

        void Terminate(){
            m_window = nullptr;
        }
    private:
    Renderer(){}
        Window* m_window;
        inline static bool m_running = false;
    };
}