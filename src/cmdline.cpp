#include "cmdline.h"

#include <tclap/CmdLine.h>

#include <iostream>
#include <string>

int parseThem(int argc, const char *argv[], CommandLineOptions &opts)
try
{
    if (argc == 1) {
        return 0;
    }

    TCLAP::CmdLine cmd("Simple Blocks blocking volume render experiment.", ' ');

    // volume data file
    TCLAP::ValueArg<std::string> fileArg("f", "file", "Path to data file.",
            false, "", "string");
    cmd.add(fileArg);

    // volume data type
    std::vector<std::string> dataTypes{ "float", "ushort", "uchar" };
    TCLAP::ValuesConstraint<std::string> dataTypeAllowValues(dataTypes);
    TCLAP::ValueArg<std::string> dataTypeArg("t", "type",
            "Data type (float, ushort, uchar).", false, "", &dataTypeAllowValues);
    cmd.add(dataTypeArg);

    
    // volume dims
    TCLAP::ValueArg<size_t> xdimArg("", "volx", "Volume x dim.", false, 1, "uint");
    cmd.add(xdimArg); 

    TCLAP::ValueArg<size_t> ydimArg("", "voly", "Volume y dim.", false, 1, "uint");
    cmd.add(ydimArg);
    
    TCLAP::ValueArg<size_t> zdimArg("", "volz", "Volume z dim.", false, 1, "uint");
    cmd.add(zdimArg);


    // threshold min/max
    TCLAP::ValueArg<float> tmin("", "tmin", "Thresh min", false, 0.0, "float");
    cmd.add(tmin);

    TCLAP::ValueArg<float> tmax("", "tmax", "Thresh max", false, 1.0, "float");
    cmd.add(tmax);


    // halting criteria for recursive division of volume.
    TCLAP::ValueArg<float> emptyPercentArg("", "eperc", "Min. empty percent",
            false, 1.0, "float");
    cmd.add(emptyPercentArg);

    TCLAP::ValueArg<int> minVoxelsArg("", "minvox", "Minimum voxels per region",
            false, 1, "float");
    cmd.add(minVoxelsArg);


    // print blocks
    TCLAP::SwitchArg printBlocksArg("", "print-blocks", "Print blocks into to stdout.", cmd, false);


    cmd.parse(argc, argv);

    opts.filePath = fileArg.getValue();
    opts.type = dataTypeArg.getValue(); 
    opts.printBlocks = printBlocksArg.getValue();
    opts.w = xdimArg.getValue();
    opts.h = ydimArg.getValue();
    opts.d = zdimArg.getValue();
    opts.tmin = tmin.getValue();
    opts.tmax = tmax.getValue();
    opts.emptyPercent = emptyPercentArg.getValue();
    opts.minVoxels = minVoxelsArg.getValue();

    return static_cast<int>(cmd.getArgList().size());

} catch (TCLAP::ArgException &e) {
    std::cout << "Error parsing command line args: " << e.error() << " for argument " 
        << e.argId() << std::endl;
    return 0;
}


void printThem(CommandLineOptions &opts)
{
    std::cout <<
        "File path: " << opts.filePath << "\n"
        "Dat file: " << opts.datFilePath << "\n"
        "Data Type: " << opts.type     << "\n"
        "Vol dims (w X h X d): "   << opts.w        << " X " << opts.h        << " X " << opts.d        << "\n"
        "Range(min-max): "    << opts.tmin     << " - " << opts.tmax     << "\n"
        "Print blocks: "           << (opts.printBlocks ? "True" : "False")   << 
    std::endl;
}

