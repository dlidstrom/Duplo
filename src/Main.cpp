#include "ArgumentParser.h"
#include "Duplo.h"
#include "Options.h"

#include <algorithm>
#include <iostream>
#include <iterator>

namespace {
    int run(long argc, const char* argv[]) {
        ArgumentParser ap(argc, argv);

        const int MIN_BLOCK_SIZE = 4;
        const int MIN_CHARS = 3;

        if (!ap.is("--help") && argc > 2) {
            auto minBlockSize = ap.getInt("-ml", MIN_BLOCK_SIZE);
            auto blockPercentThresholdValue = std::clamp(ap.getInt("-pt", 100), 0, 100);
            if (blockPercentThresholdValue < 0 || 100 < blockPercentThresholdValue) {
                throw std::runtime_error("-pt out of range");
            }

            unsigned char blockPercentThreshold = static_cast<unsigned char>(blockPercentThresholdValue);
            auto numberOfFiles = ap.getInt("-n", 0);
            auto minChars = ap.getInt("-mc", MIN_CHARS);
            bool ignorePrepStuff = ap.is("-ip");
            bool outputXml = ap.is("-xml");
            bool ignoreSameFilename = ap.is("-d");
            std::string listFilename(argv[argc - 2]);
            std::string outputFilename(argv[argc - 1]);
            Options options(
                minChars,
                ignorePrepStuff,
                minBlockSize,
                blockPercentThreshold,
                numberOfFiles,
                outputXml,
                ignoreSameFilename,
                listFilename,
                outputFilename);
            return Duplo::Run(options);
        } else {
            std::cout << "\nNAME\n";
            std::cout << "       Duplo " << VERSION << " - duplicate source code block finder\n\n";

            std::cout << "\nSYNOPSIS\n";
            std::cout << "       duplo [OPTIONS] [INTPUT_FILELIST] [OUTPUT_FILE]\n";

            std::cout << "\nDESCRIPTION\n";
            std::cout << "       Duplo is a tool to find duplicated code blocks in large\n";
            std::cout << "       C/C++/Java/C#/VB.Net/Ada software systems.\n\n";

            std::cout << "       -ml              minimal block size in lines (default is " << MIN_BLOCK_SIZE << ")\n";
            std::cout << "       -pt              percentage of lines of duplication threshold to override -ml\n";
            std::cout << "                        (default is 100%)\n";
            std::cout << "                        useful for identifying whole file duplication\n";
            std::cout << "       -mc              minimal characters in line (default is " << MIN_CHARS << ")\n";
            std::cout << "                        lines with less characters are ignored\n";
            std::cout << "       -n               only report for first N files\n";
            std::cout << "       -ip              ignore preprocessor directives\n";
            std::cout << "       -d               ignore file pairs with same name\n";
            std::cout << "       -xml             output file in XML\n";
            std::cout << "       INPUT_FILELIST   input filelist (specify '-' to read from stdin)\n";
            std::cout << "       OUTPUT_FILE      output file (specify '-' to output to stdout)\n";

            std::cout << "\nVERSION\n";
            std::cout << "       " << VERSION << "\n";

            std::cout << "\nAUTHORS\n";
            auto authors = {
                "       Daniel Lidstrom (dlidstrom@gmail.com)",
                "       Christian M. Ammann (cammann@giants.ch)",
                "       Trevor D'Arcy-Evans (tdarcyevans@hotmail.com)",
            };
            std::copy(std::begin(authors), std::end(authors), std::ostream_iterator<const char*>(std::cout, "\n"));
            std::cout << "\n";
            return EXIT_FAILURE;
        }
    }
}

int main(int argc, const char* argv[]) {
    try {
        return run(argc, argv);
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}
