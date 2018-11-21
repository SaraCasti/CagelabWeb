#ifndef MEANVALUE_H
#define MEANVALUE_H

#include "BarycentricCoordinates.h"
#include <iostream>
#include "vec3.h"
#include "trimesh/trimesh.h"

#define EPS 0.00000000001
#define EPS1 0.0000001
#define sign(i) ((i >= EPS) ? (1) : (-1))

class MeanValue: public BarycentricCoordinates
{
public:
    MeanValue();
    MeanValue(Cage boundingCage, Trimesh* objMesh);
    void computeCoords();
    void deform(Cage deformedCage);
    vector<vector<double> > getCoords();
    vector<double> getDeformedMesh();

protected:
    //Mean value weight functions wj for each vertex pj of the cage
    vector<vector <double> > mvcW;
    Cage cage; //Bounding cage
    Trimesh* mesh; //Original object mesh
};

#endif // MEANVALUE_H
