#include "qwt3d_types.h"
#include <stdlib.h> // qsort
#include <algorithm>
#include <float.h>
#include <QColor>

using namespace Qwt3D;

namespace {
    typedef double coordinate_type;

    int ccw(coordinate_type **p, int i, int j, int k){
        coordinate_type a = p[i][0] - p[j][0],
                b = p[i][1] - p[j][1],
                c = p[k][0] - p[j][0],
                d = p[k][1] - p[j][1];
        return a*d - b*c <= 0;
    }

#define CMPM(c, A, B) \
    v = (*(coordinate_type**)A)[c] - (*(coordinate_type**)B)[c];\
    if (v > 0) return 1;\
    if (v < 0) return -1;

    int cmpl(const void *a, const void*b){
        double v;
        CMPM(0, a, b);
        CMPM(1, a, b);
        return 0;
    }

    int cmph(const void *a, const void *b){return cmpl(a, b);}

    int make_chain(coordinate_type **V, int n, int (*cmp)(const void*, const void*)){
        int i, j, s = 1;
        coordinate_type *t;
        qsort(V, n, sizeof(coordinate_type*), cmp);
        for (i = 2; i < n; i++){
            for (j = s; j >= 1 && ccw(V, i, j, j - 1); j--){
                ;
            }
            s = j + 1;
            t = V[s];
            V[s]= V[i];
            V[i] = t;
        }
        return s;
    }

    int _ch2d(coordinate_type **P, int n){
        int u = make_chain(P, n, cmpl);
        if (!n)return 0;
        P[n] = P[0];
        return u + make_chain(P + u, n - u + 1, cmph);
    }
}

unsigned Qwt3D::tesselationSize(const Qwt3D::CellField &t)
{
    unsigned ret = 0;

    for (unsigned i = 0; i != t.size(); ++i){
        ret += t[i].size();
    }

    return ret;
}


QColor Qwt3D::GL2Qt(GLdouble r, GLdouble g, GLdouble b)
{
    return QColor(round(r * 255), round(g * 255), round(b * 255));
}

Qwt3D::RGBA Qwt3D::Qt2GL(QColor col)
{
    QRgb qrgb = col.rgb();
    RGBA rgba;
    rgba.r = qRed(qrgb) / 255.0;
    rgba.g = qGreen(qrgb) / 255.0;
    rgba.b = qBlue(qrgb) / 255.0;
    rgba.a = qAlpha(qrgb) / 255.0;

    return rgba;
}

Qwt3D::GridData::GridData()
{
    GridData(0, 0);
}

Qwt3D::GridData::GridData(unsigned int columns, unsigned int rows)
{
    datatype = Qwt3D::GRID;
    setSize(columns, rows);
    setPeriodic(false, false);
}

int Qwt3D::GridData::columns() const
{
    return (int)vertics.size();
}

int Qwt3D::GridData::rows() const
{
    return (empty() ? 0 : (int)vertics[0].size());
}

void Qwt3D::GridData::clear()
{
    setHull(ParallelEpiped());
    for (unsigned i = 0; i != vertics.size(); ++i){
        for (unsigned j = 0; j != vertics[i].size(); ++j){
            delete[] vertics[i][j];
        }
        vertics[i].clear();
    }
    vertics.clear();

    for (unsigned i = 0; i != normals.size(); ++i){
        for (unsigned j = 0; j != normals[i].size(); ++j){
            delete[] normals[i][j];
        }
        normals[i].clear();
    }
    normals.clear();
}

bool Qwt3D::GridData::empty() const
{
    return vertics.empty();
}

void Qwt3D::GridData::setSize(unsigned int columns, unsigned int rows)
{
    this->clear();

    vertics = std::vector<DataRow>(columns);
    for (unsigned int i = 0; i != vertics.size(); ++i){
        vertics[i] = DataRow(rows);
        for (unsigned int j = 0; j != vertics[i].size(); ++j){
            vertics[i][j] = new GLdouble[3];
        }
    }

    normals = std::vector<DataRow>(columns);
    for (unsigned int i = 0; i != vertics.size(); ++i){
        normals[i] = DataRow(rows);
        for (unsigned int j = 0; j != vertics[i].size(); ++j){
            normals[i][j] = new GLdouble[3];
        }
    }
}

void Qwt3D::CellData::clear()
{
    setHull(ParallelEpiped());
    cells.clear();
    nodes.clear();
    normals.clear();
}

const Qwt3D::Triple &Qwt3D::CellData::operator()(unsigned cellnumber, unsigned vertexnumber)
{
    return nodes[cells[cellnumber][vertexnumber]];
}

void Qwt3D::convexhull2d(std::vector<unsigned> &idx, const std::vector<Qwt3D::Triple> &src)
{
    idx.clear();
    if (src.empty()){
        return ;
    }
    if (src.size() == 1){
        idx.push_back(0);
        return ;
    }
    coordinate_type** points = new coordinate_type*[src.size() + 1];
    coordinate_type* P = new coordinate_type[src.size() * 2];

    int i = 0;
    for (i = 0; i < (int)src.size(); ++i){
        points[i] = &P[2 * i];
        points[i][0] = src[i].x;
        points[i][1] = src[i].y;
    }

    coordinate_type *start = points[0];
    int m = _ch2d(points, src.size());
    idx.resize(m);

    for (int i = 0; i < m; ++i){
        idx[i] = (points[i] - start) / 2;
    }

    delete[] points;
    delete [] P;
}
