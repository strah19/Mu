#ifndef EDITOR_H
#define EDITOR_H

#include "mu.h"

namespace Iota {
    struct Document {
        std::string name;
        std::string content;
        Mu::File file;
        bool edited = false;

        Document() = default;
        virtual ~Document() { }
        Document(const std::string& name) : name(name) { }
    };

    class Editor {
    public:
        Editor();
        virtual ~Editor();

        bool SaveSelectedDocument();
        void CloseSelectedDocument();
        void CreateDocumentFromFile();
        void CreateBlankDocument();

        inline void PopDocument() { m_docs.pop_back(); }

        inline size_t DocumentCount() const { return m_docs.size(); }
        inline void SetSelectedDocument(uint32_t index) { m_selected_document = index; }
        inline uint32_t GetSelectedDocument() const { return m_selected_document; }
        inline bool SelectedFileEdited() const { return m_docs[m_selected_document]->edited; }
        inline bool IsThereSelectedDocument() const { return (m_selected_document != -1); }
        inline std::vector<Document*> GetDocuments() { return m_docs; }
    private:
        std::vector<Document*> m_docs;
        uint32_t m_selected_document = -1;
    };
}

#endif // !EDITOR_H
