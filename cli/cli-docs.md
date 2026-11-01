# Utils/cli

## args.hpp
- Requires: C++14 or newer (the `Flag` struct is built with `{}` and has a default member value)
- Namespace: `albert::args`
- A small command line argument parser, you declare your flags and it sorts the arguments into options, flags, positionals and unknown ones
- Supports `--name value`, `--name=value`, `-n value`, `-n=value`, and `--` to treat everything after it as positional
- Doesn't touch `argc`/`argv` directly, you give it a `std::vector<std::string>` of the arguments (without the program name)

### Usage:
#### How to declare flags
A flag is the `Flag` struct, with 3 members: the long name (`verbose`), the short name (`v`) and a status. Names are written WITHOUT dashes, so `"output"` and `"o"` mean `--output` and `-o`. (`verbose` here just means the long name, it has nothing to do with a verbosity flag.)

The status says how the flag handles values:
- `Flag::ValueStatus::None`: the flag takes no value (like `--help`)
- `Flag::ValueStatus::Required`: the flag needs a value (like `--output file.txt`)
- `Flag::ValueStatus::Optional`: the flag can have a value, or be used alone

You put them in a `std::vector<Flag>`:
```cpp
#include <utils/cli/args.hpp>

using namespace albert::args;

std::vector<Flag> flags = {
    {"help",   "h", Flag::ValueStatus::None},
    {"output", "o", Flag::ValueStatus::Required},
    {"level",  "l", Flag::ValueStatus::Optional},
};
```

#### How to parse
Use `parseArgs(flags, args)`, where `args` is a `std::vector<std::string>` of the arguments **without the program name**. From `main` you can build it like this:
```cpp
int main(int argc, char **argv) {
    std::vector<std::string> args(argv + 1, argv + argc);
    ParsedArgs parsed = parseArgs(flags, args);
}
```

#### How to read the result
`parseArgs` returns a `ParsedArgs` struct:
- `options`: an `std::unordered_map<std::string, std::string>` with the flags that got a value, the key is always the **long name**
- `flags`: an `std::vector<std::string>` with the flags that were used without a value, stored by long name
- `positionals`: an `std::vector<std::string>` with the arguments that aren't tied to any flag (like a file name)
- `unknown`: an `std::vector<std::string>` with the tokens that look like flags but you never declared
- `error`: a `bool` which is `true` if something went wrong

The short and long version of a flag give the same result, so `-o file.txt` and `--output file.txt` both end up as `options["output"]`.
```cpp
// program --output=out.txt -l 3 input.txt --help
if (parsed.error) {
    // something was wrong, parsed.unknown has the undeclared flags
}
if (parsed.options.count("output")) {
    std::string out = parsed.options["output"]; // "out.txt"
}
bool help = albert::list::isIn(std::string("help"), parsed.flags); // true (needs list_utils.hpp)
// parsed.positionals contains: {"input.txt"}
```

#### When `error` is set to true
- A flag that you didn't declare is used (it's also added to `unknown`)
- A `None` flag is given a value (`--help=1`)
- A `Required` flag has no value (it's the last argument and has nothing after it)

The parser keeps going after an error, so you still get everything it managed to parse.

#### Things to keep in mind
- Any token starting with `-` (and longer than just `-`) is treated as a flag, so a negative number like `-5` is NOT a positional or a value you can give with a space. Use `--level=-5` instead.
- A `Required` flag always takes the next argument as its value, even if it looks like a flag (`--output --help` makes `--help` the output name).
- An `Optional` flag only takes the next argument if it doesn't look like a flag.
- Short flags can't be combined, `-abc` is read as one flag named `abc`, not `-a -b -c`.
- Empty arguments (`""`) are skipped.

#### Helper functions
- `looksLikeFlag(token)`: returns `true` if the token starts with `-` and has more than just that dash. This is what the parser uses to decide if something is a flag.
- `findFlag(flags, name, isLong)`: finds a declared flag by its long name (`isLong = true`) or short name, and returns a `const Flag *`, or `nullptr` if there's no match.
