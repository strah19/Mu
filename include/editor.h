#ifndef EDITOR_H
#define EDITOR_H

#include "mu.h"

namespace Iota {
    struct Document {
        std::string name;
        std::string content;
        std::string path;

        bool edited = false;

        Document() = default;
        virtual ~Document() { }
        Document(const std::string& name, const std::string& path) : name(name), path(path) { }
    };

    class Editor {
    public:
        Editor();
        virtual ~Editor();
        
        void CreateNewFileFromDialog(const std::string& name);    
        bool SaveSelectedDocument();
        void OpenFileFromDialog();
        void OpenFileFromPath(const std::string& path);
        void CloseSelectedDocument();

        void CloseFiles();

        int32_t SearchByPath(const std::string& path);

        inline void PopDocument() { m_docs.pop_back(); }

        inline size_t DocumentCount() const { return m_docs.size(); }
        inline void SetSelectedDocument(uint32_t index) { m_selected_document = index; }
        inline int32_t GetSelectedDocument() const { return m_selected_document; }

        inline bool SelectedFileEdited() const { return m_docs[m_selected_document]->edited; }
        inline bool IsThereSelectedDocument() const { return (m_selected_document != -1); }

        inline std::vector<Document*> GetDocuments() { return m_docs; }
        bool DuplicateFile(const std::string& filepath);
    private:
        void ResetEditOnSeletedDocument();
    private:
        std::vector<Document*> m_docs;
        int32_t m_selected_document = -1;
    };
}

#endif // !EDITOR_H
