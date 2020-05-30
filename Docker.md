# Duplo (C/C++/Java Duplicate Source Code Block Finder)

This project is found in GitHub: [https://github.com/dlidstrom/Duplo](https://github.com/dlidstrom/Duplo).

## General Information

Duplicated source code blocks can harm maintainability of software systems.
Duplo is a tool to find duplicated code blocks in large C, C++, Java, C# and
VB.Net systems.

## Maintainer

Duplo was originally developed by Christian
M. Ammann and is now maintained and developed by Daniel
Lidström.

## Usage

Duplo works with a list of files. You can either specify a file that contains the list of files, or you can pass them using `stdin`.

Run `duplo --help` on the command line to see the detailed options.

### Passing files using `stdin`

```bash
# Docker on unix
> find . -type f \( -iname "*.cpp" -or -iname "*.h" \) | docker run --rm -i -w /src -v $(pwd):/src dlidstrom/duplo - out.txt
```

`duplo` will write the duplicated blocks into `out.txt`.

### Passing files using file

`duplo` can analyze files specified in a separate file:

```bash
# Docker on unix
> find . -type f \( -iname "*.cpp" -o -iname "*.h" \) > files.lst
> docker run --rm -i -w /src -v $(pwd):/src dlidstrom/duplo files.lst out.txt
```

Again, `duplo` will write the duplicated blocks into `out.txt`.

## Feedback and Bug Reporting

Please open a GitHub issue to discuss feedback,
feature requests and bug reports.

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
