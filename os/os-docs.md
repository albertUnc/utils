# Utils/os

## platform.hpp
- Requires: C++17 or newer (it uses `inline` variables and `std::filesystem`)
- Namespace: `albert::platform`
- Uses `fs` for `std::filesystem`
- Can tell you which OS your program was compiled for (Windows/Mac/Linux/Other)
- Can get the right folder for your app's data on each OS (like `%APPDATA%` on Windows)

### Usage:
#### How to get the platform
Use `platform::getPlatform();`, it returns a `RunningPlatform` enum which is one of `Windows`, `Mac`, `Linux` or `Other`. This is decided at compile time with preprocessor macros (`_WIN32`, `__APPLE__`, `__linux__`), so it tells you what the program was built for, not something detected while running.
```cpp
#include <utils/os/platform.hpp>

using albert::platform::RunningPlatform;

if (albert::platform::getPlatform() == RunningPlatform::Windows) {
    // windows only stuff
}
```

#### How to get the app data folder
Use `platform::appDataPath();`, it returns an `fs::path` to the folder where your app should store its data (settings, logs, saves...). Where that is depends on the OS:
- Windows: `%APPDATA%/<APP_NAME>`
- Mac: `~/Library/Application Support/<APP_NAME>`
- Linux: `$XDG_DATA_HOME/<APP_NAME>`, or if that isn't set, `~/.local/share/<APP_NAME>`
- Other (or if the needed environment variable doesn't exist): `./<APP_NAME>`, a folder in the current directory

**It only gives you the path, it does NOT create the folder.** You have to make it yourself with `fs::create_directories(path);` before writing to it. A good place to use it is in the function that makes your stream for [log.hpp](../file-io/log.hpp).

#### How to set the app name
The folder name comes from `platform::APP_NAME`, which is `"MyApp"` by default. Change it at the start of your program with `platform::setAppName("MyGame");` (or just overwrite the variable with `platform::APP_NAME = "MyGame";`). You need to do this before calling `appDataPath()`, otherwise you will get the path with the default name.
```cpp
albert::platform::setAppName("MyGame");
fs::path dataDir = albert::platform::appDataPath();
fs::create_directories(dataDir);
```

## time.hpp
- Requires: C++20 (it uses `std::chrono` time zones and `std::format`)
- Namespace: `albert::time`
- Contains small time helpers, mostly made for [log.hpp](../file-io/log.hpp)

### Usage:
#### Getting the current time as seconds
Use `time::getNowSeconds();`, it returns a `long long` with the seconds passed since the Unix epoch (1 January 1970 UTC).

#### Getting a readable time for logs
Use `time::currentMinute();`, it returns a `std::string` with the current local time in the `HH:MM:SS` format, like `18:37:54`. Even though it's called `currentMinute` it also includes the seconds. It's meant for logs only, which is what `Log::write` uses it for.
```cpp
#include <utils/os/time.hpp>

long long now = albert::time::getNowSeconds();   // 1767225600
std::string stamp = albert::time::currentMinute(); // "18:37:54"
```
