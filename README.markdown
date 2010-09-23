Duplo (C/C++/Java Duplicate Source Code Block Finder)

# GENERAL INFORMATION

Duplicated source code blocks can harm maintainability of software systems.
Duplo is a tool to find duplicated code blocks in large C, C++, Java, C# and 
VB.Net systems.

Sample output snippet
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

## OPTIONS

Run "duplo --help" on the command line to see the detailed options.

## FEEDBACK AND BUG REPORTING

Please send Feedback and Bug Reports to: dlidstrom@gmail.com

## FILE LIST GENERATION

Generate a list of all files of a directory with:

- Windows
    dir /s /b /a-d *.cpp *.h > files.lst

- UNIX
    find -name "*.cpp" > cppList.lst
    find -name "*.h" > includeList.lst
    cat cppList.lst includeList.lst > all.lst
---

# PERFORMANCE MEASUREMENTS

    SYSTEM                  FILES     LOCs   TIME   HARDWARE
    3D Game Engine            275    12211   4sec   3.4GHZ P4
    Quake2                    266   102740  58sec   3.4GHZ P4
    Computer Game            5639   754320  34min   3.4GHZ P4
    Linux Kernel 2.6.11.10  17034  4184356  16h     3.4GHZ P4
---

# BACKGROUND

Duplo uses the same techniques as Duploc to detect duplicated code blocks. See
[Duca99bCodeDuplication](http://www.iam.unibe.ch/~scg/Archive/Papers/Duca99bCodeDuplication.pdf) for
further information.

---

# LICENSE

Duplo is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Foobar is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Duplo; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
---

2005, Christian M. Ammann (cammann@giants.ch)
2010, Daniel Lidstrom     (dlidstrom@gmail.com)
