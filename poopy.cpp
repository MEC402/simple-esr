//
// Created by jim on 8/16/15.
//
#include "poopy.h"

#include <vector>
#include <iostream>
#include <algorithm>

namespace svt{
    std::vector<int> sumTable;

    Vec3 extents;

    struct accum_delta {
        int d;  // delta
        int v;  // value
        accum_delta(int delta) : d{delta} { }
        int operator()() { return v += d; }
    };

} // namespace svt


///////////////////////////////////////////////////////////////////////////////
size_t to1d(size_t x, size_t y, size_t z, size_t maxX, size_t maxY)
{
    return x + maxX * (y + maxY * z);
}


///////////////////////////////////////////////////////////////////////////////
int get(int x, int y, int z)
{
    return svt::sumTable[x + svt::extents.x * (y + svt::extents.y * z)];
}


///////////////////////////////////////////////////////////////////////////////
int get_check(int x, int y, int z)
{
    if (x < 0 || y < 0 || z < 0) {
        return 0;
    }

    return get(x, y, z);
}


///////////////////////////////////////////////////////////////////////////////
int num(Vec3 const &min, Vec3 const &max)
{
    return (get(max.x, max.y, max.z) - get(max.x, max.y, min.z))
           - (get(min.x, max.y, max.z) - get(min.x, max.y, min.z))
           - (get(max.x, min.y, max.z) - get(max.x, min.y, min.z))
           + (get(min.x, min.y, max.z) - get(min.x, min.y, min.z));
}


///////////////////////////////////////////////////////////////////////////////
void createSumTable(float *volume, Vec3 extents, std::function<int(int)> empty)
{
    svt::extents = extents;
    svt::sumTable.resize(size_t(extents.x) * size_t(extents.y) * size_t(extents.z));
    for(int z = 1; z < extents.z; ++z) {
    for(int y = 1; y < extents.y; ++y) {
    for(int x = 1; x < extents.x; ++x) {
        size_t idx{ to1d(x, y, z, extents.x, extents.y) };

        int v1{ empty(volume[idx]) };
        int v2{ v1 + get_check(x, y, z - 1)
                + (get_check(x - 1, y,     z) - get_check(x - 1, y, z - 1))
                + (get_check(x,     y - 1, z) - get_check(x, y - 1, z - 1))
                - (get_check(x - 1, y - 1, z) - get_check(x - 1, y - 1, z - 1))
            };

        svt::sumTable[idx] = v2;
    }}}
}


///////////////////////////////////////////////////////////////////////////////
void findPlane(int perc, int axis, Vec3 const &min, Vec3 const &max)
{
    size_t numPlanes{ 0 };
    size_t delta{ 0 };
    std::vector<int> candidates;

    // move candidate plane along axis
    switch (axis)
    {
        case 0:   // X
            numPlanes = static_cast<size_t>(perc * svt::extents.x);
            delta = svt::extents.x / numPlanes;
            candidates.resize(numPlanes);
            std::generate(candidates.begin(), candidates.end(), svt::accum_delta(delta));
            for (size_t i = 0; i < candidates.size(); ++i) {
                  //TODO: BV
            }
            break;
        case 1:   // Y
            numPlanes = static_cast<size_t>(perc * svt::extents.y);
            delta = svt::extents.y / numPlanes;
            candidates.resize(numPlanes);
            std::generate(candidates.begin(), candidates.end(), svt::accum_delta(delta));
            for (size_t i = 0; i < candidates.size(); ++i) {
                //TODO: BV
            }
            break;
        case 2:   // Z
            numPlanes = static_cast<size_t>(perc * svt::extents.z);
            delta = svt::extents.z / numPlanes;
            candidates.resize(numPlanes);
            std::generate(candidates.begin(), candidates.end(), svt::accum_delta(delta));
            for (size_t i = 0; i < candidates.size(); ++i) {
                //TODO: BV
            }
            break;
        default: break;
    }
}


void bv(Vec3 const& rmin, Vec3 const& rmax)
{
    Vec3 bvmin; 
    Vec3 bvmax;
    
    // xmin --> xmax
    for (int x{ rmin.x }; x < rmax.x; ++x) {
        std::cout << "xmin=" << x << ", ";
        if (num(rmin, { x, rmax.y, rmax.z }) != 0) {
            bvmin.x = x;
            std::cout << " Found xmin: " << bvmin;
            break;
        }
    }
    std::cout << '\n';

    // ymin --> ymax
    for (int y{ rmin.y }; y < rmax.y; ++y) {
        std::cout << "ymin=" << y << ", ";
        if (num(rmin, { rmax.x, y, rmax.z }) != 0) {
            bvmin.y = y;
            std::cout << " Found ymin: " << bvmin;
            break;
        }
    }
    std::cout << '\n';

    // zmin --> zmax
    for (int z{ rmin.z }; z < rmax.z; ++z) {
        std::cout << "zmin=" << z << ", ";
        if (num(rmin, { rmax.x, rmax.y, z }) != 0) {
            bvmin.z = z;
            std::cout << " Found zmin: " << bvmin;
            break;
        }
    }
    std::cout << '\n';


    // xmax --> xmin
    for (int x{ rmax.x }; x-- > bvmin.x;) {
        std::cout << "xmax=" << x << ", ";
        if (num({ x, bvmin.y, bvmin.z }, rmax) != 0) {
            bvmax.x = x;
            std::cout << " Found xmax: " << bvmax;
            break;
        }
    }
    std::cout << '\n';


    // ymax --> ymin
    for (int y{ rmax.y }; y-- > bvmin.y;) {
        std::cout << "ymax=" << y << ", ";
       if (num({ bvmin.x, y, bvmin.z }, rmax) != 0) {
           bvmax.y = y;
           std::cout << " Found ymax: " << bvmax;
           break;
       }
    }
    std::cout << '\n';


    // zmax --> zmin
    for (int z{ rmax.z }; z-- > bvmin.z;) {
        std::cout << "zmax=" << z << ", ";
        if (num({ bvmin.x, bvmin.y, z }, rmax) != 0) {
            bvmax.z = z;
            std::cout << " Found zmax: " << bvmax;
            break;
        }
    }
    std::cout << '\n';

    std::cout << "BV Min: " << bvmin << std::endl;
    std::cout << "BV Max: " << bvmax << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
void printSumTable()
{
    int i{ 0 };
    for (auto v : svt::sumTable) {
        if (i % svt::extents.x == 0) {
            std::cout << "\n";
            if (i % (svt::extents.x * svt::extents.y) == 0) {
                std::cout << "\n";
            }
        }
        std::cout << v << ", ";
        ++i;
    }
}


///////////////////////////////////////////////////////////////////////////////
void printNumCoords(Vec3 const &min, Vec3 const &max)
{
    std::cout
           <<  "\nN(R) = (" << Vec3{max.x, max.y, max.z} << ":" << get(max.x,max.y,max.z) << " - " << Vec3{max.x, max.y, min.z} << ":" << get(max.x,max.y,min.z)
           << ")\n     - (" << Vec3{min.x, max.y, max.z} << ":" << get(min.x,max.y,max.z) << " - " << Vec3{min.x, max.y, min.z} << ":" << get(min.x,max.y,min.z)
           << ")\n     - (" << Vec3{max.x, min.y, max.z} << ":" << get(max.x,min.y,max.z) << " - " << Vec3{max.x, min.y, min.z} << ":" << get(max.x,min.y,min.z)
           << ")\n     + (" << Vec3{min.x, min.y, max.z} << ":" << get(min.x,min.y,max.z) << " - " << Vec3{min.x, min.y, min.z} << ":" << get(min.x,min.y,min.z)
           << ")\n     = " << num(min, max) << "\n";
}


///////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream &os, Vec3 const &v)
{
    return os << "{" << v.x << ", " << v.y << ", " << v.z << "}";
}

