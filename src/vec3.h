
#ifndef vec3_h__
#define vec3_h__

struct Vec3
{
    Vec3() 
        : Vec3(0,0,0) 
    { }
    

    Vec3(int x, int y, int z) 
        : x{ x } , y{ y } , z{ z } 
    { }


    Vec3(Vec3 const &o) 
        : x{ o.x }, y{ o.y }, z{ o.z } 
    { }


    bool operator==(Vec3 const &o)
    {
        return x == o.x && y == o.y && z == o.z;
    }


    Vec3 operator+(int s) 
    {
        Vec3 ret{ *this };
        ret.x += s;
        ret.y += s;
        ret.z += s;

        return ret;
    }


    Vec3 operator-(int s) 
    {
        Vec3 ret{ *this };
        ret.x -= s;
        ret.y -= s;
        ret.z -= s;

        return ret;
    }

    int x;
    int y;
    int z;
};


struct Plane
{
    Plane() : Plane({ 0,0,0 }, { 0,0,0 }) { }

    Plane(Vec3 const &vmin, Vec3 const &vmax) : m_min{vmin}, m_max{vmax} { }

    Plane(Plane const &o) : m_min{ o.m_min }, m_max{ o.m_max } { }

    bool operator==(Plane const &o) { return m_min == o.m_min && m_max == o.m_max; }

    Vec3 const & min() const { return m_min; }
    void min(Vec3 const &m) { m_min = m; }

    Vec3 const & max() const { return m_max; }
    void max(Vec3 const &m) { m_max = m; }

private:
    Vec3 m_min;
    Vec3 m_max;
};

#endif // !vec3_h__
