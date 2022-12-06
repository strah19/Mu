#include "file.h"
#include <vector>
#include <iostream>
#include "log.h"

namespace Mu {
    File::File(const char* filepath) : m_filepath(filepath) {
        Open(m_filepath.c_str());
    }

    File::File() : m_filepath() { }

    File::~File() {
        if (IsOpen())
            Close(m_filepath.c_str());
    }

    void File::Empty() {
        Close(m_filepath.c_str());

        /* Setting it to the trunc mode will clear the m_file's contents. */
        m_file.open(m_filepath, std::fstream::out | std::fstream::trunc);
    }

    void File::Close(const char* filepath) {
        m_file.flush();
        m_file.close();
        MU_LOG("Closed file '%s'", filepath);
    }

    void File::Destroy() {
        Close(m_filepath.c_str());

        remove(m_filepath.c_str());
    }

    void File::Open(const char* filepath) {
        m_file = std::fstream(filepath, std::fstream::in | std::fstream::out | std::fstream::app);
        m_filepath = filepath;

        MU_LOG("Opened file '%s'", m_filepath.c_str());
    }

    bool File::IsOpen() {
        return m_file.is_open();
    }

    bool File::Eof() {
        return m_file.eof();
    }

    std::streampos File::Location() {
        return m_file.tellg();
    }

    void File::Reset() {
        m_file.clear();
        m_file.seekg(0);
    }

    void File::Location(const uint32_t read_location) {
        if (read_location > -1)
            m_file.seekg(read_location, std::ios::beg);
    }

    std::streamoff File::Size() {
        std::streampos beg, end;

        m_file.seekg(0, std::ios::beg);
        beg = m_file.tellg();
        m_file.seekg(0, std::ios::end);
        end = m_file.tellg();

        return end - beg;
    }

    bool File::IsEmpty() {
        return (Size() == 0) ? true : false;
    }

    const char* File::Path() {
        return m_filepath.c_str();
    }

    uint64_t File::LineCount() {
        Reset();
        int num_lines = 0;
        std::string unused;
        while (std::getline(m_file, unused))
            ++num_lines;

        return num_lines;
    }

    uint64_t File::CharacterCount() {
        Reset();

        char active_char;
        unsigned long number_chars = 0;

        while (m_file.get(active_char))
            if (active_char != '\n')
                number_chars++;

        return number_chars;
    }

    uint64_t File::WordCount() {
        Reset();

        std::string search_word;
        uint64_t word_counter = 0;
        while (m_file >> search_word)
            word_counter++;

        return word_counter;
    }

    std::string File::Read() {
        Reset();

        std::string buffer((std::istreambuf_iterator<char>(m_file)),
            std::istreambuf_iterator<char>());

        return buffer;
    }

    std::string File::ReadLine(const uint32_t line_number) {
        Reset();

        std::string line_search;
        uint32_t line_counter = 0;

        while (getline(m_file, line_search)) {
            if (line_counter == line_number)
                return line_search;
            line_counter++;
        }

        return "";
    }

    std::string File::ReadWord(const uint32_t location) {
        Reset();

        std::string word_search;
        uint32_t word_counter = 0;
        while (m_file >> word_search) {
            if (word_counter == location)
                return word_search;
            word_counter++;
        }
        return "";
    }

    void File::DeleteLine(const uint32_t line_number) {
        std::string m_file_data = Read();

        std::string line_search;
        unsigned int line_counter = 0;

        std::vector<size_t> line_sizes;

        while (getline(m_file, line_search)) {
            line_sizes.push_back(line_search.length());

            if (line_counter == line_number) {
                size_t location = 0;
                for (size_t i = 0; i < line_sizes.size() - 1; i++)
                    location += line_sizes[i];
                m_file_data.erase(location, line_sizes.back() + 1);
                break;
            }
            line_counter++;
        }

        line_sizes.clear();

        Empty();
        m_file << m_file_data;
    }

    void File::DoEachWord(const std::function<bool(const std::string& word, const uint32_t counter)>& func) {
        std::string word_search;
        uint32_t word_counter = 0;
        while (m_file >> word_search) {
            if (!func(word_search, word_counter))
                break;
            word_counter++;
        }

        Reset();
    }

    void File::DoEachLine(const std::function<bool(const std::string& line, const uint32_t counter)>& func) {
        std::string line;
        uint32_t line_counter = 0;
        while (getline(m_file, line)) {
            if (!func(line, line_counter))
                break;
            line_counter++;
        }

        Reset();
    }
}