#ifndef EXPLORER_WIDGET_H
#define EXPLORER_WIDGET_H

#include "widget.h"
#include "mu.h"
#include "project.h"

namespace Iota {
    class ExplorerWidget : public Widget {
    public:
        ExplorerWidget();
        void Update();
    private:
        void OpenFolderCallback();

        void DisplayDirTree(const std::filesystem::path& path);
    private:
        Mu::Texture m_addfile_texture;

        Project m_project;
    };
}

#endif // !EXPLORER_WIDGET_H