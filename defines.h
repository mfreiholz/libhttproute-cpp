#pragma once

/*
How to use classes:
```cpp
MF_NS(route)::Router r;
r.func();
```
*/

#define LIBHTTPROUTE_NS_BEGIN namespace mf { namespace route {
#define LIBHTTPROUTE_NS_END }}
#define MF_NS(X) ::mf::X