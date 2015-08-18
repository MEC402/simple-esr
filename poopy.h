//
// Created by jim on 8/16/15.
//

#ifndef POOPYPANTS_POOPY_H
#define POOPYPANTS_POOPY_H

#include <functional>
#include <cstdlib>

struct Vec3{
    int x;
    int y;
    int z;
};

size_t to1d(size_t x, size_t y, size_t z, size_t maxX, size_t maxY);
int get(int x, int y, int z);
int get_check(int x, int y, int z);

///////////////////////////////////////////////////////////////////////////////
/// \brief Gives the number of non-empty voxels in [min+1, max]
///
/// \note R=(min, max] (R non-inclusive on left side).
///////////////////////////////////////////////////////////////////////////////
int num(Vec3 const &min, Vec3 const &max);

void createSumTable(float *volume, Vec3 extents, std::function<int(int)> empty);
void findPlane(int delta, int axis, Vec3 const &rmin, Vec3 const &rmax);
int bv(Vec3 const &vmin, Vec3 const &vmax);

void printSumTable();
void printNumCoords(Vec3 const &min, Vec3 const &max);

std::ostream& operator<<(std::ostream &os, Vec3 const &v);

#endif //POOPYPANTS_POOPY_H
