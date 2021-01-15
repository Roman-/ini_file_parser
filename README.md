# ini_parser
Simple single-header ini/cfg file parser for C++.

# cfg file example
This is an example of cfg/ini file that could be parsed with ini_parser
```
[example]
test=I am test string
i = 42
f = 3.1415926
d = 0.123456789

[features]
# sections are ignored
# comments (that starts with hash) are also ignored
# keys and values are trimmed (left and right)
  extra_spaces  = not a big deal
# only first '=' sign matters
test_string_with_eq = 2+2=4
# last keys overwrite first
x=2
# will be overwritten by
x=4
# generally, all lines are ignored as long as there is no '=' sign
this line will be ignored
# strings may be enclosed in quotes
my_string="I am a string"
# the above is equivalent to
my_string=I am a string
```

# Usage
```c
#include <ini_parser.h>
int main() {
    CfgParser cfg("/path/to/cfg.ini");
    bool isValid = cfg.isValid(); // will return false if file does not exist
    bool hasKeyTest = cfg.hasKey("test"); // true if key "test" exists regardless of type
    std::string testStr = cfg.getString("test"); // may throw std::out_of_range if key "test" is not in .ini file, so always surround with try/catch and check with .hasKey("test") inb4
    int i = cfg.getInt("i"); // may throw std::out_of_range and std::invalid_arg as it is converting string to int with std::stoi
    float f = cfg.getFloat("f"); // same with stof
    double d = cfg.getDouble("d"); // same with stod
    std::cout << "testStr = " << testStr << "; i = " << i << "; f = " << f << "; d = " << d << std::endl;

    // you may specify required keys in cfg like so:
    CfgParser cfgWithRequiredKeys("/path/to/cfg.ini", {"absentKey1", "absentKey2"});
    // if either of these keys is not defined in cfg file, then
    cfgWithRequiredKeys.isValid(); // will return false
}
```
output:
```
testStr = I am test string; i = 42; f = 3.14159; d = 0.123456789
```


# install
Add this to your CMakeLists.txt:
```
include(FetchContent)

FetchContent_Declare(ini_parser
    GIT_REPOSITORY https://github.com/Roman-/ini_file_parser.git
    GIT_TAG        1.2 # replace this with the latest tag
)
FetchContent_MakeAvailable(ini_parser)
include_directories(${ini_parser_SOURCE_DIR})
```

