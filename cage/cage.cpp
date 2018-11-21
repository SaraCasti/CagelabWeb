#include "cage.h"

Cage::Cage() : Trimesh()
{

}

Cage::Cage(const char *filename) : Trimesh(filename)
{

}

Cage::Cage(const std::vector<double> &coords, const std::vector<int> &tris) : Trimesh(coords,tris)
{

}
