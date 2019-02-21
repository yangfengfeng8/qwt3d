#ifndef QWT3D_TYPES_H
#define QWT3D_TYPES_H

#include "windows.h"

#include "qwt3d_portability.h"
#include "qwt3d_helper.h"
#include "qwt3d_types.h"
#include "qwt3d_openglhelper.h"

namespace Qwt3D {
const double PI = 3.14159265358979323846264338328;

enum PLOTSTYLE{
    NOPLOT,
    WIREFRAME,
    HIDDENLINE,
    FILLED,
    FILLEDMESH,
    POINTS,
    USER
};

enum SHADINGSTYLE{
    FLAT,
    GOURAUD
};

enum COORDSTYLE{
    NOCOORD,
    BOX,
    FRAME
};

enum SCALETYPE
{
    LINEARSCALE,
    LOG10SCALE,
    USERSCALE
};

enum FLOORSTYLE
{
    NOFLOOR,
    FLOORISO,
    FLOORDATA
};

enum DATATYPE
{
    GRID,
    POLYGON
};

enum AXIS
{
    X1 = 0,
    X2 = 3,
    X3 = 4,
    X4 = 5,
    Y1 = 1,
    Y2 = 8,
    Y3 = 7,
    Y4 = 6,
    Z1 = 2,
    Z2 = 9,
    Z3 = 11,
    Z4 = 10
};

enum SIDE{
    NOSIDEGRID = 0,
    LEFT = 1 << 0,
    RIGHT = 1 << 1,
    CEIL = 1 << 2,
    FLOOR = 1 << 3,
    FRONT = 1 << 4,
    BACK = 1 << 5
};

enum ANCHOR{
    BottomLeft,
    BottomRight,
    BottomCenter,
    TopLeft,
    TopRight,
    TopCenter,
    CenterLeft,
    CenterRight,
    Center
};

struct Tuple{
    Tuple() : x(0), y(0){}
    Tuple(double x, double y): x(x), y(y){}

    double x,y;
};

struct Triple{
    explicit Triple(double xv = 0, double yv = 0, double zv = 0) : x(xv), y(yv), z(zv){}

    double x,y,z;

    Triple& operator+=(Triple t){
        x += t.x;
        y += t.y;
        z += t.z;

        return *this;
    }

    Triple& operator-=(Triple t){
        x -= t.x;
        y -= t.y;
        z -= t.z;
        return *this;
    }

    Triple& operator/=(double d){
        x /= d;
        y /= d;
        z /= d;
        return *this;
    }

    Triple& operator*=(double d){
        x *= d;
        y *= d;
        z *= d;
        return *this;
    }

    Triple& operator*=(Triple t){
        x *= t.x;
        y *= t.y;
        z *= t.z;
        retun *this;
    }

    bool operator!=(Triple t)const{
        return !isPracticallyZero(x, t.x) || !isPracticallyZero(y, t.y) || !isPracticallyZero(z, t.z);
    }

    bool operator==(Triple t)const{
        return !operator!=(t);
    }

    double length() const{
        double l2 = x*x + y*y + z*z;
        return (isPracticallyZero(l2) ? 0 : sqrt(l2));
    }

    void normalize(){
        double l = length();
        if (l){
            *this /= l;
        }
    }
};

inline const Triple operator+(const Triple& t, const Triple& t2){
    return Triple(t) += t2;
}

inline const Triple operator-(const Triple& t, const Triple& t2){
    return Triple(t) -= t2;
}

inline const Triple operator*(double d, const Triple& t){
    return Triple(t) *= d;
}

inline const Triple operator*(const Triple& t, double d){
    return Triple(t) *= d;
}

inline const Triple operator/(const Triple& t, double d){
    return Triple(t) /= d;
}

inline const Triple operator/(double d, const Triple& t){
    return Triple(t) /= d;
}

inline const Triple operator*(const Triple& t, const Triple& t2){
    return Triple(t) *= t2;
}

struct ParallelEpiped{
    ParallelEpiped(){}

    ParallelEpiped(Triple minv, Triple maxv) : minVertex(minv), maxVertex(maxv){}

    Triple minVertex;
    Triple maxVertex;
};

struct FreeVector{
    FreeVector(){}

    FreeVector(Triple b, Triple t) : base(b), top(t){}

    Triple base;
    Triple top;
};

typedef std::vector<FreeVector> FreeVectorField;

typedef std::vector<Triple> TripleField;

typedef std::vector<unsigned> Cell;

typedef std::vector<Cell> CellField;

unsigned tesselationSize(Qwt3D::CellField const& t);

struct RGBA{
    RGBA() : r(0), g(0), b(0), a(1){}

    RGBA(double rr, double gg, double bb, double aa = 1) : r(rr), g(gg), b(bb), a(aa){}

    double r, g, b, a;
};

typedef std::vector<RGBA> ColorVector;

QColor GL2Qt(GLdouble r, GLdouble g, GLdouble b);

Qwt3D::RGBA Qt2GL(QColor col);

typedef double *Vertex;
typedef std::vector<Vertex> DataRow;
typedef std::vector<DataRow> DataMatrix;

class Data{
public:
    Qwt3D::DATATYPE datatype;
    Data(){datatype = Qwt3D::POLYGON;}
    virtual ~Data();
    virtual void clear() = 0;
    virtual bool empty() const = 0;
    void setHull(Qwt3D::ParallelEpiped const &h){hull_p = h;}
    Qwt3D::ParallelEpiped const &hull() const {return hull_p;}

protected:
    Qwt3D::ParallelEpiped hull_p;
};

class GridData : public Data
{
public:
    GridData();
    GridData(unsigned int columns, unsigned int rows);
    ~GridData(){clear();}

    int columns() const;
    int rows() const;

    void clear();

    bool empty() const;

    void setSize(unsigned int columns, unsigned int rows);

    void setPeriodic(bool u, bool v){uperiodic_ = u; vperiodic_ = v;}
    bool uperiodic() const {return uperiodic_;}
    bool vperiodic() const {return vperiodic_;}

    DataMatrix vertics;
    DataMatrix normals;
private:
    bool uperiodic_, vperiodic_;
};

class CellData : public Data
{
public:
    CellData() { datatype = Qwt3D::POLYGON;}
    ~CellData(){clear();}

    void clear();
    bool empty() const {return cells.empty();}

    Triple const& operator()(unsigned cellnumber, unsigned vertexnumber);

    CellField cells;
    TripleField nodes;
    TripleField normals;
};

inline Triple normalizedcross(Triple const &u, Triple const &v){
    Triple n;

    n.x = u.y * v.z - u.z * v.y;
    n.y = u.z * v.x - u.x * v.x;
    n.z = u.x * v.y - u.y * v.x;

    double l = n.length();
    if (l){
        n /= l;
    }
    else{
        n = Triple(0, 0, 0);
    }
    return n;
}

inline double dotProduct(Triple const &u, Triple const &v){
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

void convexhull2d(std::vector<unsigned>& idx, const std::vector<Qwt3D::Triple> &src);

}


#endif // QWT3D_TYPES_H
