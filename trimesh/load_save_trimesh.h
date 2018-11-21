/**
 @author    Marco Livesu (marco.livesu@gmail.com)
 @copyright Marco Livesu 2014.
*/

#ifndef LOAD_SAVE_TRIMESH_H
#define LOAD_SAVE_TRIMESH_H

#include <vector>

// OBJ FILES
//
void load_OBJ(const char          * filename,
              std::vector<double> & xyz,
              std::vector<int>    & tri);

void save_OBJ(const char          * filename,
              std::vector<double> & xyz,
              std::vector<int>    & tri);

void save_coords(const char * filename, std::vector<std::vector <double> > & coords );

#endif // LOAD_SAVE_TRIMESH_H
