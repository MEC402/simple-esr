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

    /////////////////////////////////////////////////////////////////////////// 
    /// \brief Generator that produces ints spaced by \c delta amount and 
    /// starting at \c start.
    /////////////////////////////////////////////////////////////////////////// 
    struct accum_delta {
        int d;  // delta
        int v;  // value

        accum_delta(int start, int delta) : d{ delta }, v{ start } { }

        int operator()() { 
            int r = v;
            v += d;

            return r; 
        }
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
std::vector<int> const & 
createSumTable(float *volume, Vec3 extents, std::function<int(float)> empty)
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
    
    return svt::sumTable;
}


///////////////////////////////////////////////////////////////////////////////
Vec3 findPlane(float perc, int axis, Vec3 const &min, Vec3 const &max)
{
    size_t numPlanes{ 0 };
    size_t delta{ 0 };
    size_t smallestIdx{ 0 };
    std::vector<int> candidates;
     
    int smallest{ std::numeric_limits<int>::max() };
    // move candidate plane along axis
    switch (axis)
    {
        case 0:   // X
        {
            numPlanes = static_cast<size_t>(perc * svt::extents.x);
            delta = svt::extents.x / numPlanes;
            candidates.resize(numPlanes);
            std::generate(candidates.begin(), candidates.end(), 
                svt::accum_delta(min.x, delta));

            for (size_t i = 0; i < candidates.size(); ++i) {
                Vec3 candidate{ min.x + candidates[i], min.y, min.z };

                int diff = std::abs( bv(min, candidate) - bv(candidate, max) );

                if (diff < smallest) {
                    smallest = diff;
                    smallestIdx = i;
                }
            }

            return{ min.x + candidates[smallestIdx], min.y, min.z };
        }

        case 1:   // Y
        {
            numPlanes = static_cast<size_t>(perc * svt::extents.y);
            delta = svt::extents.y / numPlanes;
            candidates.resize(numPlanes);
            std::generate(candidates.begin(), candidates.end(), 
                svt::accum_delta(min.y, delta));

            for (size_t i = 0; i < candidates.size(); ++i) {
                Vec3 candidate{ min.x, min.y + candidates[i], min.z };

                int diff = std::abs( bv(min, candidate) - bv(candidate, max) );

                if (diff < smallest) {
                    smallest = diff;
                    smallestIdx = i;
                }
            }

            return{ min.x, min.y + candidates[smallestIdx], min.z };
        }

        case 2:   // Z
        {
            numPlanes = static_cast<size_t>(perc * svt::extents.z);
            delta = svt::extents.z / numPlanes;
            candidates.resize(numPlanes);
            std::generate(candidates.begin(), candidates.end(), 
                svt::accum_delta(min.z, delta));

            for (size_t i = 0; i < candidates.size(); ++i) {
                Vec3 candidate{ min.x, min.y, min.z + candidates[i] };

                int diff = std::abs(bv(min, candidate) - bv(candidate, max));

                if (diff < smallest) {
                    smallest = diff;
                    smallestIdx = i;
                }
            }

            return{ min.x, min.y, min.z + candidates[smallestIdx] };
        }

        default: break;
    }
    return{ -1, -1, -1 };
}


int bv(Vec3 const& rmin, Vec3 const& rmax)
{
    Vec3 bvmin{0,0,0};
    Vec3 bvmax{0,0,0};
    
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

    // Q: Why is 1 added to final x, y, and z values?
    // A: num() actually returns the value for the region starting one voxel in front of the 
    // minimum indexes provided for its rmin. To compensate we add 1 to the final index.
    // In other words, for a region R=[min, max], num(R) returns non-empty voxels for the
    // region R=[min+1, max].

    // xmax --> xmin
    for (int x{ rmax.x }; x > rmin.x; --x) {
        std::cout << "xmax=" << x << ", ";
        if (num({ x, rmin.y, rmin.z }, rmax) != 0) {
            bvmax.x = x+1;
            std::cout << " Found xmax: " << bvmax;
            break;
        }
    }
    std::cout << '\n';


    // ymax --> ymin
    for (int y{ rmax.y }; y > rmin.y; --y) {
        std::cout << "ymax=" << y << ", ";
       if (num({ rmin.x, y, rmin.z }, rmax) != 0) {
           bvmax.y = y+1;
           std::cout << " Found ymax: " << bvmax;
           break;
       }
    }
    std::cout << '\n';


    // zmax --> zmin
    for (int z{ rmax.z }; z > rmin.z; --z) {
        std::cout << "zmax=" << z << ", ";
        if (num({ rmin.x, rmin.y, z }, rmax) != 0) {
            bvmax.z = z+1;
            std::cout << " Found zmax: " << bvmax;
            break;
        }
    }
    std::cout << std::endl;

    //int n = num({ bvmin.x-1, bvmin.y-1, bvmin.z-1 }, bvmax);
    int n = num({ bvmin.x, bvmin.y, bvmin.z }, bvmax);

    std::cout << "BV Min: " << bvmin << '\n';
    std::cout << "BV Max: " << bvmax << '\n';
    std::cout << "Num(BV_min, BV_max): " << n << '\n';

    return n;
}


///////////////////////////////////////////////////////////////////////////////
void printSumTable()
{
    int i{ 0 };
    for (auto v : svt::sumTable) {
        if (i % svt::extents.x == 0) {
            std::cout << '\n';
            if (i % (svt::extents.x * svt::extents.y) == 0) {
                std::cout << '\n';
            }
        }
        std::cout << v << ", ";
        ++i;
    }
    std::cout << '\n';
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

