#ifndef UTILS_H
#define UTILS_H

//#ifdef __APPLE__
//#include <gl.h>
//#include <glu.h>
//#else
//#include <GL/gl.h>
//#include <GL/glu.h>
//#endif

#include <fstream>

using namespace std;

typedef pair<int,int> edge;

inline edge unique_edge(int v0, int v1)
{
    edge e;
    e.first  = min(v0,v1);
    e.second = max(v0,v1);
    return e;
}

// Different selection modes
enum OperationMode { NONE, ADD, REMOVE, DEFORM };

//Different coloration modes
enum ColorationMode { VERTEX_COLOR, TRIANGLE_COLOR, INFLUENCE_COLOR };

//Different barycentric coordinates modes
enum BarycentricMode { NONE_COORDS, GREEN_COORDS, MVC_COORDS };

//inline void gl_sphere(float x, float y, float z, float radius, bool selected)
//{
//        glEnable(GL_LIGHTING);
//        glShadeModel(GL_SMOOTH);
//        if(selected)
//        {
//            glColor3d(0.9, 0.2, 0.2);
//        } else {
//            glColor3d(0.99, 0.85, 0.39);
//        }
//        glPushMatrix();
//        glTranslated(x, y, z);
//        GLUquadric *sphere = gluNewQuadric();
//        gluQuadricOrientation(sphere,GLU_OUTSIDE);
//        gluSphere(sphere, radius, 30, 30);
//        glPopMatrix();
//}



#endif // UTILS_H
