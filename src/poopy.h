//
// Created by jim on 8/16/15.
//

#ifndef POOPYPANTS_POOPY_H
#define POOPYPANTS_POOPY_H

#include <functional>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>

struct Vec3
{
    Vec3() : Vec3(0,0,0) { }
    Vec3(int x, int y, int z) : x{ x } , y{ y } , z{ z } { }
    Vec3(Vec3 const &o) : x{ o.x }, y{ o.y }, z{ o.z } { }

    bool operator==(Vec3 const &o)
    {
        return x == o.x && y == o.y && z == o.z;
    }

    int x;
    int y;
    int z;
};

struct Vec3MinMaxPair
{
    Vec3MinMaxPair() : Vec3MinMaxPair({ 0,0,0 }, { 0,0,0 }) { }

    Vec3MinMaxPair(Vec3 const &vmin, Vec3 const &vmax) : min{vmin}, max{vmax} { }

    Vec3MinMaxPair(Vec3MinMaxPair const &o) : min{ o.min }, max{ o.max } { }

    bool operator==(Vec3MinMaxPair const &o) { return min == o.min && max == o.max; }

    Vec3 min;
    Vec3 max;
};

/////////////////////////////////////////////////////////////////////////// 
/// \brief A node!!!! In a tree!!!
/////////////////////////////////////////////////////////////////////////// 
class Node
{
public:
    Node() : Node(0,0,0) { }

    Node(int a, int minP, int maxP) 
        : m_axis{ a }
        , m_minPlane{ minP }
        , m_maxPlane{ maxP }
        , m_leftChild{ 0 }
        , m_rightChild{ 0 }
    { }

    int axis() const { return m_axis; }
    int minP() const { return m_minPlane; }
    int maxP() const { return m_maxPlane; }
    int leftChild() const { return m_leftChild; }
    int rightChild() const { return m_rightChild; }
    
    void leftChild(int c) { m_leftChild = c; }
    void rightChild(int c) { m_rightChild = c; }

private:
    int m_axis;
    int m_minPlane;
    int m_maxPlane;
    int m_leftChild;
    int m_rightChild;
};

class BoundingVolume{
public:
    BoundingVolume(int n, Vec3 const &min, Vec3 const &extent)
        : BoundingVolume(n, {min, extent} )
    { }

    BoundingVolume(int n, Vec3MinMaxPair const &minmax)
        : m_nonEmptyVoxels{ n }
        , m_minmax{ minmax }
    { }

    BoundingVolume(BoundingVolume const & o)
        : m_nonEmptyVoxels{ o.m_nonEmptyVoxels }
        , m_minmax{ o.m_minmax }
    { }

    Vec3 const & min() const { return m_minmax.min; }

    Vec3 const & extent() const { return m_minmax.max; }

    int nonEmptyVoxels() const { return m_nonEmptyVoxels; }

private:
    int m_nonEmptyVoxels;
    Vec3MinMaxPair m_minmax;
};


size_t to1d(size_t x, size_t y, size_t z, size_t maxX, size_t maxY);


///////////////////////////////////////////////////////////////////////////////
/// \brief Get the value V(x,y,z) from the sum table.
///////////////////////////////////////////////////////////////////////////////
int get(int x, int y, int z);


///////////////////////////////////////////////////////////////////////////////
/// \brief Get a value V(x,y,z) from the sum table, but return 0 if any of 
///        the parameters are less than 0.
///////////////////////////////////////////////////////////////////////////////
int get_check(int x, int y, int z);


///////////////////////////////////////////////////////////////////////////////
/// \brief Gives the number of non-empty voxels in [min+1, max]
///
/// \note R=(min, max], which is non-inclusive on left side. Thus, calling 
///       num(min, max) returns the non-empty voxels in the region [min+1, max],
///       which is inclusive on both ends.
///////////////////////////////////////////////////////////////////////////////
int num(Vec3 const &min, Vec3 const &max);


///////////////////////////////////////////////////////////////////////////////
/// \brief Create a summed volume table (Glassner 90) for \c volume with given
///        \c extents. \c empty is a Callable object that tests a voxel for
///        emptyness.
///////////////////////////////////////////////////////////////////////////////
std::vector<int> const & 
createSumTable(float const *volume, Vec3 extents, std::function<int(float)> empty);


///////////////////////////////////////////////////////////////////////////////
/// \brief Generate \c numPlanes planes with spacing \c delta.
/// \param delta spacing between planes.
/// \param numPlanes Number of planes to generate.
/// \param start Region start in voxels. 
/// \param [out] candidates Return storage.
///////////////////////////////////////////////////////////////////////////////
void genPlanes(int numPlanes, int delta, int start, std::vector<int> &candidates);


///////////////////////////////////////////////////////////////////////////////
/// \brief Find bounding volumes that minimize empty space.
/// \param minEmptyPercent Halting condition, a BV may not be divided if it has
///                        < \c minEmptyPercent of total voxel count emptyness.
/// \param minVoxels Halting condition, a BV may not be divided smaller than
///                  this.
/// \param delta
/// \param [out] bvols Output storage for bounding volumes.
///////////////////////////////////////////////////////////////////////////////
void split(float minEmptyPercent, int minVoxels, int delta, 
        std::vector<BoundingVolume> &bvols);


///////////////////////////////////////////////////////////////////////////////
/// \brief Find bounding volumes that minimize empty space.
///////////////////////////////////////////////////////////////////////////////
void recursiveSplitHelper(Node &n, std::vector<BoundingVolume>);


///////////////////////////////////////////////////////////////////////////////
/// \brief Find the plane in region R=[rmin, rmax] that balances the number 
///        of non-empty voxels on both sides of the plane.
/// \param candidates set of candidate planes to search.
/// \param axis The axis to generate the planes along, perpendicular to the
///             splitting axis.
/// \param rmin The minimum of the bounding box around region.
/// \param rmax The maximum of the bounding box around region.
/// \return A pair of Vec3 that is the min and max of the chosen splitting plane.
///////////////////////////////////////////////////////////////////////////////
Vec3MinMaxPair findPlane(std::vector<int> const &candidates, int axis, 
        Vec3 const &rmin, Vec3 const &rmax);


///////////////////////////////////////////////////////////////////////////////
/// \brief Find the splitting plane that balances the number of non-empty
///        voxels on both sides.
///////////////////////////////////////////////////////////////////////////////
int bv(Vec3 const &vmin, Vec3 const &vmax);


///////////////////////////////////////////////////////////////////////////////
/// \brief Find the splitting plane that balances the number of non-empty
///        voxels on both sides. Returns the bv boundaries in bounds.
///////////////////////////////////////////////////////////////////////////////
int bv(Vec3 const &vmin, Vec3 const &vmax, Vec3MinMaxPair &bounds);


///////////////////////////////////////////////////////////////////////////////
/// \brief Compute the non-empty voxel difference between left and right
///        regions spec'd by the given min and max vectors.
///
/// \return An int that is the non-empty voxel difference.
///////////////////////////////////////////////////////////////////////////////
int diffSides(Vec3 const &leftMin, Vec3 const &leftMax, 
        Vec3 const &rightMin, Vec3 const &rightMax);


void printSumTable();
void printNumCoords(Vec3 const &min, Vec3 const &max);

std::ostream& operator<<(std::ostream &os, Vec3 const &v);
std::ostream& operator<<(std::ostream &os, Vec3MinMaxPair const &v);
std::ostream& operator<<(std::ostream &os, BoundingVolume const &bv);

#endif //POOPYPANTS_POOPY_H
