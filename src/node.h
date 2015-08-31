
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
    Node() : Node(Axis::X, true, { 0, Vec3{0,0,0}, Vec3{0,0,0} }) 
    { }


    /////////////////////////////////////////////////////////////////////////// 
    /// \brief Constructs a node with given things and whatnots.
    /////////////////////////////////////////////////////////////////////////// 
    Node(Axis a, bool leaf, BoundingVolume bv) 
        : m_axis{ a }
        , m_index{ 0 }
        , m_isLeaf{ leaf }
        , m_bv{ bv }
//      , m_minPlane{ minP }
//      , m_maxPlane{ maxP }
    { }

    Axis axis() const { return m_axis; }

    /// \brief This Nodes index in the tree array.
    void index(int i) { m_index = i; }
    int index() const { return m_index; }

//  int minP() const { return m_minPlane; }
//  int maxP() const { return m_maxPlane; }

//  int leftChild() const { return 2 * m_index + 1; }
//  int rightChild() const { return 2 * m_index + 2; }

private:
    Axis m_axis;
    int m_index;
    bool m_isLeaf;
    BoundingVolume m_bv;
//  int m_minPlane;
//  int m_maxPlane;
};

#endif  // !node_h__
