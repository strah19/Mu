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
            if (doc->edited) {
            Mu::File file(doc->path);
            file.Empty();
            file.Write(doc->content);
            file.Close();
            ResetEditOnSeletedDocument();
            return true;
        }

        return false;
	}

    void Editor::ResetEditOnSeletedDocument() {
        if (m_docs[m_selected_document]->edited) {
            m_docs[m_selected_document]->edited = false;
            m_docs[m_selected_document]->name.pop_back();
        }
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

    bool Editor::DuplicateFile(const std::string& filepath) {
        for (size_t i = 0; i < m_docs.size(); i++) 
            if (filepath == std::string(m_docs[i]->path)) return true;
        return false;
    }

    void Editor::CreateNewFileFromDialog(const std::string& name) {
        std::string path = Mu::FileDialogs::Save("Mu Environment\0", name.c_str());
        if (path.empty()) return;
        if (DuplicateFile(path)) return;

        m_docs.push_back(new Document(name, path));
        m_selected_document = m_docs.size() - 1;
           
        Mu::File file(path);
        file.Close();
    }

    void Editor::OpenFileFromDialog() {
        std::string path = Mu::FileDialogs::Open("Mu Environment (*.lua *.cpp)\0*.lua;*.cpp\0");
        if (path.empty()) return;
        if (DuplicateFile(path)) return;

        m_docs.push_back(new Document(Mu::GetNameOfPath(path), path));
        m_selected_document = m_docs.size() - 1;

        Document* doc = m_docs[m_selected_document];
        Mu::File file(path);
        doc->content = file.Read();
        file.Close();
    }

    void Editor::OpenFileFromPath(const std::string& path) {
        if (path.empty()) return;
        if (DuplicateFile(path)) return;

        m_docs.push_back(new Document(Mu::GetNameOfPath(path), path));
        m_selected_document = m_docs.size() - 1;

        Document* doc = m_docs[m_selected_document];
        Mu::File file(path);
        doc->content = file.Read();
        file.Close();
    }

    int32_t Editor::SearchByPath(const std::string& path) {
        for(int32_t i = 0; i < m_docs.size(); i++) 
            if (m_docs[i]->path == path) return i;
        return -1;
    }
}