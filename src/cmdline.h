#ifndef cmdline_h__
#define cmdline_h__

#include <string>

struct CommandLineOptions
{
    // raw file path
    std::string filePath;
    // transfer function file path
    std::string tfuncPath;
    // for .dat descriptor file (currently unimplemented)
    std::string datFilePath;
    // volume data type
    std::string type;
    // true if block data should be dumped to file
    bool printBlocks;
    // vol width
    size_t w;
    // vol height
    size_t h;
    // vol depth
    size_t d;
    // threshold max
    float tmax;
    // threshold minimum
    float tmin;
};

///////////////////////////////////////////////////////////////////////////////
/// \brief Parses command line args and populates \c opts.
///
/// If non-zero arg was returned, then the parse was successful, but it does 
/// not mean that valid or all of the required args were provided on the 
/// command line.
///
/// \returns 0 on parse failure, non-zero if the parse was successful.
///////////////////////////////////////////////////////////////////////////////
int parseThem(int argc, const char *argv [], CommandLineOptions &opts);

void printThem(CommandLineOptions &);
    
#endif // cmdline_h__
