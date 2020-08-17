# Duplo (C/C++/Java Duplicate Source Code Block Finder) <!-- omit in toc -->

![C/C++ CI](https://github.com/dlidstrom/Duplo/workflows/C/C++%20CI/badge.svg)

- [1. General Information](#1-general-information)
- [2. Maintainer](#2-maintainer)
- [3. File Format Support](#3-file-format-support)
- [4. Installation](#4-installation)
  - [4.1. Docker](#41-docker)
  - [4.2. Pre-built binaries](#42-pre-built-binaries)
- [5. Usage](#5-usage)
  - [5.1. Passing files using `stdin`](#51-passing-files-using-stdin)
    - [5.1.1. Bash](#511-bash)
    - [5.1.2. Windows](#512-windows)
    - [5.1.3. Docker](#513-docker)
  - [5.2. Passing files using file](#52-passing-files-using-file)
  - [5.3. Xml output](#53-xml-output)
- [6. Feedback and Bug Reporting](#6-feedback-and-bug-reporting)
- [7. Algorithm Background](#7-algorithm-background)
  - [7.1. Performance Measurements](#71-performance-measurements)
- [8. Developing](#8-developing)
  - [8.1. Unix](#81-unix)
  - [8.2. Windows](#82-windows)
  - [8.3. Additional Language Support](#83-additional-language-support)
  - [8.4. Language Suggestions](#84-language-suggestions)
- [9. Changes](#9-changes)
- [10. Accompanying Software](#10-accompanying-software)
- [11. License](#11-license)

## 1. General Information

Duplicated source code blocks can harm maintainability of software systems.
Duplo is a tool to find duplicated code blocks in large C, C++, Java, C# and
VB.Net systems.

## 2. Maintainer

Duplo was originally developed by Christian
M. Ammann and is now maintained and developed by Daniel
Lidström.

## 3. File Format Support

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

## 4. Installation

### 4.1. Docker

If you have Docker, the way to run Duplo is to use this command:

```bash
# Docker on unix
> docker run --rm -i -w /src -v $(pwd):/src dlidstrom/duplo
```

This pulls the latest image and runs duplo. Note that you'll have to pipe the filenames into this command. A complete commandline sample will be shown below.

### 4.2. Pre-built binaries

Duplo is also available as a pre-built binary for (alpine) linux and macos. Grab the executable from the [releases](https://github.com/dlidstrom/Duplo/releases) page.

You can of course build from source as well, and you'll have to do so to get a binary for Windows.

## 5. Usage

Duplo works with a list of files. You can either specify a file that contains the list of files, or you can pass them using `stdin`.

Run `duplo --help` on the command line to see the detailed options.

### 5.1. Passing files using `stdin`

In each of the following commands, `duplo` will write the duplicated blocks into `out.txt` in addition to the information written to stdout.

#### 5.1.1. Bash

```bash
# unix
> find . -type f \( -iname "*.cpp" -o -iname "*.h" \) | duplo - out.txt
```

Let's break this down. `find . -type f \( -iname "*.cpp" -o -iname "*.h" \)` is a syntax to look recursively in the current directory (the `.` part) for files (the `-type f` part) matching `*.cpp` or `*.h` (case insensitive). The output from `find` is piped into `duplo` which then reads the filenames from `stdin` (the `-` tells `duplo` to get the filenames from `stdin`, a common unix convention in many commandline applications). The result of the analysis is then written to `out.txt`.

#### 5.1.2. Windows

```bash
# windows
> Get-ChildItem -Include "*.cpp", "*.h" -Recurse | % { $_.FullName } | Duplo.exe - out.txt
```

This works similarly to the Bash command, but uses PowerShell commands to achieve the same effect.

#### 5.1.3. Docker

```bash
# Docker on unix
> find . -type f \( -iname "*.cpp" -or -iname "*.h" \) | docker run --rm -i -w /src -v $(pwd):/src dlidstrom/duplo - out.txt
```

This command also works in a similar fashion to the Bash command, but instead of piping into a local `duplo` executable, it will pipe into `duplo` running inside Docker. This is very convenient as you do not have to install `duplo` separately. You will have to install Docker though, if you haven't already. That is a good thing to do anyway, since it opens up a lot of possibilities apart from running `duplo`.

Again, similarly to the Bash command, this uses `find` to find files in the current directory, then passes the file list to Docker which will pass it further into an instance of the latest version of `duplo`. The working directory in the `duplo` container should be `/src` (that's where the `duplo` executable is located) and the current path of your host machine will be mapped to `/src` when the container is running. The `-i` allows `stdin` of your host machine to be passed into Docker to allow `duplo` to read the filenames. Any parameters to `duplo` can be placed at the end of the command as you can see `- out.txt` has been.

### 5.2. Passing files using file

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

### 5.3. Xml output

Duplo can also output xml and there is a stylesheet that will format the result for viewing in a browser. This can be used as a report tab in your continuous integration tool (TeamCity, etc).

## 6. Feedback and Bug Reporting

Please open an issue to discuss feedback,
feature requests and bug reports.

## 7. Algorithm Background

Duplo uses the same techniques as Duploc to detect duplicated code blocks. See
[Duca99bCodeDuplication](http://scg.unibe.ch/archive/papers/Duca99bCodeDuplication.pdf) for
further information.

### 7.1. Performance Measurements

| System | Files | Loc's | Time |
|-|-|-|-|
| Quake2 | 266 | 102740 | 18sec |

## 8. Developing

### 8.1. Unix

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

### 8.2. Windows

Use Visual Studio 2019 to open the included solution file (or try `CMake`).

### 8.3. Additional Language Support

Duplo can analyze all text files regardless of format, but it has special support for some programming languages (C++, C#, Java, for example). This allows Duplo to improve the duplication detection as it can ignore preprocessor directives and/or comments.

To implement support for a new language, there are a couple of options (in order of complexity):

1. Implement `FileTypeBase` which has support for handling comments and preprocessor directives. You just need to decide what is a comment. With this option you need to implement a couple of methods, one which is `CreateLineFilter`. This is to remove multiline comments. Look at `CstyleCommentsFilter` for an example.
2. Implement `IFileType` interface directly. This gives you the most freedom but also is the hardest option of course.

### 8.4. Language Suggestions

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

## 9. Changes

- 0.6
  - Improved integration with [duploq](https://github.com/ArsMasiuk/duploq)
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

## 10. Accompanying Software

For a pretty ui you should check out [duploq](https://github.com/ArsMasiuk/duploq) by [ArsMasiuk](https://github.com/ArsMasiuk):

![Duploq](https://github.com/ArsMasiuk/duploq/raw/master/duploq.png)

From duploq's Readme file:

> duploq's approach is a pretty straighforward. First, duploq allows you to choose where to look for the duplicates (files or folders). Then it builds list of input files and passes it to the Duplo engine together with necessary parameters. After the files have been processed, duploq parses Duplo's output and visualises the results in easy and intuitive way. Also it provides additional statistics information which is not a part of Duplo output.

## 11. License

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
