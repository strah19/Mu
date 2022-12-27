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
        CommandArg(const char* str) : str(str), type(CommandType::STR) { }
        union {
            const char* str;
            double num;
        };  
        CommandType type = CommandType::STR;
    };

    typedef CommandOutput (*CommandFn)(const std::vector<CommandArg>&);
    struct Command {
        Command(const std::string& name, const std::vector<CommandType>& arg_types, CommandFn fn) : name(name), fn(fn), variable(false), variable_type(CommandType::NONE) { 
            std::copy(arg_types.begin(), arg_types.end(), back_inserter(this->arg_types));
        }
        Command(const std::string& name, CommandFn fn, CommandType variable_type = CommandType::STR) : name(name), fn(fn), variable(true), variable_type(variable_type) { }
        Command(const std::string& name, const std::vector<CommandType>& arg_types, CommandFn fn, CommandType variable_type) : name(name), fn(fn), variable(true), variable_type(variable_type) { 
            std::copy(arg_types.begin(), arg_types.end(), back_inserter(this->arg_types));
        }

        bool variable;
        std::string name;
        CommandType variable_type;
        std::vector<CommandType> arg_types;
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
        inline void AddToHistory(const std::string& entry) { m_history.push_back(entry); }
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