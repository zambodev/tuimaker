# libtui
A cross platform library that aims to simplify TUIs creation

<br>

## How to use

**Linux**:

Dwonload the [latest release](https://github.com/zambodev/libtui/releases/latest)

Add include/ and lib/ in the project folder

Include in files:
```cpp
// C++ example

#include <tui.hpp>

int main()
{
	...
}
```
Link to the project:
```bash
c++ -o <output> <your_files> -ltui -Llib -Iinclude
```
<br>

## Build from source
Clone the repository [link](https://github.com/zambodev/libtui.git)

Compile and create the static archive
```bash
make
```
