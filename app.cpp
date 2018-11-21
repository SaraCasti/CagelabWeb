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

    cage = Cage(fileCage.c_str());

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
    return greenCoords.getDeformedMesh();
}

std::vector<double> App::getCoords4cageV(int id_cv, int baryMode)
{
    std::vector<std::vector<double> > coords;
    if(baryMode==MVC_COORDS)
    {
         coords = mvcCoords.getCoords();
    }
    else if(baryMode==GREEN_COORDS)
    {
        coords = greenCoords.getCoords();
    }
    std::vector<double> influence;
    for(int i=0; i<coords.size(); i++)
    {
        influence.push_back(coords[i][id_cv]);
    }
    return influence;
}

std::vector<double> App::debugMC()
{
    return mesh.vector_coords();
}

std::vector<int> App::debugMT()
{
    return mesh.vector_triangles();
}

std::vector<double> App::debugCC()
{
    return cage.vector_coords();
}

std::vector<int> App::debugCT()
{
    return cage.vector_triangles();
}
