#ifndef QWT3D_ENRICHMENT_H
#define QWT3D_ENRICHMENT_H

#include "qwt3d_types.h"

namespace Qwt3D {

class Qwt3d_plot;

class Enrichment{
public:
    enum TYPE{
        VERTEXENRICHMENT,
        EDGEENRICHMENT,
        FACEENRICHMENT,
        VOXELENRICHMENT
    };

    Enrichment() : plot(0){}
    virtual ~Enrichment(){}
    virtual Enrichment* clone() const = 0;
    virtual void drawBegin(){}
    virtual void drawEnd(){}
    virtual void assign(Qwt3d_plot const &pl){plot = &pl;}
    virtual TYPE type() const = 0;

protected:
    const Qwt3d_plot *plot;
};

class VertexEnrichment : public Enrichment
{
public:
    VertexEnrichment() : Qwt3D::Enrichment(){}
    virtual Enrichment *clone() const = 0;
    virtual void draw(Qwt3D::Triple const &) = 0;
    virtual TYPE type() const {return Qwt3D::Enrichment::VERTEXENRICHMENT;}
};

}

#endif // QWT3D_ENRICHMENT_H
