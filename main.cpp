#include <iostream>
#include <cage/cage.h>
#include <trimesh/trimesh.h>
#include <coordinates/green.h>
#include <coordinates/meanvalue.h>
#include <app.h>

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;

//    Trimesh<double> mesh = Trimesh<double>(argv[1]);

//    Cage cage = Cage(argv[2]);


//    Green green = Green(cage, &mesh);
//    MeanValue mvc = MeanValue(cage, &mesh);

//    std::vector<std::vector<double> > coords;
//    coords = green.getCoords();

//    for(int i=0; i<coords.size(); i++)
//        for(int j=0; j<coords[i].size(); j++)
//            std::cout << coords[i][j] << endl;

    App a = App();
    string meshF = "/Users/Sary/Documents/CageLab_Internato/models_and_cages/armadillo.obj";
    string cageF = "/Users/Sary/Documents/CageLab_Internato/models_and_cages/armadillo_cage.obj";
    a.computeMVC(meshF, cageF);

    return 0;
}
