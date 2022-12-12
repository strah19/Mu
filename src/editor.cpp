#include "editor.h"

namespace Iota {
	Editor::Editor() { }

	Editor::~Editor() { 
        for (auto& doc : m_docs)
            delete doc;
    }

	bool Editor::SaveSelectedDocument() {
        bool successful = false;

        if (m_selected_document != -1) {
            Document* doc = m_docs[m_selected_document];
            if (doc->file.IsOpen()) {
                doc->file.Empty();
                doc->file.Write(doc->content);
                successful = true;
            }
            else {
                std::string path = Mu::FileDialogs::Save("Mu Environment (*.lua *.cpp)\0*.lua;*.cpp\0");
                if (!path.empty()) {
                    doc->file.Open(path.c_str());
                    doc->file.Empty();
                    doc->file.Write(doc->content);
                    successful = true;
                }
            }
        }

        if (successful && m_docs[m_selected_document]->edited) {
            m_docs[m_selected_document]->edited = false;
            m_docs[m_selected_document]->name.pop_back();
        }

        return successful;
	}

	void Editor::CloseSelectedDocument() {
        delete m_docs[m_selected_document];
        m_docs.erase(m_docs.begin() + m_selected_document);
        m_selected_document = -1;
	}

	void Editor::CreateDocumentFromFile() {
        std::string path = Mu::FileDialogs::Open("Mu Environment (*.lua *.cpp)\0*.lua;*.cpp\0");
        if (!path.empty()) {
            m_docs.push_back(new Document());
            m_docs.back()->file.Open(path.c_str());
            m_docs.back()->content = m_docs.back()->file.Read();
            m_docs.back()->name = m_docs.back()->file.Path();
        }
	}

    void Editor::CreateBlankDocument() {
        m_docs.push_back(new Document());
    }
}