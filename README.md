# fastply [![Build Status](https://travis-ci.org/dbadrian/fastply.svg?branch=develop)](https://travis-ci.org/dbadrian/fastply)

This single-include header file provides fast sequential/random read access to larger-than-memory PLY files. No framework specific types are forced on you and elements are simply represented by their equivalent C/C++ struct definition - no conversions happening! By memory-mapping files it is up to the user what to load and store in memory.

What's the catch? You need to know the definition of all elements at compile time (with the exception of how many entries per element type there are). Currently, POSIX only (tested on linux/osx), little-endian binary only (for now), and C++17 standard is required. And read-only (for now).

If you check one or more of these, maybe fastply is for you:
  - [ ] Very fast sequential and random read-only access to binary PLY files
  - [ ] Your PLY files are (much) larger than the available memory
  - [ ] Framework independent PLY reader library (type conversions are up to the user)
  - [ ] Barebones (~250LOC), header only library with a permissive license (MIT) without any dependenies, but STL


# Why another PLY reader?
While there is a large amount of (C++) PLY parsers, most fall into one of two categories (or both):
  * they are very generic and make no assumption about the structure of your PLY files (category I)
  * and/or are heavily focused on a specific environment (category II)

While category I/the ability to read arbitrary PLY files (which conform to the standard) can be a big bonus, often you know exactly what types you will read in (and in C++ you need to define it anyway). PLY readers of category II have been developed inside a larger framework (think PCL) and thus often cater to the needs of that framework. Inclusion of such a massive third-party library, when all you needs is to read a ply file, is prohibitively. Also, often it comes with a lot of type conversion and potentially unnecessary allocations. With fastply's approach you will read in about 50-100x faster than other libraries.

Last but not least, most PLY readers read a PLY file in its entirety into memory. When working with PLY files larger than memory (...well maybe you shouldn't) this is more than problematic.

