#include <iostream>
#include <vector>
#include <array>

#include "poopy.h"

const int X{ 0 };
const int Y{ 1 };
const int Z{ 2 };

// 12 x 8 x 4
std::array<float, 12*8*4> region{
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,

    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,1,1,1,0,0,0,0,0,0,0,
    0,0,1,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,1,1,0,0,
    0,0,0,0,0,0,0,0,1,1,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,

    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,1,1,1,0,0,0,0,0,0,0,
    0,0,1,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,1,1,0,0,
    0,0,0,0,0,0,0,0,1,1,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,

    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0
};

struct is_empty
{
    int operator()(float x) { return static_cast<int>(x); }
};

int main()
{
    createSumTable(region.data(), { 12, 8, 4 }, is_empty());
    printSumTable();
//    findPlane(0.3f, X, )

    Vec3 roi_min{ 0,0,0 };
    Vec3 roi_max{ 6,6,2 };

    std::cout << std::endl;

    Vec3MinMaxPair split = findPlane(0.3f, X, roi_min, roi_max);

    std::cout << split << std::endl;

    printNumCoords(roi_min, roi_max);

    return 0;
}