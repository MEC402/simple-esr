//
// Created by jim on 8/16/15.
//

#ifndef POOPYPANTS_POOPY_H
#define POOPYPANTS_POOPY_H

#include <functional>
#include <cstdlib>
#include <vector>
#include <iostream>

struct Vec3{
    Vec3() : Vec3(0,0,0) { }
    Vec3(int x, int y, int z) : x{ x } , y{ y } , z{ z } { }

    bool operator==(Vec3 const &o)
    {
        return x == o.x && y == o.y && z == o.z;
    }

    std::ostream& operator<<(std::ostream &o) const
    {
        return o << "{" << x << ", " << y << ", " << z << "}";
    }

    int x;
    int y;
    int z;
};

struct Vec3MinMaxPair{
    Vec3MinMaxPair()
        : Vec3MinMaxPair({ 0,0,0 }, { 0,0,0 }) { }

    Vec3MinMaxPair(Vec3 const &vmin, Vec3 const &vmax)
        : min{vmin}, max{vmax} { }

    bool operator==(Vec3MinMaxPair const &o)
    {
        return min == o.min && max == o.max;
    }

    Vec3 min;
    Vec3 max;
};


struct BoundingVolume{
    BoundingVolume(int n, Vec3 const &min, Vec3 const &max)
        : BoundingVolume(n, {min, max} )
    { }

    BoundingVolume(int n, Vec3MinMaxPair const &minmax)
        : nonEmptyVoxels{ n }
        , minmax{ minmax }
    { }


    int nonEmptyVoxels;
    Vec3MinMaxPair minmax;
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
/// \brief Find the plane in region R=[rmin, rmax] that balances the number 
///        of non-empty voxels on both sides of the plane.
/// \param perc The percentage of the extent along \c axis that should be used as
///             spacing between planes. This needs to be small enough to guarantee
///             enough planes are generated to span the extent of the splitting region.
/// \param axis The axis to generate the planes along, perpendicular to the
///             splitting axis.
/// \param rmin The minimum of the bounding box around region.
/// \param rmax The maximum of the bounding box around region.
/// \return A pair of Vec3 that is the min and max of the chosen splitting plane.
///////////////////////////////////////////////////////////////////////////////
Vec3MinMaxPair findPlane(int numPlanes, int axis, Vec3 const &rmin, Vec3 const &rmax);


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

void printSumTable();
void printNumCoords(Vec3 const &min, Vec3 const &max);

std::ostream& operator<<(std::ostream &os, Vec3 const &v);
std::ostream& operator<<(std::ostream &os, Vec3MinMaxPair const &v);

#endif //POOPYPANTS_POOPY_H
