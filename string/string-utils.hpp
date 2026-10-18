#include <algorithm>
#include <ranges>
#include <string>
#include <vector>

using std::string, std::vector;

namespace albert::str
{
    inline string lstrip(string s, const string &chars = " \t\n\r\f\v") {
        s.erase(0, s.find_first_not_of(chars));
        return s;
    }

    inline string rstrip(string s, const string &chars = " \t\n\r\f\v") {
        auto last = s.find_last_not_of(chars);
        if (last == string::npos) return "";
        s.erase(last + 1);
        return s;
    }

    inline string strip(string s, const string &chars = " \t\n\r\f\v") {
        return lstrip(rstrip(s, chars), chars);
    }

    inline string to_lower(string s) {
        for (char &c : s) c = tolower(c);
        return s;
    }

    inline string to_upper(string s) {
        for (char &c : s) c = toupper(c);
        return s;
    }

    inline string capitalize(string s) {
        if (s.empty()) return s;
        s[0] = toupper(s[0]);
        for (size_t i = 1; i < s.length(); ++i) s[i] = tolower(s[i]);
        return s;
    }

    inline bool startswith(const string &s, const string &prefix) {
        return s.starts_with(prefix);
    }

    inline bool endswith(const string &s, const string &suffix) {
        return s.ends_with(suffix);
    }

    inline bool contains(const string &s, const string &sub) {
        return s.find(sub) != string::npos;
    }

    inline vector<string> split(const string &s, const string &delimiter = " ") {
        vector<string> tokens;
        if (delimiter.empty()) {
            tokens.push_back(s);
            return tokens;
        }
        for (auto &&part : s | std::views::split(delimiter)) {
            tokens.emplace_back(part.begin(), part.end());
        }
        return tokens;
    }

    inline string replace(string s, const string &from, const string &to) {
        if (from.empty()) return s;
        size_t pos = 0;
        while ((pos = s.find(from, pos)) != string::npos) {
            s.replace(pos, from.length(), to);
            pos += to.length();
        }
        return s;
    }

    inline string join(const string &delimiter, const vector<string> &vec) {
        string result = "";
        for (size_t i = 0; i < vec.size(); ++i) {
            result += vec[i];
            if (i + 1 < vec.size()) result += delimiter;
        }
        return result;
    }
} // namespace albert::str