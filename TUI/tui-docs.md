# Utils/TUI

## clear.hpp
- Requires: C++98 or newer
- It can clear the screen of a terminal app (like the `clear` command)
- It can wait for user input before continuing (like pressing enter to continue)

### How to clear the terminal
Use `clear::clearScreen();`. That's it, it will execute the clear immediately. This works by checking the OS and sending a clear or cls command that clears the terminal. It doesn't actually clear the screen as the name would suggest, only the text in the terminal (so new text appears at the top, unlike if we just sent 100 newlines to 'clear it')

### Waiting for user input to continue
The default way is `clear::hold();`, if this is called with no arguments, it's gonna use the HOLD_PROMPT variable as a prompt. To change that, you can overwrite it with `clear::HOLD_PROMPT = "";` or with `clear::changeHoldPrompt("");`. The default prompt it will set this to is "Press ENTER to continue\n".

The alternative way: if you want to call it with a custom prompt but you only need that prompt 1 time, you can call it with a string argument: `clear::hold("something");`. This will not change your prompt stored in `HOLD_PROMPT`, it will use your argument only once and then later if called with no arguments, it will jump back to the saved one.

**Reset the prompt**: You can call `clear::resetHoldPrompt();` which will set it again to the default one ("Press ENTER to continue\n")

## terminal_input.hpp
- Requires: C++11 or newer
- It can collect user input from terminal that is strictly an int or a float between a min and max range

### Collect input
#### ints
Use `input::getUserInput();`. This returns an `int`, calling it with no arguments will cause the minimum to be 0 and the maximum to be the biggest number an int can be (`std::numeric_limits<int>::max()`).

You can call this with arguments too, first a min and a max value, like `int num = albert::input::getUserInput(-2, 3);`, this will ask the user over and over again for a number until it enters something valid from -2 to 3.

#### floats
Use `getFloatInput()`. This takes the same arguments as the int one, but it takes `float` objects, not ints. It also returns a `float`. Essentially the same but with decimals

#### Why their namings don't match
getUserInput was a function i created long ago and used in many programs, and getFloatInput was added just now. Renaming getUserInput to something like getUserIntInput would cause all of my code already using it to break and need a lot of manual fixes. Making an overload of it wouldn't always work as inteded (because if called with no arguments it can't decide which one to use)