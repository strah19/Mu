#include "editor.h"
#include "mu.h"

namespace Iota {
	Editor::Editor() { }

	Editor::~Editor() { 
        for (auto& doc : m_docs)
            delete doc;
    }

	bool Editor::SaveSelectedDocument() {
        if (m_selected_document == -1) return false;
    
        Document* doc = m_docs[m_selected_document];
        if (!doc->file.Path().empty()) {
            doc->file.Open(doc->file.Path());
            doc->file.Empty();
            doc->file.Write(doc->content);
            doc->file.Close();
            ResetEditOnSeletedDocument();
            return true;
        }
        else 
            return SaveAsSelectedDocument();

        return false;
	}

    void Editor::ResetEditOnSeletedDocument() {
        if (m_docs[m_selected_document]->edited) {
            m_docs[m_selected_document]->edited = false;
            m_docs[m_selected_document]->name.pop_back();
        }
    }

    bool Editor::SaveAsSelectedDocument() {
        if (m_selected_document == -1) return false;

        std::string path = Mu::FileDialogs::Save("Mu Environment\0");
        if (!path.empty()) {
            Document* doc = m_docs[m_selected_document];
            doc->file.Open(path.c_str());
            doc->name = path.substr(path.find_last_of("/\\") + 1);
            doc->file.Empty();
            doc->file.Write(doc->content);
            doc->file.Close();
            ResetEditOnSeletedDocument();
            return true;
        }
        return false;
    }

	void Editor::CloseSelectedDocument() {
        delete m_docs[m_selected_document];
        m_docs.erase(m_docs.begin() + m_selected_document);
        m_selected_document = -1;
	}

    void Editor::CloseFiles() {
        size_t start_size = m_docs.size();
        for (int i = start_size - 1; i >= 0; i--) {
            SetSelectedDocument(i);
            CloseSelectedDocument();
        }
    }

	void Editor::CreateDocumentFromFileDialog() {
        std::string path = Mu::FileDialogs::Open("Mu Environment (*.lua *.cpp)\0*.lua;*.cpp\0");
        CreateDocumentFromFile(path);
	}

    void Editor::CreateDocumentFromFile(const std::string& path) {
        CreateDocumentFromFile(path, path);
    }

    void Editor::CreateDocumentFromFile(const std::string& path, const std::string& name) {
        if (!path.empty()) {
            if (DuplicateFile(path)) return;

            CreateBlankDocument();
            m_docs.back()->file.Open(path.c_str());
            m_docs.back()->content = m_docs.back()->file.Read();
            m_docs.back()->name = name;
        }     
    }

    void Editor::CreateBlankDocument() {
        m_docs.push_back(new Document());
        SetSelectedDocument(m_docs.size() - 1);
    }

    bool Editor::DuplicateFile(const std::string& filepath) {
        for (size_t i = 0; i < m_docs.size(); i++) 
            if (filepath == std::string(m_docs[i]->file.Path())) return true;
        return false;
    }
}