/**
 @author    Marco Livesu (marco.livesu@gmail.com), Fabrizio Corda, Sara Casti
 @copyright Marco Livesu, Fabrizio Corda, Sara Casti 2014.
*/

#include "load_save_trimesh.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>


using namespace std;




void save_OBJ(const char            * filename,
              std::vector<double>   & xyz,
              std::vector<int>      & tri)
{


    ofstream fp;
    fp.open (filename);
    fp.precision(6);
    fp.setf( std::ios::fixed, std:: ios::floatfield ); // floatfield set to fixed

    if(!fp)
    {
        cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : save_OBJ() : couldn't open output file " << filename << endl;
        exit(-1);
    }

    //cout << "v " << xyz[3] << " " << xyz[4] << " " << xyz[5] << endl;

    for(int i=0; i<(int)xyz.size(); i+=3)
    {
        fp << "v " << xyz[i] << " " << xyz[i+1] << " " << xyz[i+2] << endl;
    }

    for(int i=0; i<(int)tri.size(); i+=3)
    {
        fp << "f " << tri[i]+1 << " " << tri[i+1]+1 << " " << tri[i+2]+1 << endl;
    }

    fp.close();
}

void save_coords(const char  * filename, std::vector<std::vector <double> > & coords )
{


    //FILE *fp = fopen(filename, "w");
    ofstream fp;
    fp.open(filename);

    if(!fp)
    {
        std::cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : save_OBJ() : couldn't open input file " << filename << endl;
        exit(-1);
    }

    //fprintf(fp, "#i j coord[i][j]");

    for(int i=0; i<coords.size(); i++)
    {
        for(int j=0; j<coords[i].size(); j++)
        {


            fp << i << " " <<  j << " " << coords[i][j] << std::endl;
        }
    }

    fp.close();
}
