
#ifndef boundingvolume_h__
#define boundingvolume_h__

#include "axis.h"
#include "vec3.h"

#include <ostream>
#include <sstream>
#include <string>

class BoundingVolume{
public:

    BoundingVolume(int n, Vec3 const &min, Vec3 const &extent)
        //: BoundingVolume(n, {min, extent} )
        : m_nonEmptyVoxels{ n }
        , m_min{ min }
        , m_extent{ extent }
    { }


    BoundingVolume(BoundingVolume const & o)
        : m_nonEmptyVoxels{ o.m_nonEmptyVoxels }
        , m_min{ o.m_min }
        , m_extent{ o.m_extent }
    { }


    
    ////////////////////////////////////////////////////////////////////////////
    /// \brief Return the position of the corner closest to the origin of 
    ///        this BoundingVolume.
    ////////////////////////////////////////////////////////////////////////////
    Vec3 const & min() const { return m_min; }
    void min(Vec3 const &m) { m_min = m; }


    ////////////////////////////////////////////////////////////////////////////
    /// \brief Return the length of the sides of this BoundingVolume.
    ////////////////////////////////////////////////////////////////////////////
    Vec3 const & extent() const { return m_extent; }
    void extent(Vec3 const &e) { m_extent = e; }


    ////////////////////////////////////////////////////////////////////////////
    /// \brief Return the number of non-empty voxels in this BoundingVolume.
    ////////////////////////////////////////////////////////////////////////////
    int nonEmptyVoxels() const { return m_nonEmptyVoxels; }
    void nonEmptyVoxels(int n) { m_nonEmptyVoxels = n; }

    int totalVoxels() const { return m_extent.x * m_extent.y * m_extent.z; }


    //////////////////////////////////////////////////////////////////////////
    /// \brief Percentage of non-empty voxels to total voxels
    //////////////////////////////////////////////////////////////////////////
    float percentEmpty() const
    {
        return nonEmptyVoxels() / static_cast<float>(totalVoxels());
    }


    ////////////////////////////////////////////////////////////////////////////
    /// \brief Return the shortest Axis of this BoundingVolume.
    ////////////////////////////////////////////////////////////////////////////
    Axis shortestAxis() const 
    {
        int shortest{ extent().x };
        Axis a{ Axis::X };

        if ( extent().y < shortest ) {
            shortest = extent().y;
            a = Axis::Y;
        }
        if ( extent().z < shortest ) {
            shortest = extent().z;
            a = Axis::Z;
        }

        return a;
    }

    std::string to_string() const
    {
        std::stringstream ss;
        ss << "{" << min() << ", " << extent() << ", " << nonEmptyVoxels() << "}";
        return ss.str();
    }

    ////////////////////////////////////////////////////////////////////////////
    friend std::ostream& operator<<(std::ostream &os, BoundingVolume const &bv)
    {
        return os << bv.to_string();
    }

private:
    int m_nonEmptyVoxels;
    Vec3 m_min;
    Vec3 m_extent;
};

#endif // !boundingvolume_h__
