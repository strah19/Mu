#ifndef FILE_H
#define FILE_H

#include <string>
#include <functional>
#include <sstream>
#include <fstream>

namespace Mu {
    class File {
    public:
        File(const char* filepath);
        File();

        virtual ~File();
        void Empty();
        void Close();
        void Destroy();
        void Open(const char* filepath);
        std::fstream& FileHandler() { return m_file; }

        bool IsOpen();
        bool Eof();
        std::streampos Location();
        void Reset();
        void Location(const uint32_t read_location);
        std::streamoff Size();
        bool IsEmpty();
        const char* Path();

        uint64_t LineCount();
        uint64_t CharacterCount();
        uint64_t WordCount();

        std::string Read();
        std::string ReadLine(const uint32_t line_number);
        std::string ReadWord(const uint32_t location);
        void DeleteLine(const uint32_t line_number);

        template <typename T>
        void ReadObj(T& object) {
            m_file.read((char*)&object, sizeof(object));
        }

        template <typename T>
        void WriteObj(T& object) {
            m_file.write((char*)&object, sizeof(object));
        }

        template<typename T>
        void WriteAsLine(const T& stream, const uint32_t line_number) {
            std::string file_data = Read();

            int word_counter = 0;
            if (line_number > -1) {
                for (size_t i = 0; i < file_data.length(); i++) {
                    if (file_data[i] == '\n') {
                        if (line_number == word_counter) {
                            std::stringstream ss;
                            ss << stream;
                            file_data.insert(i, ss.str());
                            break;
                        }
                        word_counter++;
                    }
                }

                Empty();
                m_file << file_data;
            }
        }

        template <typename T>
        void Write(const T& stream) {
            std::stringstream ss;
            ss << stream;

            m_file << ss.str();
        }

        void DoEachWord(const std::function<bool(const std::string& word, const uint32_t counter)>& func);
        void DoEachLine(const std::function<bool(const std::string& line, const uint32_t counter)>& func);
    protected:
        std::string m_filepath;
        std::fstream m_file;
    };
}

#endif // !FILE_H