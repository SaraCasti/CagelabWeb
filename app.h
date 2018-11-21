#ifndef APP_H
#define APP_H

#include <iostream>
#include <vector>
#include <cage/cage.h>
#include <trimesh/trimesh.h>
#include <coordinates/green.h>
#include <coordinates/meanvalue.h>


class App
{
public:
    App();
    bool computeCoords(std::vector<double> meshC, std::vector<int> meshF, std::vector<double> cageC, std::vector<int> cageF);

    void computeMVC(string fileMesh, string fileCage);
    int test(int b);
    std::vector<double> deformMVC(std::vector<double> cageC);
    std::vector<double> deformGreen(std::vector<double> cageC);
    std::vector<double> getCoords4cageV(int id_cv, int baryMode);
    std::vector<double> debugMC();
    std::vector<int> debugMT();
    std::vector<double> debugCC();
    std::vector<int> debugCT();

     //std::vector< std::vector<double> > computeMVC(const char* meshfile, const char* cagefile);
     // std::vector<double> meshCoords, std::vector<int> meshFaces

private:
    Trimesh mesh;
    Cage cage;
    MeanValue mvcCoords;
    Green greenCoords;
};

#endif // APP_H
