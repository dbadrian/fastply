# fastply

A barebones PLY reader (=read-only) that mmap()s your input,  for a lightweight and fast random access to larger-than-memory PLY files ... which right now is a buggy prototype ;).

NOTE: Under development, not stable (default = develop branch)!

Status | Compiler |
-------- | ------------ |
[![Build Status](https://travis-ci.org/dbadrian/fastply.svg?branch=develop)](https://travis-ci.org/dbadrian/fastply) | GCC 7.0, Clang 3.9 - 4.0 - 5.0 |

# Why fastply (..in the future)
If you check one or more of these, maybe fastply is for you:
  - [ ] Small, header only library with a permissive license (MIT) without any dependency, but STL
  - [ ] You need to read PLY files very fast (sequential or random access)
  - [ ] Your PLY files are largar than available memory (but you got reasonably fast disks)
  - [ ] You know at compile-time what the layout of your PLY files looks like
  - [ ] Your PLY files are uniform (no elements using lists with varying lengths per entry)
    - In other words, if all elements can be described by a 'struct' with fixed size, fastply will be able to parse it.

# Why another PLY reader?
While there is a large amount of (C++) PLY parsers, most fall into one of two categories (or both):
  * they try to solve either every single situation (category I)
  * and/or are heavily focused on a specific environment (category II)

Readers of category I can read arbitrary PLY files (which conform to the standard). This means no optimizations (by the coder or the compiler) can be made ahead of time. For prototyping, testing, and general-purpose projects this is sufficient. Readers of category II have been developed inside a larger framework (think PCL) and thus often cater to the needs of that framework. Inclusion of such a massive third-party library, when all you needs is to read a ply file, is prohibitively.

More over, some PLY readers read a PLY file in its entirety into memory. When working with PLY files larger than memory (...well maybe you shouldn't) this is more than problematic.

