
#ifndef node_h__
#define node_h__

#include "axis.h"
#include "boundingvolume.h"

/////////////////////////////////////////////////////////////////////////// 
/// \brief A node!!!! In a tree!!!
/////////////////////////////////////////////////////////////////////////// 
class Node
{
public:
    
    /////////////////////////////////////////////////////////////////////////// 
    /// \brief Constructs a leaf node with an empty and volumeless bounding box
    /////////////////////////////////////////////////////////////////////////// 
    Node()
        : Node(0, true, 0, Vec3{0,0,0}, Vec3{0,0,0})
    { }


    /////////////////////////////////////////////////////////////////////////// 
    /// \brief Constructs a node with given things and whatnots.
    /////////////////////////////////////////////////////////////////////////// 
    Node(int idx, bool isLeaf, int nonEmptyVoxels, const Vec3& min, const Vec3& extent)
        : m_index{ 0 }
        , m_isLeaf{ isLeaf }
        , m_bv{ nonEmptyVoxels, min, extent }
        , m_axis{ m_bv.shortestAxis() }
    { }

    Axis shortestAxis() const { return m_axis; }

    /// \brief This Nodes index in the tree array.
    void index(int i) { m_index = i; }
    int index() const { return m_index; }

    void isLeaf(bool leafyness) { m_isLeaf = leafyness; }
    bool isLeaf() const { return m_isLeaf; }

    int leftChild()  const { return 2 * m_index + 1; }
    int rightChild() const { return 2 * m_index + 2; }

    BoundingVolume& bv() { return m_bv; }
    const BoundingVolume& bv() const { return m_bv; }

    const Vec3& min()    const { return m_bv.min(); }
    const Vec3& extent() const { return m_bv.extent(); }
    Vec3 max() const { return m_bv.min() + m_bv.extent(); }

    float percentEmpty() const { return m_bv.percentEmpty(); }
    int numVoxels() const { return m_bv.totalVoxels(); }

private:
    int m_index;
    bool m_isLeaf;
    BoundingVolume m_bv;
    Axis m_axis;
};

#endif  // !node_h__
