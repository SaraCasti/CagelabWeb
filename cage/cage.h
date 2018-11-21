#ifndef CAGE_H
#define CAGE_H

#include <assert.h>
#include <float.h>
#include <vector>
#include <map>
#include <set>
#include <stdlib.h>

#include "bbox.h"
#include "utils.h"
#include "vec3.h"
#include "trimesh/trimesh.h"

using namespace std;

class Cage: public Trimesh
{

    public:

    Cage();
    Cage(const char *filename);
    Cage(const std::vector<double>& coords, const std::vector<int>& tris);


};

#endif // CAGE_H
