# Utils/list

## list_utils.hpp
- Requires: C++11 or newer
- Namespace: `albert::list`
- Contains small helpers for `std::vector`, for now only a "is this value in the list" check (like `in` from python)

### Usage:
#### Checking if a value is in a vector
Use `list::isIn(value, vector)`. It returns `true` if the value is somewhere in the vector, `false` if not. It works with any type that can be compared with `==`.
```cpp
#include <utils/list/list_utils.hpp>

std::vector<int> nums = {1, 2, 3};
bool a = albert::list::isIn(2, nums); // true
bool b = albert::list::isIn(7, nums); // false

std::vector<std::string> names = {"anna", "bob"};
bool c = albert::list::isIn(std::string("bob"), names); // true
```

#### Note about string literals
The value and the vector have to be the same type, so with a `std::vector<std::string>` you can't pass a plain `"bob"` (that's a `const char *`, and the compiler won't know if `T` is that or `std::string`). Wrap it in `std::string("bob")` like above.
