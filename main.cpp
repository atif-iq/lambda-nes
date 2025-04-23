#include "src/Window/Window.h"
#include "src/Renderer/Renderer.h"
#include "src/GUI/GUI.h"
#include "src/CPU/CPU.h"

int main(){
    Lambda::CPU cpu;
    Lambda::Window window("NES 6502", 1050, 600);
    Lambda::Renderer::getRenderer().Init(&window);
    Lambda::Renderer::getRenderer().setContext();
    Lambda::GUI::getGUI().Init(window.getNativeWindow(), &cpu);
    while(window.isRunning()){         
        if(cpu.running()){
            cpu.fetch_decode_execute();
        }
        Lambda::Renderer::getRenderer().clearBuffer();
        //Order matters, Z - Indexing
        Lambda::Renderer::getRenderer().Render();
        Lambda::GUI::getGUI().Render();
        window.swapBuffer();
        window.pollEvents();
    }

    Lambda::GUI::getGUI().Terminate();
    Lambda::Renderer::getRenderer().Terminate();
    return 0;
}