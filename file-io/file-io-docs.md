# Utils/file-io

## log.hpp
- Requires: C++20, [time.hpp](../os/time.hpp)
- Makes a MessageTpye enum (info/warn/error/fatal)
- Uses `fs` for `std::filesystem`
- Can write log messages to a selected file like `[18:37:54] main.cpp/main() [INFO] Did ... and ...`
- Has stack-scoping for names (you can name each function and when it returns the name/signature will change back to the parent/caller function)
- Can get set an std::ofstream or try to load it from an fs::path
- One statement to set up the name scope at each function, one for each write
- Writes info by default, can specify warn, error or fatal

### Usage:
#### How to set up a scope
The class is `Log::SignatureScope` which uses a constructor, it needs to get passed you Log object in which it will make the scope, and the name for the scope. For example in a file
```cpp
#include <utils/file-io/log.hpp>

Log logs;

void foo() {
    Log::SignatureScope scope(logs, "main.cpp/foo()");
}
```
Calling logs.write after this will pass the name "main.cpp/foo()" in the line it writes in this function, and when the function returns, it will delete that name and keep writing the parent/caller function's name

#### How to write to a log object
The function is `void Log::write(const std::string &message, const MessageType &type = MessageType::Info);`, so all you need to pass is a message as a string and optinally a message type which will default to info if passed none, for example let's say in a program you declared `Log logs;`, you can write an info message to it with `logs.write("SomeInfo\n");` or a warning, error, or fatal message with `logs.write("SomeWarning idk\n", MessageType::Warn);`

Note that the warning memeber in the MessageType enum is NOT Warning, it's Warn. 

#### How to create a log object
The Log class/object does not have a constructor, so you have to manually make it with `Log logs;` or any name instead of logs, but i like to use that (recommendation: don't name it `log` because that is/is gonna be mistaken for the `log()` function in `<cmath>`), then you have to manually give it a path or a stream.

##### The intended way: give it an std::ofstream
You might want to create your own function, like `std::ofstream makeLogs()`, which handles the path from your settings or hovewer you like, makes sure it has the parent folders and permissions needed, and returns the ofstream which you can pass into `Log::setStream(std::ofstream)`. For example: `logs.setStream(makeLogs());`, or you can directly construct a stream with `logs.setStream(std::ofstream("path"));`, but this is risky since if path's parent folder doesn't exist or you have no permissions it will fail

##### The easy/not that safe way: give it a path
You can use `Log::loadStream(fs::path, bool clear)`, which is a bool function. It will return `true` if the stram was succesfully loaded and the file got opened, and `false` if the parent path doesn't exist or for some other reason it failed to open the file. This is okay to use if you just set up a log object once at the start of your program and use it

##### A note for multi-file use:
You should have a header file that includes log.hpp, and in there you should declare a `Log logs;` or any name. After that, you should include this file in all the files from your multi-file project where you want to use the logging system, and always use the same `logs` or whatever Log object you created. Do not create a seperate object for every file or function, as you will need to re-load the stream and re-make the signature scope which will make you do more work and make extra lag. Only make a new SignatureScope for each function/file (however you like)