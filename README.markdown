# Duplo (C/C++/Java Duplicate Source Code Block Finder) <!-- omit in toc -->

- [General Information](#general-information)
  - [Maintainer](#maintainer)
- [File Format Support](#file-format-support)
- [Usage](#usage)
  - [Passing files using `stdin`](#passing-files-using-stdin)
  - [Passing files using file](#passing-files-using-file)
  - [Xml output](#xml-output)
- [Feedback and Bug Reporting](#feedback-and-bug-reporting)
- [Algorithm Background](#algorithm-background)
  - [Performance Measurements](#performance-measurements)
- [Developing](#developing)
  - [Unix](#unix)
  - [Windows](#windows)
  - [Additional Language Support](#additional-language-support)
  - [Language Suggestions](#language-suggestions)
- [Changes](#changes)
- [License](#license)

## General Information

Duplicated source code blocks can harm maintainability of software systems.
Duplo is a tool to find duplicated code blocks in large C, C++, Java, C# and
VB.Net systems.

### Maintainer

Duplo was originally developed by Christian
M. Ammann and is now maintained by Daniel
Lidström.

## File Format Support

Duplo has built in support for the following
file formats:

- C/C++ (.c, .cpp, .cxx, .h, .hpp)
- Java
- C#
- VB
- GCC assembly

This means that Duplo will remove
preprocessor directives, block comments, using
statements, etc, to only consider duplicates
in actual code.
In addition, Duplo can be used as a general
(without special support) duplicates detector
in arbitrary text files and will even detect
duplicates found in the same file.

Sample output snippet:

```txt
...
src\engine\geometry\simple\TorusGeometry.cpp(56)
src\engine\geometry\simple\SphereGeometry.cpp(54)
    pBuffer[currentIndex*size+3]=(i+1)/(float)subdsU;
    pBuffer[currentIndex*size+4]=j/(float)subdsV;
    currentIndex++;
    pPrimitiveBuffer->unlock();

src\engine\geometry\subds\SubDsGeometry.cpp(37)
src\engine\geometry\SkinnedMeshGeometry.cpp(45)
    pBuffer[i*size+0]=m_ct[0]->m_pColors[i*3];
    pBuffer[i*size+1]=m_ct[0]->m_pColors[i*3+1];
    pBuffer[i*size+2]=m_ct[0]->m_pColors[i*3+2];
...
```

## Usage

Duplo works with a list of files. You can either specify a file that contains the list of files, or you can pass them using `stdin`.

Run `duplo --help` on the command line to see the detailed options.

### Passing files using `stdin`

```bash
# unix
> find . \( -iname "*.cpp" -o -iname "*.h" \) | duplo - out.txt

# windows
> Get-ChildItem -Include "*.cpp", "*.h" -Recurse | % { $_.FullName } | Duplo.exe - out.txt
```

`duplo` will write the duplicated blocks into `out.txt`.

### Passing files using file

`duplo` can analyze files specified in a separate file:

```bash
# unix
> find . -type f \( -iname "*.cpp" -o -iname "*.h" \) > files.lst
> duplo files.lst out.txt

# windows
> Get-ChildItem -Include "*.cpp", "*.h" -Recurse |  % { $_.FullName } | Out-File -encoding ascii files.lst
> Duplo.exe files.lst out.txt
```

### Xml output

Duplo can also output xml and there is a stylesheet that will format the result for viewing in a browser. This can be used as a report tab in your continuous integration tool (TeamCity, etc).

## Feedback and Bug Reporting

Please open an issue to discuss feedback,
feature requests and bug reports.

## Algorithm Background

Duplo uses the same techniques as Duploc to detect duplicated code blocks. See
[Duca99bCodeDuplication](http://scg.unibe.ch/archive/papers/Duca99bCodeDuplication.pdf) for
further information.

### Performance Measurements

| System | Files | Loc's | Time |
|-|-|-|-|
| Quake2 | 266 | 102740 | 18sec |

## Developing

### Unix

You need `CMake` and preferrably `fswatch` for the best experience.

```bash
# build dependencies
/> brew install cmake
/> brew install fswatch
```

Compiling is best done using the continuous file watcher:

```bash
# CMake builds in the build folder
/> mkdir build
/> pushd build
build /> cmake ..
# now issue make
build /> make
build /> popd
# continuous build can now be used in root folder
# (needs fswatch)
> ./watch.sh
```

### Windows

Use Visual Studio 2019 to open the included solution file (or try `CMake`).

### Additional Language Support

Duplo can analyze all text files regardless of format, but it has special support for some programming languages (C++, C#, Java, for example). This allows Duplo to improve the duplication detection as it can ignore preprocessor directives and/or comments.

To implement support for a new language, there are a couple of options (in order of complexity):

1. Implement `FileTypeBase` which has support for handling comments and preprocessor directives. You just need to decide what is a comment. With this option you need to implement a couple of methods, one which is `CreateLineFilter`. This is to remove multiline comments. Look at `CstyleCommentsFilter` for an example.
2. Implement `IFileType` interface directly. This gives you the most freedom but also is the hardest option of course.

### Language Suggestions

- JavaScript (easy, just look at the existing C-based ones)
- Ruby
- Python
- Perl
- PHP
- Rust
- F#
- Scala
- Haskell
- Erlang
- What else?

Send me a pull request!

## Changes

- 0.4
  - Significant performance improvements
  - Using modern C++ techniques
  - Modularized to simplify adding support of new file formats
  - Can pass files using `stdin`
- 0.3
  - Updated links in html output to GitHub
  - Support for gcc assembly (.s)
  - Fixed minimum number of lines in analysis
  - Fixed limitation of total number of lines of code
  - Checking of arbitrary files

## License

Duplo is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Duplo is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Duplo; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

---
