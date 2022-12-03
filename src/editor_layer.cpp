#include "editor_layer.h"
#include "application.h"

static int MyResizeCallback(ImGuiInputTextCallbackData* data) {
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        ImVector<char>* my_str = (ImVector<char>*)data->UserData;
        IM_ASSERT(my_str->begin() == data->Buf);
        my_str->resize(data->BufSize); 
        data->Buf = my_str->begin();
    }
    return 0;
}

#define MAX_FILENAME_LEN 256

namespace Iota {
    void EditorLayer::UpdateGui() {
        ImGui::Begin("Editor");

        if (ImGui::BeginTabBar("editor tab")) {
            for (int i = 0; i < in_editor_files.size(); i++) {
                if (i == in_editor_files.size() - 1 && creating_new_file)
                    break;

                if (ImGui::BeginTabItem(in_editor_files[i].c_str())) {
                    ImGui::InputTextMultiline("input", in_editor_data[i].begin(), (size_t)in_editor_data[i].size(), ImGui::GetContentRegionAvail(), 
                                            ImGuiInputTextFlags_CallbackResize, MyResizeCallback, &in_editor_data[i]);
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }

        NewFile();

        ImGui::End();
    }

    void EditorLayer::MenuEventCall(MenuEvent menu_event) {
        if (menu_event == MenuEvent::NEW) {
            creating_new_file = true;
            in_editor_files.push_back(std::string());

            in_editor_data.push_back(ImVector<char>());
            in_editor_data.back().push_back(0);
        }
    }

    void EditorLayer::NewFile() {
        if (creating_new_file) {
            ImGui::OpenPopup("New File");
            if (ImGui::BeginPopupModal("New File", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
                ImGui::InputText("Name", (char*) in_editor_files.back().data(), MAX_FILENAME_LEN);
                if (ImGui::Button("Enter")) {
                    ImGui::CloseCurrentPopup();
                    creating_new_file = false;
                }
                ImGui::EndPopup();
            }
        }
    }
}