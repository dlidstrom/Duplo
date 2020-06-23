# 1. Duplo (C/C++/Java Duplicate Source Code Block Finder)

![C/C++ CI](https://github.com/dlidstrom/Duplo/workflows/C/C++%20CI/badge.svg)

- [1. Duplo (C/C++/Java Duplicate Source Code Block Finder)](#1-duplo-ccjava-duplicate-source-code-block-finder)
  - [1.1. General Information](#11-general-information)
  - [1.2. Maintainer](#12-maintainer)
  - [1.3. File Format Support](#13-file-format-support)
  - [1.4. Installation](#14-installation)
    - [1.4.1. Docker](#141-docker)
    - [1.4.2. Pre-built binaries](#142-pre-built-binaries)
  - [1.5. Usage](#15-usage)
    - [1.5.1. Passing files using `stdin`](#151-passing-files-using-stdin)
    - [1.5.2. Passing files using file](#152-passing-files-using-file)
    - [1.5.3. Xml output](#153-xml-output)
  - [1.6. Feedback and Bug Reporting](#16-feedback-and-bug-reporting)
  - [1.7. Algorithm Background](#17-algorithm-background)
    - [1.7.1. Performance Measurements](#171-performance-measurements)
  - [1.8. Developing](#18-developing)
    - [1.8.1. Unix](#181-unix)
    - [1.8.2. Windows](#182-windows)
    - [1.8.3. Additional Language Support](#183-additional-language-support)
    - [1.8.4. Language Suggestions](#184-language-suggestions)
  - [1.9. Changes](#19-changes)
  - [1.10. License](#110-license)

## 1.1. General Information

Duplicated source code blocks can harm maintainability of software systems.
Duplo is a tool to find duplicated code blocks in large C, C++, Java, C# and
VB.Net systems.

## 1.2. Maintainer

Duplo was originally developed by Christian
M. Ammann and is now maintained and developed by Daniel
Lidström.

## 1.3. File Format Support

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

## 1.4. Installation

### 1.4.1. Docker

If you have Docker, the way to run Duplo is to use this command:

```bash
# Docker on unix
> docker run --rm -i -w /src -v $(pwd):/src dlidstrom/duplo
```

This pulls the latest image and runs duplo. Note that you'll have to pipe the filenames into this command. A complete commandline sample will be shown below.

### 1.4.2. Pre-built binaries

Duplo is also available as a pre-built binary for (alpine) linux and macos. Grab the executable from the [releases](https://github.com/dlidstrom/Duplo/releases) page.

You can of course build from source as well, and you'll have to do so to get a binary for Windows.

## 1.5. Usage

Duplo works with a list of files. You can either specify a file that contains the list of files, or you can pass them using `stdin`.

Run `duplo --help` on the command line to see the detailed options.

### 1.5.1. Passing files using `stdin`

```bash
# unix
> find . -type f \( -iname "*.cpp" -o -iname "*.h" \) | duplo - out.txt

# windows
> Get-ChildItem -Include "*.cpp", "*.h" -Recurse | % { $_.FullName } | Duplo.exe - out.txt

# Docker on unix
> find . -type f \( -iname "*.cpp" -or -iname "*.h" \) | docker run --rm -i -w /src -v $(pwd):/src dlidstrom/duplo - out.txt
```

In each of the above commands, `duplo` will write the duplicated blocks into `out.txt` in addition to the information written to stdout.

### 1.5.2. Passing files using file

`duplo` can analyze files specified in a separate file:

```bash
# unix
> find . -type f \( -iname "*.cpp" -o -iname "*.h" \) > files.lst
> duplo files.lst out.txt

# windows
> Get-ChildItem -Include "*.cpp", "*.h" -Recurse |  % { $_.FullName } | Out-File -encoding ascii files.lst
> Duplo.exe files.lst out.txt

# Docker on unix
> find . -type f \( -iname "*.cpp" -o -iname "*.h" \) > files.lst
> docker run --rm -i -w /src -v $(pwd):/src dlidstrom/duplo files.lst out.txt
```

Again, the duplicated blocks are written to `out.txt`.

### 1.5.3. Xml output

Duplo can also output xml and there is a stylesheet that will format the result for viewing in a browser. This can be used as a report tab in your continuous integration tool (TeamCity, etc).

## 1.6. Feedback and Bug Reporting

Please open an issue to discuss feedback,
feature requests and bug reports.

## 1.7. Algorithm Background

Duplo uses the same techniques as Duploc to detect duplicated code blocks. See
[Duca99bCodeDuplication](http://scg.unibe.ch/archive/papers/Duca99bCodeDuplication.pdf) for
further information.

### 1.7.1. Performance Measurements

| System | Files | Loc's | Time |
|-|-|-|-|
| Quake2 | 266 | 102740 | 18sec |

## 1.8. Developing

### 1.8.1. Unix

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
build/> cmake ..
# now issue make
build/> make
build/> popd
# continuous build can now be used in root folder
# (needs fswatch)
> ./watch.sh
```

### 1.8.2. Windows

Use Visual Studio 2019 to open the included solution file (or try `CMake`).

### 1.8.3. Additional Language Support

Duplo can analyze all text files regardless of format, but it has special support for some programming languages (C++, C#, Java, for example). This allows Duplo to improve the duplication detection as it can ignore preprocessor directives and/or comments.

To implement support for a new language, there are a couple of options (in order of complexity):

1. Implement `FileTypeBase` which has support for handling comments and preprocessor directives. You just need to decide what is a comment. With this option you need to implement a couple of methods, one which is `CreateLineFilter`. This is to remove multiline comments. Look at `CstyleCommentsFilter` for an example.
2. Implement `IFileType` interface directly. This gives you the most freedom but also is the hardest option of course.

### 1.8.4. Language Suggestions

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

## 1.9. Changes

- 0.5
  - Fixed malformed xml (thanks [@ArsMasiuk](https://github.com/ArsMasiuk)!)
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

## 1.10. License

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
