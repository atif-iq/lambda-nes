#pragma once
#include <bitset>
#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/backends/imgui_impl_glfw.h"
#include "../../vendor/imgui/backends/imgui_impl_opengl3.h"
#include "../CPU/CPU.h"
#include "../BUS/BUS.h"

namespace Lambda{

    //Singleton design for the UI Interface
    class GUI{
    public:
        GUI(GUI&) = delete;
        GUI& operator=(const GUI&) = delete;

        inline static GUI& getGUI(){
            static GUI instance;
            return instance;
        }

        void Init(GLFWwindow* window, CPU* cpu){
            m_cpu = cpu;
            
            //Imgui initialisation code
            const char* glsl_version = "#version 130";
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();

            ImGuiIO& io = ImGui::GetIO(); (void)io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            io.Fonts->AddFontFromFileTTF("fonts/JetBrainsMono-Regular.ttf", 16.0f); //Loads custom font
            ImGui::StyleColorsDark();   //For defaulting dark colors on styles we have not modified

            /* Custom Styling ImGui */
            ImGuiStyle * style = &ImGui::GetStyle();
            style->WindowPadding = ImVec2(15, 15);
            style->FramePadding = ImVec2(5, 5);
            style->FrameRounding = 4.0f;
            style->ItemSpacing = ImVec2(12, 8);
            style->ItemInnerSpacing = ImVec2(8, 6);
            style->IndentSpacing = 25.0f;
            style->ScrollbarSize = 15.0f;
            style->ScrollbarRounding = 9.0f;
            style->GrabMinSize = 5.0f;
            style->GrabRounding = 3.0f;

            style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
            style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
            style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
            style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
            style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
            style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
            style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
            style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
            style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
            style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
            style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
            style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
            style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
            style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
            style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
            style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
            style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
            style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
            style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
            style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
            style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
            style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
            style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);

            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init(glsl_version);

            //Getting access to the RAM and storing it in a raw pointer for accessing data through GUI
            for(auto& component: m_cpu->m_bus->m_components){
                if((m_ram = dynamic_cast<RAM*>(component.get()))){
                    break;
                }
            }
        }

        void Render(){
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();      

            //Actual GUI code

            float leftPaneWidth = 512.0f; // double NES resolution
            float totalWidth = ImGui::GetIO().DisplaySize.x;
            float totalHeight = ImGui::GetIO().DisplaySize.y;

            //Centers the GameOutput Window
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(leftPaneWidth, totalHeight));

            ImGui::Begin("Game Output", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
            ImGui::End();

            //Sets the Debug Info right next to the GameOutput window
            ImGui::SetNextWindowPos(ImVec2(leftPaneWidth, 0));
            ImGui::SetNextWindowSize(ImVec2(totalWidth - leftPaneWidth, totalHeight));

            ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

            /*--- Components for Debugging -------------------------------------------------------------------*/

            ImGui::Text("Control Panel");
            //ImVec2(x,y): x: 0 because setting it to zero auto fills the rest of the place based on parent's width

            //Individual fixed width, length sections inside the DebugInfo window called "xscroll"
            ImGui::BeginChild("ControlScroll", ImVec2(0, 170), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);

            //Buttons when pressed passes the conditional
            if(ImGui::Button("Play")){
                m_cpu->emulation_paused = false;
            }
            ImGui::SameLine();
            if(ImGui::Button("Pause")){
                m_cpu->emulation_paused = true;
            }
            ImGui::SameLine();
            if(ImGui::Button("Step")){
                //Pauses execution after single step (Feature not a bug :D)
                m_cpu->emulation_paused = true;
                m_cpu->fetch_decode_execute();
            }
            ImGui::SameLine();
            if(ImGui::Button("Reset")){
                m_cpu->reset();
            }

            ImGui::Text("FPS: ");
            ImGui::Text("Cycles Executed: %d", m_cpu->m_cycles);
            ImGui::Text("ROM: %s", m_cpu->m_romName.c_str());
            ImGui::EndChild();

            ImGui::Separator();
            ImGui::Text("CPU Registers");
            // ImGui::BeginChild("RegisterScroll", ImVec2(0, 170), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
            ImGui::BeginChild("RegisterScroll", ImVec2(0, 170), true);

            //Dummy variables as InputInt doesn't takes uint8_t might fix later
            int32_t acc = m_cpu->A; 
            int32_t stk_ptr= m_cpu->SP;
            int32_t index_y= m_cpu->IY;
            int32_t index_x= m_cpu->IX;
            ImGui::InputInt(" A", &acc,1,100, ImGuiInputTextFlags_CharsHexadecimal);
            ImGui::InputInt("SP", &stk_ptr, 1, 100, ImGuiInputTextFlags_CharsHexadecimal);
            ImGui::InputInt("IY", &index_y, 1, 100, ImGuiInputTextFlags_CharsHexadecimal);
            ImGui::InputInt("IX", &index_x, 1, 100, ImGuiInputTextFlags_CharsHexadecimal);
            //Casting back to 8 bits
            m_cpu->A = static_cast<uint8_t>(acc&0x00FF);
            m_cpu->SP = static_cast<uint8_t>(stk_ptr&0x0000FF);
            m_cpu->IX = static_cast<uint8_t>(index_x&0x0000FF);
            m_cpu->IY = static_cast<uint8_t>(index_y&0x0000FF);
            
            //The actual PC changes due to manipulation of the PC by branching or changing instructions, so a constant dummy called oldPC is kept
            ImGui::Text("PC: 0x%04X", m_cpu->oldPC);
            ImGui::Text("PS: 0b%s (%04X)", std::bitset<8>(m_cpu->PS).to_string().c_str(), m_cpu->PS);
            ImGui::EndChild();
            ImGui::Separator();

            ImGui::Text("Disassembly");
            ImGui::BeginChild("DisassemblyScroll", ImVec2(0, totalHeight/4), true, ImGuiWindowFlags_HorizontalScrollbar);
            //Addressing mode string
            const std::string addrName = m_cpu->m_instructions[m_cpu->m_currentOpcode].addrName;
            //Oocodes string
            const char* funcName = m_cpu->m_instructions[m_cpu->m_currentOpcode].name.c_str();

            ImGui::SameLine();
            ImGui::Text("[%s]", addrName.c_str());
            ImGui::SameLine();
            if(addrName == "ACC") {
                ImGui::Text("%02X", m_cpu->read8(m_cpu->oldPC));
                ImGui::SameLine();
                ImGui::Text("%s", funcName);
            }
            else if (addrName == "IMM") {
                ImGui::Text("%02X %02X", m_cpu->read8(m_cpu->oldPC), m_cpu->read8(m_cpu->oldPC+1));
                ImGui::SameLine();
                ImGui::Text("%s", funcName);
                ImGui::SameLine();
                ImGui::Text("#$%04X", m_cpu->read8(m_cpu->oldPC+1));
            }
            else if (addrName == "ABS") {
                ImGui::Text("%02X %02X %02X", m_cpu->read8(m_cpu->oldPC), m_cpu->read8(m_cpu->oldPC+1), m_cpu->read8(m_cpu->oldPC+2));
                ImGui::SameLine();
                ImGui::Text("%s", funcName);
                ImGui::SameLine();
                ImGui::Text("$%04X", (m_cpu->read8(m_cpu->oldPC+2)<<8)|m_cpu->read8(m_cpu->oldPC+1));
            }
            else if (addrName == "ZP ") {
                ImGui::Text("%02X %02X", m_cpu->read8(m_cpu->oldPC), m_cpu->read8(m_cpu->oldPC+1));
                ImGui::SameLine();
                ImGui::Text("%s", funcName);
                ImGui::SameLine();
                ImGui::Text("$%02X", (m_cpu->read8(m_cpu->oldPC+1)));

            }
            else if (addrName == "ZPX") {
                ImGui::Text("%02X %02X", m_cpu->read8(m_cpu->oldPC), m_cpu->read8(m_cpu->oldPC+1));
                ImGui::SameLine();
                ImGui::Text("%s", funcName);
                ImGui::SameLine();
                ImGui::Text("$%02X", (m_cpu->read8(m_cpu->oldPC+1)));

            }
            else if (addrName == "ZPY") {
                ImGui::Text("%02X %02X", m_cpu->read8(m_cpu->oldPC), m_cpu->read8(m_cpu->oldPC+1));
                ImGui::SameLine();
                ImGui::Text("%s", funcName);
                ImGui::SameLine();
                ImGui::Text("$%02X", (m_cpu->read8(m_cpu->oldPC+1)));
            }
            else if (addrName == "REL") {
                ImGui::Text("%02X %02X", m_cpu->read8(m_cpu->oldPC), m_cpu->read8(m_cpu->oldPC+1));
                ImGui::SameLine();
                ImGui::Text("%s", funcName);
                ImGui::SameLine();
                ImGui::Text("%d", (static_cast<int8_t>(m_cpu->read8(m_cpu->oldPC+1))));
                ImGui::SameLine();
                ImGui::Text("($%04X)", (static_cast<int8_t>(m_cpu->read8(m_cpu->oldPC+1)))+m_cpu->oldPC+2);
            }
            else if (addrName == "IMP") {
                ImGui::Text("%02X", m_cpu->read8(m_cpu->oldPC));
                ImGui::SameLine();
                ImGui::Text("%s", funcName);
            }
            else if (addrName == "IND") {
                ImGui::Text("%02X %02X %02X", m_cpu->read8(m_cpu->oldPC), m_cpu->read8(m_cpu->oldPC+1), m_cpu->read8(m_cpu->oldPC+2));
                ImGui::SameLine();
                ImGui::Text("%s", funcName);
                ImGui::SameLine();
                ImGui::Text("$%04X", (m_cpu->read8(m_cpu->oldPC+2)<<8)|m_cpu->read8(m_cpu->oldPC+1));
            }
            else if (addrName == "IDX") {
                ImGui::Text("%02X %02X %02X", m_cpu->read8(m_cpu->oldPC), m_cpu->read8(m_cpu->oldPC+1), m_cpu->read8(m_cpu->oldPC+2));
                ImGui::SameLine();
                ImGui::Text("%s", funcName);
                ImGui::SameLine();
                ImGui::Text("$%04X", (m_cpu->read8(m_cpu->oldPC+2)<<8)|m_cpu->read8(m_cpu->oldPC+1));
            }
            else if (addrName == "IDY") {
                ImGui::Text("%02X %02X %02X", m_cpu->read8(m_cpu->oldPC), m_cpu->read8(m_cpu->oldPC+1), m_cpu->read8(m_cpu->oldPC+2));
                ImGui::SameLine();
                ImGui::Text("%s", funcName);
                ImGui::SameLine();
                ImGui::Text("$%04X", (m_cpu->read8(m_cpu->oldPC+2)<<8)|m_cpu->read8(m_cpu->oldPC+1));
            }
            else if (addrName == "ABX") {
                ImGui::Text("%02X %02X %02X", m_cpu->read8(m_cpu->oldPC), m_cpu->read8(m_cpu->oldPC+1), m_cpu->read8(m_cpu->oldPC+2));
                ImGui::SameLine();
                ImGui::Text("%s", funcName);
                ImGui::SameLine();
                ImGui::Text("$%04X", (m_cpu->read8(m_cpu->oldPC+2)<<8)|m_cpu->read8(m_cpu->oldPC+1));
            }
            else if (addrName == "ABY") {
                ImGui::Text("%02X %02X %02X", m_cpu->read8(m_cpu->oldPC), m_cpu->read8(m_cpu->oldPC+1), m_cpu->read8(m_cpu->oldPC+2));
                ImGui::SameLine();
                ImGui::Text("%s", funcName);
                ImGui::SameLine();
                ImGui::Text("$%04X", (m_cpu->read8(m_cpu->oldPC+2)<<8)|m_cpu->read8(m_cpu->oldPC+1));
            }
            else {
                ImGui::Text("Okay what the fuck bro??");
            }
            ImGui::SameLine();
            ImGui::EndChild();
            ImGui::Separator();
            
            //The memory viewer shows the RAM in realtime
            ImGui::Text("Memory View");
            ImGui::SameLine();
            static char searchBuf[32] = "";
            static bool scrolled = true;
            static size_t searchAddr = 0;
            static size_t highlightAddr = -1;   //-1 highlights nothing
            ImGui::PushItemWidth(150);
            ImGui::InputTextWithHint("##SearchAddr", "Search address", searchBuf, IM_ARRAYSIZE(searchBuf));
            ImGui::PopItemWidth();
            ImGui::SameLine();

            if (ImGui::Button("Go")) {
                if (strlen(searchBuf) > 0) {
                    searchAddr = strtoul(searchBuf, nullptr, 16);
                    scrolled = false;
                    highlightAddr = searchAddr;
                }
            }

            ImGui::BeginChild("MemoryScroll", ImVec2(0, totalHeight/2), true, ImGuiWindowFlags_HorizontalScrollbar);

            //Traversing via the whole memory
            for(uint32_t i=0; i<=0xFFFF; i++){
                //Is a multiple of 16 (yes 0 inclusive for `AESTHETICS` ^_^), i.e one row is finished, we print 8 per row
                if(i==0) [[unlikely]]{
                    ImGui::Text("$%04X", i);
                }
                else if(i%16==0 && i!=0) [[likely]]{    //Branch optimisation
                    ImGui::NewLine();
                    ImGui::Text("$%04X", i);
                }
                ImGui::SameLine();
                if(i==highlightAddr){
                    //This is to ensure we're not stuck in one place, like if this were absent and we scroll up it'll bring us back to the same position.
                    if(!scrolled){
                        ImGui::SetScrollHereY(0.5f);    //Scroll to the current rendering row
                        ImGui::SetScrollHereX(0.5f);    //Scroll to the current rendering column
                        scrolled=true;
                    }
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255)); //Yellow
                    ImGui::Text("%02X", m_ram->read8(i)); //memory is one dimensional
                    ImGui::SameLine();
                    ImGui::PopStyleColor();
                    continue;
                }
                ImGui::Text("%02X", m_ram->read8(i)); //memory is one dimensional
                ImGui::SameLine();   
            }
            
            ImGui::EndChild();
            ImGui::End();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        void Terminate(){
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            m_cpu = nullptr;
            m_ram = nullptr;
        }
    private:
        CPU* m_cpu; //GUI is friend with CPU, so it'll need an object to access and print data for debugging 
        RAM* m_ram; //GUI is friend with RAM, so it'll need an object to access and print data for debugging
        GUI(){}
    };
}