

#include "data.h"

#include <svt.h>

#include <catch.hpp>

const int X_AXIS = 0, Y_AXIS = 1, Z_AXIS = 2;

void createSumTableWithRegionArray()
{
    createSumTable(region.data(), { 12, 8, 4 },
        [](float voxel) -> int { return static_cast<int>(voxel); });
}


//TEST_CASE("num explodes ", "[num][explodes]")
//{
//    createSumTableWithRegionArray();
//    Vec3 roi_min{0,0,0};
//    Vec3 roi_max{0,0,0};
//
//    int actual = num(roi_min, roi_max);
//    int expected = 0;
//
//    REQUIRE(actual == expected);
//}
//
//TEST_CASE("num finds 12 in [0,0,0]-[5,5,2]", "[num]")
//{
//    createSumTableWithRegionArray();
//    Vec3 roi_min{0,0,0};
//    Vec3 roi_max{5,5,2};
//
//    int actual = num(roi_min, roi_max);
//    int expected = 12;
//
//    REQUIRE(actual == expected);
//}
//
//TEST_CASE("num finds 6 in [0,0,0]-[5,5,1]", "[num]")
//{
//
//    createSumTableWithRegionArray();
//    Vec3 roi_min{0,0,0};
//    Vec3 roi_max{5,5,1};
//
//    int actual = num(roi_min, roi_max);
//    int expected = 6;
//
//    REQUIRE(actual == expected);
//}
//
//TEST_CASE("num finds 0 in [0,0,0]-[5,5,0]", "[num]")
//{
//    createSumTableWithRegionArray();
//    Vec3 roi_min{0,0,0};
//    Vec3 roi_max{5,5,0};
//
//    int actual = num(roi_min, roi_max);
//    int expected = 0;
//
//    REQUIRE(actual == expected);
//}
//
//
//TEST_CASE("bv handles edge on minimum side of region", "[bv][bv_minimum_edge]")
//{
//
//    createSumTableWithRegionArray();
//    Vec3 roi_min{ 0,0,0 };
//    Vec3 roi_max{ 6,6,2 };
//    Vec3MinMaxPair actual_bounds;
//    int actual_n = bv(roi_min, roi_max, actual_bounds);
//    int expected_n = 12;
//    Vec3MinMaxPair expected_bounds{ {2,2,1}, {4,3,2} };
//
//    REQUIRE(actual_n == expected_n);
//    REQUIRE(actual_bounds == expected_bounds);
//}
//
//
//TEST_CASE("bv handles edge on max side of region", "[bv][bv_max_edge_corner]")
//{
//    createSumTableWithRegionArray();
//    Vec3 roi_min{ 7,3,0 };
//    Vec3 roi_max{ 11,7,3 };
//    Vec3MinMaxPair actual_bounds;
//    int actual_n = bv(roi_min, roi_max, actual_bounds);
//    int expected_n = 8;
//    Vec3MinMaxPair expected_bounds{ {8,4,1}, {9,5,2} };
//
//    REQUIRE(actual_n == expected_n);
//    REQUIRE(actual_bounds == expected_bounds);
//}
//
//
//TEST_CASE("findPlane finds plane on X axis", "[findPlane][xaxis]")
//{
//    createSumTableWithRegionArray();
//    Vec3 roi_min{0,0,0};
//    Vec3 roi_max{11,7,3};
//
//    Vec3MinMaxPair actual_pair{ findPlane(12, X_AXIS, roi_min, roi_max) };
//    Vec3MinMaxPair expected_pair{ { 7,0,0 }, { 7,7,3 } };
//
//    REQUIRE(actual_pair == expected_pair);
//}
//
//
//TEST_CASE("findPlane finds plane on Y axis", "[findPlane][yaxis]")
//{
//    createSumTableWithRegionArray();
//    Vec3 roi_min{0,0,0};
//    Vec3 roi_max{11,7,3};
//
//    Vec3MinMaxPair actual_pair{ findPlane(8, Y_AXIS, roi_min, roi_max) };
//    Vec3MinMaxPair expected_pair{ { 0,3,0 }, { 11,3,3 } };
//
//    REQUIRE(actual_pair == expected_pair);
//}




