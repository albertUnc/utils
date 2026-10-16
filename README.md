# Utils
By albert

## What is Utils?
This is a collection of small utilities in form of .hpp files (to use in your c++ programs) and some python scripts/compiled apps. The compiled apps have the source file in here too, everything else is includable files or python (scripts, open source by default) which you need the python 3 or newer interpreter to run.

## Upcoming in the updates
- Organizing all of these into folders
- Making a general namespace (like albert::) for all of the c++ (h++) files (inside that each file will still have its own namespace)
- Adding new stuff to time, log, and platform .hpp files
- Adding some other tools i made that aren't here
- Making a seperate documentation for each folder's files after organized and only having a guide on how to navigate them in the readme


## Tools documentation
#### This section has the description in detail (documentation/usage) to all the tools
 
# C++ tools
### clear.hpp
- Needs base c++, no other dependencies
- What it does: Can clear a terminal screen and wait for a user continue input then clear it and move on

Usage:
```cpp
clear::clearScreen(); // sends a clear (or cls on windows) command to the terminal the program is running in, making it clear the screen, then returns. Your program can continue

clear::hold("customPrompt"); // shows the custom prompt (intended to be something like "press enter to continue"), then waits for the user to press enter, clears the screen and returns.
clear::hold(); // with no arguments: uses the HOLD_PROMPT variable (see below) instead of a custom one
clear::HOLD_PROMPT = "something"; // The prompt that will be output when using clear::hold
clear::changeHoldPrompt("str"); // Changes the hold prompt, essentially the same as manually reassigning it
```

### platform.hpp
- Needs base c++, no other dependecies
- Declares a RunningPlatform enum
- Has a function for the platform (ret type RunningPlatform)
- Has a function to get the path to the folder for your app's data (user appdata folder based on platform + your app name) (ret type std::filesystem::path)
- Uses namespace `fs` for `std::filesystem`, this will be a thing in all code you include the file in

Usage:
```cpp
std::string platform::APP_NAME = "MyAppsName"; // This will be used to make the path to the folder where you have to store your app's data, if you ONLY need the platform it's useless.
void platform::setAppName(std::string name); // Sets the name, essentially the same as manually re signing
enum class platform::RunningPlatform {}; // Contains Windows, Mac, Linux and Other
platform::RunningPlatform platform::getPlatform(); // Returns a RunningPlatform type var, detecting it at runtime
fs::path platform::appDataPath(); // Returns the absolute path to your app's appdata folder INCLUDING THE MOST BOTTOM CHILD FOLDER WITH YOUR APP'S NAME, so not the general appdata folder only
```