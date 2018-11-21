#include "app.h"

App::App()
{

}

bool App::computeCoords(std::vector<double> meshC, std::vector<int> meshF, std::vector<double> cageC, std::vector<int> cageF)
{
    mesh = Trimesh(meshC,meshF);

    cage = Cage(cageC,cageF);

    greenCoords = Green(cage, &mesh);
    mvcCoords = MeanValue(cage, &mesh);

    return true;
}

void App::computeMVC(std::string fileMesh, std::string fileCage)
{
    std::vector <std::vector<double> > mvc;
    mesh = Trimesh(fileMesh.c_str());

    Cage cage = Cage(fileCage.c_str());

    //return cage.vector_coords();

//    Green greenCoord = Green(cage, &mesh);
    mvcCoords = MeanValue(cage, &mesh);

    mvc = mvcCoords.getCoords();
}

int App::test(int b)
{
    return b*2;
}

std::vector<double> App::deformMVC(std::vector<double> cageC)
{
    Cage deformed_cage = Cage(cageC,cage.vector_triangles());
    mvcCoords.deform(deformed_cage);
    return mvcCoords.getDeformedMesh();
}

std::vector<double> App::deformGreen(std::vector<double> cageC)
{
    Cage deformed_cage = Cage(cageC,cage.vector_triangles());
    greenCoords.deform(deformed_cage);
    greenCoords.getDeformedMesh();
}
