#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace albert::args
{
    // A declared flag. Names are stored without dashes ("output", "o").
    struct Flag {
        enum class ValueStatus {
            None,
            Optional,
            Required
        };
        std::string verbose;
        std::string v;
        ValueStatus status = ValueStatus::None;
    };

    struct ParsedArgs {
        // flags that received a value, keyed by long name
        std::unordered_map<std::string, std::string> options;
        // flags present without a value, stored by long name
        std::vector<std::string> flags;
        // arguments not tied to any flag
        std::vector<std::string> positionals;
        // tokens that look like flags but were never declared
        std::vector<std::string> unknown;
        bool error = false;
    };

    // A token counts as a flag when it starts with '-' and has more than the dash itself.
    inline bool looksLikeFlag(const std::string &token) {
        return token.size() > 1 && token[0] == '-';
    }

    // Finds a declared flag by its long name (isLong) or its short name. Returns nullptr if none match.
    inline const Flag *findFlag(const std::vector<Flag> &flags, const std::string &name, bool isLong) {
        if (name.empty()) return nullptr;
        for (const Flag &f : flags) {
            if (isLong ? f.verbose == name : f.v == name) return &f;
        }
        return nullptr;
    }

    // Parses arguments (without the program name) against the declared flags.
    // Supports "--name value", "--name=value", "-n value", and "--" to treat everything after it as positional.
    inline ParsedArgs parseArgs(const std::vector<Flag> &flags, const std::vector<std::string> &args) {
        ParsedArgs result;
        bool onlyPositionals = false;

        for (size_t i = 0; i < args.size(); ++i) {
            const std::string &token = args[i];

            if (token.empty()) continue;

            if (onlyPositionals || !looksLikeFlag(token)) {
                result.positionals.push_back(token);
                continue;
            }

            if (token == "--") {
                onlyPositionals = true;
                continue;
            }

            const bool isLong = token[1] == '-';
            std::string name  = token.substr(isLong ? 2 : 1);

            bool hasInlineValue = false;
            std::string inlineValue;
            const size_t eq = name.find('=');
            if (eq != std::string::npos) {
                inlineValue    = name.substr(eq + 1);
                name           = name.substr(0, eq);
                hasInlineValue = true;
            }

            const Flag *flag = findFlag(flags, name, isLong);
            if (flag == nullptr) {
                result.unknown.push_back(token);
                result.error = true;
                continue;
            }

            switch (flag->status) {
                case Flag::ValueStatus::None:
                    if (hasInlineValue) {
                        result.error = true;
                        break;
                    }
                    result.flags.push_back(flag->verbose);
                    break;

                case Flag::ValueStatus::Required:
                    if (hasInlineValue) {
                        result.options[flag->verbose] = inlineValue;
                    } else if (i + 1 < args.size()) {
                        result.options[flag->verbose] = args[++i];
                    } else {
                        result.error = true;
                    }
                    break;

                case Flag::ValueStatus::Optional:
                    if (hasInlineValue) {
                        result.options[flag->verbose] = inlineValue;
                    } else if (i + 1 < args.size() && !looksLikeFlag(args[i + 1])) {
                        result.options[flag->verbose] = args[++i];
                    } else {
                        result.flags.push_back(flag->verbose);
                    }   
                    break;
            }
        }
        return result;
    }
} // namespace albert::args
