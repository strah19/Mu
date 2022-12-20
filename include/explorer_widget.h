#ifndef EXPLORER_WIDGET_H
#define EXPLORER_WIDGET_H

#include "widget.h"
#include "mu.h"
#include "editor.h"
#include "project.h"

namespace Iota {
    class ExplorerWidget : public Widget {
    public:
        ExplorerWidget(Editor* editor);
        void Update();
    private:
        void OpenFolderCallback();
        void CloseFolderCallback();

        void DisplayDirTree(const std::filesystem::path& path);
    private:
        Mu::Texture m_addfile_texture;

        std::string m_selected_filepath;
        Editor* m_editor;
        Project m_project;
    };
}

#endif // !EXPLORER_WIDGET_H