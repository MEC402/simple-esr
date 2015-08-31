//
// Created by jim on 8/16/15.
//

#ifndef POOPYPANTS_POOPY_H
#define POOPYPANTS_POOPY_H

#include "axis.h"
#include "vec3.h"
#include "boundingvolume.h"
#include "node.h"

#include <functional>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>

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
/// \param delta aieee!
/// \param [out] bvols Output storage for bounding volumes.
///////////////////////////////////////////////////////////////////////////////
void split(float minEmptyPercent, int minVoxels, int delta, 
        std::vector<Node> &bvols);


///////////////////////////////////////////////////////////////////////////////
/// \brief Find bounding volumes that minimize empty space.
///////////////////////////////////////////////////////////////////////////////
void recursiveSplitHelper(Node &n, Axis axis, std::vector<Node> &);


///////////////////////////////////////////////////////////////////////////////
/// \brief Find the plane in region R=[rmin, rmax] that balances the number 
///        of non-empty voxels on both sides of the plane.
/// \param[in] candidates set of candidate planes to search.
/// \param[in] a Axis along with to split.
/// \param[in] bv BoundingVolume containing the region to split.
/// \return The plane.
///////////////////////////////////////////////////////////////////////////////
Plane findPlane(std::vector<int> const &candidates, Axis a, BoundingVolume const &bv);


///////////////////////////////////////////////////////////////////////////////
/// \brief Shrink the BoundingVolume and update the non-empty voxel count.
/// \return Number of non-empty voxels, or num(R).
///////////////////////////////////////////////////////////////////////////////
int bv(Vec3 const &rmin, Vec3 const &rmax);
//void bv(BoundingVolume &volume);


///////////////////////////////////////////////////////////////////////////////
/// \brief Find the splitting plane that balances the number of non-empty
///        voxels on both sides. 
/// \param[in] vmin Minimum point of the bounding volume (0-indexed).
/// \param[in] vmax Maximum point of the bounding volume (0-indexed).
/// \param[out] bounds Bounding volume boundaries.
///
/// \return Number of non-empty voxels, or num(R).
///////////////////////////////////////////////////////////////////////////////
//int bv(Vec3 const &vmin, Vec3 const &vmax, Vec3MinMaxPair &bounds);


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
std::ostream& operator<<(std::ostream &os, Plane const &v);
std::ostream& operator<<(std::ostream &os, BoundingVolume const &bv);

#endif //POOPYPANTS_POOPY_H
