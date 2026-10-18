# Utils/string

## string.hpp
- Requires: C++20
- Namespace: `albert::str`
- Contains utility functions for string manipulation, trimming, casing, splitting, and joining
- Designed to handle multi-character delimiters and standard string transformations inline

### Usage:
#### How to use the functions
All functions reside inside the `albert::str` namespace. Since they are marked `inline`, you can include the header across multiple files in your project without linker errors.

##### Trimming strings
You can strip characters from the left, right, or both ends of a string. By default, they remove standard whitespace (`" \t\n\r\f\v"`), but you can also pass a custom string of characters to strip.
```cpp
#include <utils/string.hpp>

string text = "   hello world   ";
string cleaned = albert::str::strip(text);   // "hello world"
string lefted  = albert::str::lstrip(text);  // "hello world   "
string righted = albert::str::rstrip(text);  // "   hello world"

// Custom character stripping
string custom = albert::str::strip("---hello---", "-"); // "hello"
```

##### Casing transformations

Convert strings to lowercase, uppercase, or capitalize the first letter while lowercasing the rest.

```cpp
string s = "hElLo WoRlD";
string lower = albert::str::to_lower(s);   // "hello world"
string upper = albert::str::to_upper(s);   // "HELLO WORLD"
string cap   = albert::str::capitalize(s); // "Hello world"
```

##### Checking prefixes, suffixes, and contents

Utilize fast C++20-backed checks for string boundaries and substrings.

```cpp
string filename = "document.txt";

bool is_doc = albert::str::startswith(filename, "doc"); // true
bool is_txt = albert::str::endswith(filename, ".txt");    // true
bool has_cu = albert::str::contains(filename, "ment");    // true
```

##### Splitting and joining strings

Split strings by multi-character delimiters using C++20 ranges under the hood, and join vectors of strings back together.

```cpp
// Splitting
string data = "apple,orange,banana";
vector<string> fruits = albert::str::split(data, ","); 
// fruits contains: {"apple", "orange", "banana"}

// Multi-character delimiter splitting
vector<string> parts = albert::str::split("one<->two<->three", "<->");

// Joining
string joined = albert::str::join(", ", fruits); // "apple, orange, banana"
```

##### Replacing substrings

Replace all occurrences of a target substring with a new string.

```cpp
string text = "I like cats and cats";
string updated = albert::str::replace(text, "cats", "dogs"); 
// "I like dogs and dogs"
```