#ifndef TERMINAL_H
#define TERMINAL_H

#include <vector>
#include <string>
#include <sstream>

#define MAX_OUTPUT_LEN 1024

#define NO_ERROR NULL
#define UNKNOWN_CMD_ERROR "[error] Unknown command"
#define NUM_ARGS_ERROR  "[error] Incorrect number of arguments passed"
#define NUM_TYPE_ERROR "[error] Expected number as an argument"

namespace Iota {
    struct CommandOutput {
        const char* error = NO_ERROR;
        std::string output;
    };

    enum CommandType {
        STR, NUM, ANY, NONE
    };

    struct CommandArg {
        CommandArg(const char* raw) : raw(raw), str(raw), type(CommandType::STR) { }
        union {
            const char* str;
            double num;
        };
        std::string raw;  
        CommandType type = CommandType::STR;

        bool IsNumber() const {
            return (type == CommandType::NUM);
        }
    };

    typedef CommandOutput (*CommandFn)(const std::vector<CommandArg>&);
    struct Command {
        Command(const std::string& name, CommandFn fn) : name(name), fn(fn) {  }

        std::string name;
        CommandFn fn;
    };

    class GlobalCommands {
    public:
        static void AddCommand(const Command& command);
    };

    class Terminal {
    public:
        Terminal();

        inline void SetInput(const std::string& input) { m_input = input; }
        inline std::vector<std::string> GetOutput() const { return m_output; }
        inline std::vector<std::string> GetHistory() const { return m_history; }

        void Execute();
    private:
        void AddToHistory(const std::string& entry);
        inline void ClearHistory() { m_history.clear(); }
        inline void ClearOutputBuffer() { m_output.clear(); }

        void FormattedOutput(const char* fmt, ...);
    private:
        std::vector<std::string> m_output;
        std::string m_input;

        std::vector<std::string> m_history;
    };
}

#endif // !TERMINAL_H