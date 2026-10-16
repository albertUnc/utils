#include <filesystem>
#include <string>
#include <cstdlib>
namespace fs = std::filesystem;

namespace platform
{

inline std::string APP_NAME = "MyApp";
inline void setAppName(std::string name) { APP_NAME = name; }

enum class RunningPlatform {
    Windows,
    Mac,
    Linux,
    Other
};

inline const RunningPlatform getPlatform() {
    #if defined(_WIN32)
        return RunningPlatform::Windows;
    #elif defined(__APPLE__)
        return RunningPlatform::Mac;
    #elif defined(__linux__)
        return RunningPlatform::Linux;
    #else 
        return RunningPlatform::Other;
    #endif
}

    inline fs::path appDataPath() {
	    switch (getPlatform()) {
	        case RunningPlatform::Windows: {
	            const char* env = std::getenv("APPDATA");
	            if (env) {
	                return fs::path(env) / APP_NAME;
	            }
	            break;
	        }

	        case RunningPlatform::Mac: {
	            const char* env = std::getenv("HOME");
	            if (env) {
	                return fs::path(env) / "Library/Application Support" / APP_NAME;
	            }
	            break;
	        }

	        case RunningPlatform::Linux: {
	            const char* xdg = std::getenv("XDG_DATA_HOME");
	            if (xdg) {
	                return fs::path(xdg) / APP_NAME;
	            }

	            const char* home = std::getenv("HOME");
	            if (home) {
	                return fs::path(home) / ".local/share" / APP_NAME;
	            }
	            break;
	        }

			case RunningPlatform::Other : ;
	    }

	    return fs::path(".") / APP_NAME;
	}

}