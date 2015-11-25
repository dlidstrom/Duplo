/**
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Duplo.h"

#include <fstream>
#include <time.h>

#include <algorithm>
#include <cstring>

#include "SourceFile.h"
#include "SourceLine.h"

#include "StringUtil.h"
#include "HashUtil.h"
#include "TextFile.h"
#include "ArgumentParser.h"

Duplo::Duplo(
    const std::string& listFileName, 
    unsigned int minBlockSize, 
    unsigned int blockPercentThreshold,
    unsigned int minChars, 
    bool ignorePrepStuff, bool ignoreSameFilename, bool Xml) :
    m_listFileName(listFileName),
    m_minBlockSize(minBlockSize),
    m_blockPercentThreshold(blockPercentThreshold),
    m_minChars(minChars),
    m_ignorePrepStuff(ignorePrepStuff),
    m_ignoreSameFilename(ignoreSameFilename),
    m_maxLinesPerFile(0),
    m_DuplicateLines(0),
    m_Xml(Xml),
    m_pMatrix(NULL)
{
}

Duplo::~Duplo(){
    delete[] m_pMatrix;
    m_pMatrix = NULL;
}

void Duplo::reportSeq(int line1, int line2, int count, SourceFile* pSource1, SourceFile* pSource2, std::ostream& outFile){
    if (m_Xml)
    {
        outFile << "    <set LineCount=\"" << count << "\">" << std::endl;
        outFile << "        <block SourceFile=\"" << pSource1->getFilename() << "\" StartLineNumber=\"" << pSource1->getLine(line1)->getLineNumber() << "\"/>" << std::endl;
        outFile << "        <block SourceFile=\"" << pSource2->getFilename() << "\" StartLineNumber=\"" << pSource2->getLine(line2)->getLineNumber() << "\"/>" << std::endl;
        outFile << "        <lines xml:space=\"preserve\">" << std::endl;
        for(int j = 0; j < count; j++)
        {
            // replace various characters/ strings so that it doesn't upset the XML parser
            std::string tmpstr = pSource1->getLine(j+line1)->getLine();

            // " --> '
            StringUtil::StrSub(tmpstr, "\'", "\"", -1);

            // & --> &amp;
            StringUtil::StrSub(tmpstr, "&amp;", "&", -1);

            // < --> &lt;
            StringUtil::StrSub(tmpstr, "&lt;", "<", -1);

            // > --> &gt;
            StringUtil::StrSub(tmpstr, "&gt;", ">", -1);

            outFile << "            <line Text=\"" << tmpstr << "\"/>" << std::endl;
            m_DuplicateLines++;
        }
        outFile << "        </lines>" << std::endl;
        outFile << "    </set>" << std::endl;
    }
    else
    {
        outFile << pSource1->getFilename() << "(" << pSource1->getLine(line1)->getLineNumber() << ")" << std::endl;
        outFile << pSource2->getFilename() << "(" << pSource2->getLine(line2)->getLineNumber() << ")" << std::endl;
        for(int j=0;j<count;j++){
            outFile << pSource1->getLine(j+line1)->getLine() << std::endl;
            m_DuplicateLines++;
        }
        outFile << std::endl;
    }
}

int Duplo::process(SourceFile* pSource1, SourceFile* pSource2, std::ostream& outFile){
    const unsigned int m = pSource1->getNumOfLines();
    const unsigned int n = pSource2->getNumOfLines();

    const unsigned char NONE = 0;
    const unsigned char MATCH = 1;

    // Reset matrix data
    memset(m_pMatrix, NONE, m*n);

    // Compute matrix
    for(unsigned int y=0; y<m; y++){
        SourceLine* pSLine = pSource1->getLine(y);
        for(unsigned int x=0; x<n; x++){
            if(pSLine->equals(pSource2->getLine(x))){
                m_pMatrix[x+n*y] = MATCH;
            }
        }
    }

    // support reporting filtering by both:
    // - "lines of code duplicated", &
    // - "percentage of file duplicated"
    const unsigned int lMinBlockSize = std::max(
        1u, std::min(
            m_minBlockSize, 
            (std::max(n,m)*100)/m_blockPercentThreshold
        )
    );
    
    int blocks=0;

    // Scan vertical part
    for(unsigned int y=0; y<m; y++){
        unsigned int seqLen=0;
        int maxX = MIN(n, m-y);
        for(int x=0; x<maxX; x++){
            if(m_pMatrix[x+n*(y+x)] == MATCH){
                seqLen++;
            } else {
                if(seqLen >= lMinBlockSize){
                    int line1 = y+x-seqLen;
                    int line2 = x-seqLen;
                    if (!((line1 == line2) && (pSource1 == pSource2))) {
                        reportSeq(line1, line2, seqLen, pSource1, pSource2, outFile);
                        blocks++;
                    }
                }
                seqLen=0;
            }
        }

        if(seqLen >= lMinBlockSize){
            int line1 = m-seqLen;
            int line2 = n-seqLen;
            if (!((line1 == line2) && (pSource1 == pSource2))) {
                reportSeq(line1, line2, seqLen, pSource1, pSource2, outFile);
                blocks++;
            }
        }
    }

    if (pSource1 != pSource2)
    {
        // Scan horizontal part
        for(unsigned int x=1; x<n; x++){
            unsigned int seqLen=0;
            int maxY = MIN(m, n-x);
            for(int y=0; y<maxY; y++){
                if(m_pMatrix[x+y+n*y] == MATCH){
                    seqLen++;
                } else {
                    if(seqLen >= lMinBlockSize){
                        reportSeq(y-seqLen, x+y-seqLen, seqLen, pSource1, pSource2, outFile);
                        blocks++;
                    }
                    seqLen=0;
                }
            }

            if(seqLen >= lMinBlockSize){
                reportSeq(m-seqLen, n-seqLen, seqLen, pSource1, pSource2, outFile);
                blocks++;
            }
        }
    }

    return blocks;
}

const std::string Duplo::getFilenamePart(const std::string& fullpath) const {
    std::string path = StringUtil::substitute('\\', '/', fullpath);

    std::string filename = path;

    std::string::size_type idx = path.rfind('/');
    if(idx != std::string::npos){
        filename = path.substr(idx+1, path.size()-idx-1);
    }

    return filename;
}

bool Duplo::isSameFilename(const std::string& filename1, const std::string& filename2) const {
    return (getFilenamePart(filename1) == getFilenamePart(filename2));
}

void Duplo::run(std::string outputFileName){
    std::ofstream outfile(outputFileName.c_str(), std::ios::out|std::ios::binary);

    if (m_Xml)
    {
        outfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        outfile << "<?xml-stylesheet href=\"duplo.xsl\" type=\"text/xsl\"?>" << std::endl;
        outfile << "<duplo version=\"" << VERSION << "\">" << std::endl;
        outfile << "    <check Min_block_size=\"" << m_minBlockSize << 
            "\" Min_char_line=\"" << m_minChars << 
            "\" Ignore_prepro=\"" << (m_ignorePrepStuff ? "true" : "false") << 
            "\" Ignore_same_filename=\"" << (m_ignoreSameFilename ? "true" : "false") << "\">" << std::endl;
    }

    if(!outfile.is_open()){
        std::cout << "Error: Can't open file: " << outputFileName << std::endl;
        return;
    }

    clock_t start, finish;
    double  duration;

    start = clock();

    std::cout << "Loading and hashing files ... ";
    std::cout.flush();

    std::vector<SourceFile*> sourceFiles;
    
    TextFile listOfFiles(m_listFileName.c_str());
    std::vector<std::string> lines;
    listOfFiles.readLines(lines, true);
    
    int files = 0;
    int locsTotal = 0;

    // Create vector with all source files
    for(int i=0;i<(int)lines.size();i++){
        if(lines[i].size() > 5){
            SourceFile* pSourceFile = new SourceFile(lines[i], m_minChars, m_ignorePrepStuff);
            int numLines = pSourceFile->getNumOfLines();
            if(numLines > 0){
                files++;
                sourceFiles.push_back(pSourceFile);
                locsTotal+=numLines;
                if(m_maxLinesPerFile < numLines){
                    m_maxLinesPerFile = numLines;
                }
            }
        }
    }

    std::cout << "done.\n\n";

    // Generate matrix large enough for all files
    m_pMatrix = new unsigned char[m_maxLinesPerFile*m_maxLinesPerFile];


    int blocksTotal = 0;

    // Compare each file with each other
    for(int i=0;i<(int)sourceFiles.size();i++){
        std::cout << sourceFiles[i]->getFilename();
        int blocks = 0;
        
        blocks+=process(sourceFiles[i], sourceFiles[i], outfile);
        for(int j=i+1;j<(int)sourceFiles.size();j++){
            if ((m_ignoreSameFilename && isSameFilename(sourceFiles[i]->getFilename(), sourceFiles[j]->getFilename()))==false){
                blocks+=process(sourceFiles[i], sourceFiles[j], outfile);
            }
        }

        if(blocks > 0){
            std::cout << " found: " << blocks << " block(s)" << std::endl;
        } else {
            std::cout << " nothing found." << std::endl;
        }

        blocksTotal+=blocks;
    }


    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    std::cout << "Time: "<< duration << " seconds" << std::endl;

    if (m_Xml)
    {
        outfile << "        <summary Num_files=\"" << files <<
            "\" Duplicate_blocks=\"" << blocksTotal <<
            "\" Total_lines_of_code=\"" << locsTotal <<
            "\" Duplicate_lines_of_code=\"" << m_DuplicateLines <<
            "\" Time=\"" << duration <<
            "\"/>" << std::endl;
        outfile << "    </check>" << std::endl;
        outfile << "</duplo>" << std::endl;
    }
    else
    {
        outfile << "Configuration: " << std::endl;
        outfile << "  Number of files: " << files << std::endl;
        outfile << "  Minimal block size: " << m_minBlockSize << std::endl;
        outfile << "  Minimal characters in line: " << m_minChars << std::endl;
        outfile << "  Ignore preprocessor directives: " << m_ignorePrepStuff << std::endl;
        outfile << "  Ignore same filenames: " << m_ignoreSameFilename << std::endl;
        outfile << std::endl;
        outfile << "Results: " << std::endl;
        outfile << "  Lines of code: " << locsTotal << std::endl;
        outfile << "  Duplicate lines of code: " << m_DuplicateLines << std::endl;
        outfile << "  Total " << blocksTotal << " duplicate block(s) found." << std::endl << std::endl;
        outfile << "  Time: " << duration << " seconds" << std::endl;
    }
}

int Clamp (int upper, int lower, int value)
{
    return std::max( lower, std::min( upper, value ) );
}

/**
 * Main routine
 *
 * @param argc  number of arguments
 * @param argv  arguments
 */
int main(int argc, const char* argv[]){
    ArgumentParser ap(argc, argv);

    const int MIN_BLOCK_SIZE = 4;
    const int MIN_CHARS = 3;

    if(!ap.is("--help") && argc > 2){
        Duplo duplo(
            argv[argc-2], 
            ap.getInt("-ml", MIN_BLOCK_SIZE), 
            Clamp( 100, 0, ap.getInt("-pt", 100) ),
            ap.getInt("-mc", MIN_CHARS), 
            ap.is("-ip"), ap.is("-d"), ap.is("-xml")
        );
        duplo.run(argv[argc-1]);
    } else {
        std::cout << "\nNAME\n";
        std::cout << "       Duplo " << VERSION << " - duplicate source code block finder\n\n";

        std::cout << "\nSYNOPSIS\n";
        std::cout << "       duplo [OPTIONS] [INTPUT_FILELIST] [OUTPUT_FILE]\n";

        std::cout << "\nDESCRIPTION\n";
        std::cout << "       Duplo is a tool to find duplicated code blocks in large\n";
        std::cout << "       C/C++/Java/C#/VB.Net software systems.\n\n";

        std::cout << "       -ml              minimal block size in lines (default is " << MIN_BLOCK_SIZE << ")\n";
        std::cout << "       -pt              percentage of lines of duplication threshold to override -ml\n";
        std::cout << "                        (default is 100%)\n";
        std::cout << "                        useful for identifying whole file class duplication\n";
        std::cout << "       -mc              minimal characters in line (default is " << MIN_CHARS << ")\n";
        std::cout << "                        lines with less characters are ignored\n";
        std::cout << "       -ip              ignore preprocessor directives\n";
        std::cout << "       -d               ignore file pairs with same name\n";
        std::cout << "       -xml             output file in XML\n";
        std::cout << "       INTPUT_FILELIST  input filelist\n";
        std::cout << "       OUTPUT_FILE      output file\n";

        std::cout << "\nVERSION\n";
        std::cout << "       " << VERSION << "\n";

        std::cout << "\nAUTHORS\n";
        std::cout << "       Christian M. Ammann (cammann@giants.ch)\n";    
        std::cout << "       Trevor D'Arcy-Evans (tdarcyevans@hotmail.com)\n\n";    
    }

    return 0;
}

