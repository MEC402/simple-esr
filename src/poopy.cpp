//
// Created by jim on 8/16/15.
//
#include "poopy.h"

#include <vector>
#include <iostream>
#include <algorithm>

namespace svt{

    std::vector<int> sumTable;
    std::vector<int> offsetsX;
    std::vector<int> offsetsY;
    std::vector<int> offsetsZ;
    std::vector<Node> tree;

    Vec3 extents{ 0,0,0 };
    int minVoxels{ 0 };
    float minEmptyPercent{ 0 };

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
size_t
to1d(size_t x, size_t y, size_t z, size_t maxX, size_t maxY)
{
    return x + maxX * (y + maxY * z);
}


///////////////////////////////////////////////////////////////////////////////
int
get(int x, int y, int z)
{
    return svt::sumTable[x + svt::extents.x * (y + svt::extents.y * z)];
}


///////////////////////////////////////////////////////////////////////////////
int
get_check(int x, int y, int z)
{
    if (x < 0 || y < 0 || z < 0) {
        return 0;
    }

    return get(x, y, z);
}


///////////////////////////////////////////////////////////////////////////////
int
num(Vec3 const &min, Vec3 const &max)
{
    return (get(max.x, max.y, max.z) - get(max.x, max.y, min.z))
           - (get(min.x, max.y, max.z) - get(min.x, max.y, min.z))
           - (get(max.x, min.y, max.z) - get(max.x, min.y, min.z))
           + (get(min.x, min.y, max.z) - get(min.x, min.y, min.z));
}


///////////////////////////////////////////////////////////////////////////////
std::vector<int> const & 
createSumTable(float const *volume, Vec3 extents, std::function<int(float)> empty)
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
void
genPlanes(int numPlanes, int delta, int start, std::vector<int> &candidates)
{
    candidates.resize(static_cast<size_t>(numPlanes));
    std::generate(candidates.begin(), candidates.end(),
        svt::accum_delta(start, delta));
}


///////////////////////////////////////////////////////////////////////////////
void
split(float minEmptyPercent, int minVoxels, int delta)
{
    svt::minEmptyPercent = minEmptyPercent;
    svt::minVoxels = minVoxels;

    genPlanes(svt::extents.x / delta, delta, delta, svt::offsetsX);
    genPlanes(svt::extents.y / delta, delta, delta, svt::offsetsY);
    genPlanes(svt::extents.z / delta, delta, delta, svt::offsetsZ);
    
    Node root{ 0, false, 0, {0,0,0}, svt::extents };
    
    svt::tree[0] = root;
    recursiveSplitHelper(root);
}


///////////////////////////////////////////////////////////////////////////////
void
recursiveSplitHelper(Node &n)
{
    const bool not_a_leaf{ false };
    const bool is_a_leaf{ !not_a_leaf };

    // if n.numVox too small: n.isLeafe(true); return;
    // if n.size too small: n.isLeaf(true); return;
    if (n.numVoxels() <= svt::minVoxels) {
        n.isLeaf(is_a_leaf);
        return;
    }

    if (n.percentEmpty() <= svt::minEmptyPercent){
        n.isLeaf(is_a_leaf);
        return;
    }

    Plane p{ findPlane(n.shortestAxis(), n.bv()) };

    Node left{ n.leftChild(),
               not_a_leaf,
               num(n.min(), p.max()),
               n.min(),
               p.max() - n.min() };
    recursiveSplitHelper(left);

    Node right{ n.rightChild(),
                not_a_leaf,
                num(p.min(), n.max()),
                p.min(),
                n.max() - p.min() };
    recursiveSplitHelper(right);

    svt::tree[left.index()] = left;
    svt::tree[right.index()] = right;
}


///////////////////////////////////////////////////////////////////////////////
int 
diffSides(Vec3 const &leftMin, Vec3 const &leftMax,
        Vec3 const &rightMin, Vec3 const &rightMax)
{
    int left{ bv(leftMin, leftMax) };
    int right{ bv(rightMin, rightMax) };
    return std::abs(left - right);
}

///////////////////////////////////////////////////////////////////////////////
Plane
findPlane(Axis a, BoundingVolume const &bv)
{
    int off{ 0 };
    int smallest{ std::numeric_limits<int>::max() };
    Vec3 min{ bv.min() };
    Vec3 max{ bv.min() + bv.extent() };

    // move candidate plane along axis
    switch (a)
    {
    case Axis::X:   
    {
        std::vector<int> &p = svt::offsetsX;
//        for (size_t i = 0; i < p.size(); ++i) {
        for(auto pp : p) {
            int diff{ 
                diffSides(
                    min, 
                    { min.x+pp, max.y, max.z },
                    { min.x+pp, min.y, min.z },
                    max
                ) };

            std::cout << "X pp: " << pp << " diff: " << diff << "\n-----\n" ;
            //TODO: handle diff==smallest separately
            if (diff <= smallest) {
                smallest = diff;
                off = pp;
            }
        } //for

        return Plane{ { min.x + off, min.y, min.z },
                      { min.x + off, max.y, max.z } };
    }

    case Axis::Y:   
    {
        std::vector<int> &p = svt::offsetsY;
//        for (size_t i = 0; i < p.size(); ++i) {
        for(auto pp : p) {
            int diff{ 
                diffSides( 
                    min, 
                    { max.x, min.y+pp, max.z },
                    { min.x, min.y+pp, min.z },
                    max 
                ) };

            std::cout << "Y pp: " << pp << " diff: " << diff << "\n-----\n" ;
            //TODO: handle diff==smallest separately
            if (diff <= smallest) {
                smallest = diff;
                off = pp;
            }
        } //for

        return Plane{ { min.x, min.y + off, min.z },
                      { max.x, min.y + off, max.z } };
    }

    case Axis::Z:   
    {
        std::vector<int> &p = svt::offsetsZ;
//        for (size_t i = 0; i < p.size(); ++i) {
        for(auto pp : p) {
            int diff{
                diffSides(
                    min, 
                    { max.x, max.y, min.z+pp },
                    { min.x, min.y, min.z+pp },
                    max
                ) };

            std::cout << "Z pp: " << pp << " diff: " << diff << "\n-----\n" ;
            //TODO: handle diff==smallest separatly
            if (diff <= smallest) {
                smallest = diff;
                off = pp;
            }
        } //for

        return Plane{ { min.x, min.y, min.z + off },
                      { max.x, max.y, min.z + off } };
    }

    default: break;
    }

    return Plane{ { -1,-1,-1 }, { -1,-1,-1 } };  // The most interesting case.
}

///////////////////////////////////////////////////////////////////////////////
int 
bv(Vec3 const &rmin, Vec3 const &rmax)
{
    Vec3 bvmin{ 0, 0, 0 };
    Vec3 bvmax{ 0, 0, 0 };

    uint32_t found{ 0x0 };
    // xmin --> xmax
    for (int x{ rmin.x }; x < rmax.x; ++x) {
        if (num(rmin, { x, rmax.y, rmax.z }) != 0) {
            bvmin.x = x;
            found |= 0x1;
            break;
        }
    }

    // ymin --> ymax
    for (int y{ rmin.y }; y < rmax.y; ++y) {
        if (num(rmin, { rmax.x, y, rmax.z }) != 0) {
            bvmin.y = y;
            found |= 0x2;
            break;
        }
    }

    // zmin --> zmax
    for (int z{ rmin.z }; z < rmax.z; ++z) {
        if (num(rmin, { rmax.x, rmax.y, z }) != 0) {
            bvmin.z = z;
            found |= 0x4;
            break;
        }
    }

    // Q: You ask: Why is 1 added to final x, y, and z values for max-->min searches?        //
    // A: num() actually returns the value for the region starting one voxel in front of the //
    // minimum indexes provided for its rmin. To compensate we add 1 to the final index.     //
    // In other words, for a region R=[min, max], num(R) returns non-empty voxels for the    //
    // region R=[min+1, max].

    // xmax --> xmin
    for (int x{ rmax.x }; x > rmin.x; --x) {
        if (num({ x, rmin.y, rmin.z }, rmax) != 0) {
            bvmax.x = x+1;
            found |= 0x8;
            break;
        }
    }


    // ymax --> ymin
    for (int y{ rmax.y }; y > rmin.y; --y) {
       if (num({ rmin.x, y, rmin.z }, rmax) != 0) {
           bvmax.y = y+1;
           found |= 0x10;
           break;
       }
    }


    // zmax --> zmin
    for (int z{ rmax.z }; z > rmin.z; --z) {
        if (num({ rmin.x, rmin.y, z }, rmax) != 0) {
            bvmax.z = z+1;
            found |= 0x20;
            break;
        }
    }

    
    if (found == 0x20+0x10+0x8+0x4+0x2+0x1) {
        std::cerr << "All planes found: " << found << std::endl;
    } else {
        std::cerr << "Not all planes found: " << found << std::endl;
//      return 0;
    }

    //TODO: think about which one: idx-1 or idx-none?
//  int n = num({ bvmin.x-1, bvmin.y-1, bvmin.z-1 }, bvmax);
    int n = num({ bvmin.x, bvmin.y, bvmin.z }, bvmax);

    std::cout << "Num(" << bvmin << ", " << bvmax << "): " << n << std::endl;

    return n;
}


///////////////////////////////////////////////////////////////////////////////
void
printSumTable()
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
void 
printNumCoords(Vec3 const &min, Vec3 const &max)
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


///////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream &os, Plane const &v)
{
    return os << "{" << v.min() << ", " << v.max() << "}";
}


///////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream &os, BoundingVolume const &bv)
{
    return os << "{" << bv.min() << ", " << bv.extent() << ", " << bv.nonEmptyVoxels() << "}";
}
