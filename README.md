[![CMake on Ubuntu-latest](https://github.com/olivia76/cpp-te/actions/workflows/build_cmake_ubuntu.yml/badge.svg)](https://github.com/olivia76/cpp-te/actions/workflows/build_cmake_ubuntu.yml)
[![CMake on MacOS-latest](https://github.com/olivia76/cpp-te/actions/workflows/build_cmake_macos.yml/badge.svg)](https://github.com/olivia76/cpp-te/actions/workflows/build_cmake_macos.yml)
[![CMake on Windows-2019](https://github.com/olivia76/cpp-te/actions/workflows/build_cmake_windows.yml/badge.svg)](https://github.com/olivia76/cpp-te/actions/workflows/build_cmake_windows.yml)
[![CodeQL](https://github.com/olivia76/cpp-te/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/olivia76/cpp-te/actions/workflows/codeql-analysis.yml)

# cpp-te

## Summary

This is a C++20 implementation of the type erasure + visitor design patterns.

### Classic type erasure implementation

![classic](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/olivia76/cpp-te/main/puml/te.puml)

### VTBL type erasure implementation

![vtbl](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/olivia76/cpp-te/main/puml/te2.puml)


## Example

See [test_package](test_package) directory.

## Performance

Relative measurements of calling `area()` and `perimeter()` on a vector of shapes (margin : ± 0.02)​. The number 1, 2, 3, 4, and 5 represents the number of Shape types​ (Circle, Square, Rectangle, Right Triangle, Rhombus) in the vector.  Benchmark : g++ – 11.4 ; Catch 2 – 100 iterations ; 10'000'000 shapes​

|                                                 |   1  |   2  |   3  |   4  |   5  |
| :---------------------------------------------- | :--: | :--: | :--: | :--: | :--: |
| Type erasure​                                    | 1.00​ | 1.00​ | 1.00​ | 1.00​ | 1.00​ |
| Type erasure + visitor (fct pointer)​            | 1.13​ | 1.04​ | 1.05​ | 1.04​ | 1.03​ |
| Type erasure + visitor (std::function)​          | 1.12​ | 1.06​ | 1.05​ | 1.05​ | 1.04​ |
| Type erasure – vtable​                           | 1.07​ | 0.86​ | 0.84​ | 0.84​ | 0.83​ |
| Type erasure + visitor (fct pointer) – vtable​   | 1.16​ | 0.92​ | 0.92​ | 0.88​ | 0.87​ |
| Type erasure + visitor (std::function) – vtable​ | 1.16​ | 0.96​ | 0.94​ | 0.90​ | 0.90​ |
| std::variant & std::visit​                       | 0.84​ | 0.91​ | 0.88​ | 0.89​ | 0.89​ |
