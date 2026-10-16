#include <fstream>
#include <string>
#include <format>
#include <filesystem>
#include <utils/time.hpp>
#include <utility>
#include <vector>
namespace fs = std::filesystem;

enum class MessageType {
    Info,
    Warn,
    Error,
    Fatal
};


class Log {
    private:
    std::ofstream file;
    std::vector<std::string> signatures;

    const inline std::string mttos(const MessageType &input) {
        switch(input) {
            case MessageType::Info: return "[INFO]";
            case MessageType::Warn: return "[WARN]";
            case MessageType::Error: return "[ERROR]";
            case MessageType::Fatal: return "[FATAL]";
        }
    }
    int current() {return signatures.size() - 1; }
    const std::string DEFAULT_SIG = "UNKNOWN";

    public:

    class SignatureScope {
        private:
        Log &log;

        public:
        SignatureScope(Log &owner, std::string signature) : log(owner) {
            log.signatures.push_back(std::move(signature));
        }

        ~SignatureScope() { log.signatures.pop_back(); }
    };

    void write(const std::string &message, const MessageType &type = MessageType::Info) {
        file << std::format(
            "{} {} {} {}",
            csttime::currentMinute(),
            signatures.empty() ? DEFAULT_SIG : signatures.at(current()),
            mttos(type),
            message
        );
    }
    void setStream(std::ofstream str) { file = std::move(str); }
    bool loadStream(fs::path p, bool clear) {
        if (!fs::exists(p)) return false;
        file.open(p, (clear ? std::ios::trunc : std::ios::app));
        return file.is_open();
    }

};