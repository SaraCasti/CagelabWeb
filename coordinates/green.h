#ifndef GREEN_H
#define GREEN_H

#include "coordinates/BarycentricCoordinates.h"
#include "cage/cage.h"
#include "Eigen/Geometry"
using namespace Eigen;

#define EPS 0.00000000001
#define EPS1 0.0000001
#define EPS2 0.00001
#define SCALE 100000
#define sign(i) ((i >= EPS) ? (1) : (-1))

class Green: public BarycentricCoordinates
{
public:
    Green();
    Green(Cage boundingCage, Trimesh* objMesh);
    void computeCoords();
    void deform(Cage deformedCage);
    void partialDeform(vector<int> deformedVert, Cage deformedCage);
    vector<vector<double> > getCoords();
    vector<double> getDeformedMesh();

protected:
    //Utilities methods for the calculation of coords and mesh deformation
    double gcTriInt(vec3d p, vec3d t1, vec3d t2, vec3d eta);
    void calcScalingFactors(Cage deformedCage);

    //Green cooordinate functions and scaling factors
    vector< vector<double> > gcV; //φi(η)  i∈ V
    vector< vector<double> > gcF; //ψj(η)  j∈ F
    vector<double> gcS;  //Scaling factors
    Cage cage; //Bounding cage
    Trimesh* mesh; //Original object mesh
    vector<int> exitFaces;

};

#endif // GREEN_H
