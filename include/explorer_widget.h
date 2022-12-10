#ifndef EXPLORER_WIDGET_H
#define EXPLORER_WIDGET_H

#include "widget.h"
#include "mu.h"

namespace Iota {
    class ExplorerWidget : public Widget {
    public:
        ExplorerWidget();
        void Update();
    private:
        void OpenFolderCallback();
    private:
        std::string m_current_folder_open;

        Mu::Texture m_addfile_texture;
    };
}

#endif // !EXPLORER_WIDGET_H