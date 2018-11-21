#ifndef BARYCENTRICCOORDINATES_H
#define BARYCENTRICCOORDINATES_H
#include "cage/cage.h"

class BarycentricCoordinates{
public:
    BarycentricCoordinates(){}
    virtual void computeCoords()=0;
    virtual void deform(Cage deformedCage)=0;
    virtual vector<double> getDeformedMesh()=0;
    virtual vector< vector<double> > getCoords()=0;

};


#endif // BARYCENTRICCOORDINATES_H
