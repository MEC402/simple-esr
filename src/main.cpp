
#include "svt.h"
#include "cmdline.h"


#include <iostream>
#include <memory>
//#include <vector>
//#include <array>

//const int X{ 0 };
//const int Y{ 1 };
//const int Z{ 2 };

struct is_empty {
    float tmin;
    float tmax;

    is_empty(float min, float max)
        : tmin{ min }
        , tmax{ max }
    { }

    int operator()(float x) {
        return x>tmin && x<tmax;
    }
};


int main(int argc, char const * argv[])
{
    CommandLineOptions opts;
    if (parseThem(argc, argv, opts) == 0) {
        std::cout << "Use -h for command line options help." << std::endl;
        exit(1);
    }

    std::string rawfile{ opts.filePath };
//    DataReader<float, float> reader;
//    reader.loadRaw3d(rawfile, opts.w, opts.h, opts.d, false);
//    std::unique_ptr<float[]> data{ reader.takeOwnership() };
//
//    is_empty peep(opts.tmin, opts.tmax);
//    int width  = static_cast<int>(opts.w);
//    int height = static_cast<int>(opts.h);
//    int depth  = static_cast<int>(opts.d);
//    createSumTable(data.get(), {width, height, depth}, peep);
//    split(opts.emptyPercent, opts.minVoxels, {5,5,5});
//    printTree();
    return 0;
}
