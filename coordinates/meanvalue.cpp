#include "meanvalue.h"

MeanValue::MeanValue()
{
}

MeanValue::MeanValue(Cage boundingCage, Trimesh *objMesh){
    cage = boundingCage;
    mesh = objMesh;

    //Init mvcW weight functions
    mvcW.resize(mesh->num_vertices());
    for(int i=0; i<mesh->num_vertices(); i++)
        mvcW[i].resize(cage.num_vertices(), 0.0);

    computeCoords();
}

//Compute MVC weight fuctions
void MeanValue::computeCoords(){
    /**
      *   j = 0 .. cageFaces.size() for each face 3 vertices  j=j+3
      *   i = 0 .. objMesh.num_vertices()
      */

    //Cage vertices and coordinates
    vector<double> cageCoords = cage.vector_coords();
    vector<int> cageFaces = cage.vector_triangles();
    //Coordinates of the vertices of the mesh
    vector<double> objCoords = mesh->vector_coords();

    //Resize mvcW vector dimension: (num_mesh_vertices x num_cage_vertices)
    if(mesh->num_vertices()>0)
    {
        mvcW.resize(mesh->num_vertices());
        for(int i=0; i<mesh->num_vertices(); i++)
        {
            mvcW[i].resize(cage.num_vertices());
        }
    }

    //Current vertex of the object mesh
    vec3d v = vec3d();
    //Vertices of the current face of the cage
    vec3d p1=vec3d(), p2=vec3d(), p3=vec3d();
    vec3d u1=vec3d(), u2=vec3d(), u3=vec3d();
    double x, y, z;
    int v1_id, v2_id, v3_id;

    //Foreach point v of the original mesh
    for(int i=0; i<mesh->num_vertices(); i++)
    {
        int vid = i*3;

        x = objCoords[vid+0];
        y = objCoords[vid+1];
        z = objCoords[vid+2];

        v.set(x,y,z);

        //Foreach cage face j with vertices v1 v2 v3
        for(unsigned int j=0; j<cageFaces.size(); j=j+3) //The index for the next face is j = j+3
        {

            //Set vertex coordinates for vertices p1, p2, p3


            //Get id for vertices p1,p2,p3
            v1_id = 3*cageFaces[j];
            v2_id = 3*cageFaces[j+1];
            v3_id = 3*cageFaces[j+2];


            //Coords of p1
            x = cageCoords[v1_id];
            y = cageCoords[v1_id+1];
            z = cageCoords[v1_id+2];
            p1.set(x,y,z);

            //Coords of p2
            x = cageCoords[v2_id];
            y = cageCoords[v2_id+1];
            z = cageCoords[v2_id+2];
            p2.set(x,y,z);

            //Coords of p3
            x = cageCoords[v3_id];
            y = cageCoords[v3_id+1];
            z = cageCoords[v3_id+2];
            p3.set(x,y,z);


            /*
             * Index for vertices p1,p2,p3 that lies in the current face,
             * in which mvc weight function(mvcW) is saved
             */
            int j1 = cageFaces[j+0];
            int j2 = cageFaces[j+1];
            int j3 = cageFaces[j+2];


            double d1,d2,d3;

            d1 = (p1-v).length();
            d2 = (p2-v).length();
            d3 = (p3-v).length();

            if(d1<EPS)
            {
                return; ///booooooooooooooooooooooooooooooooo  return fi?
            }
            if(d2<EPS)
            {
                return; ///booooooooooooooooooooooooooooooooo  return fi?
            }
            if(d3<EPS)
            {
                return; ///booooooooooooooooooooooooooooooooo  return fi?
            }

            u1 = (p1-v)/d1;
            u2 = (p2-v)/d2;
            u3 = (p3-v)/d3;


            double l1, l2, l3, theta1, theta2, theta3, h, c1, c2, c3, s1, s2, s3;
            l1 = (u2-u3).length();
            l2 = (u3-u1).length();
            l3 = (u1-u2).length();

            //Compute thetai = 2*arcsin(li/2)
            theta1 = 2.0*asin(l1/2.0);
            theta2 = 2.0*asin(l2/2.0);
            theta3 = 2.0*asin(l3/2.0);

            //Compute h=sum(thetai)/2
            h = (theta1+theta2+theta3)/2.0;
            if(M_PI-h < EPS)
            {
                //Point v lies on t, use 2D barycentric coordinates
                mvcW[i][j1] = sin(theta1)*d3*d2;
                mvcW[i][j2] = sin(theta2)*d1*d3;
                mvcW[i][j3] = sin(theta3)*d2*d1;
            }
            c1 = (2*sin(h)*sin(h-theta1))/(sin(theta2)*sin(theta3))-1;
            c2 = (2*sin(h)*sin(h-theta2))/(sin(theta3)*sin(theta1))-1;
            c3 = (2*sin(h)*sin(h-theta3))/(sin(theta1)*sin(theta2))-1;

            double det=0.0;
            det = u1.x()*(u2.y()*u3.z()-u2.z()*u3.y())-u1.y()*(u2.x()*u3.z()-u2.z()*u3.x())+u1.z()*(u2.x()*u3.y()-u2.y()*u3.x());
            s1 = sign(det)*sqrt(1-c1*c1);
            s2 = sign(det)*sqrt(1-c2*c2);
            s3 = sign(det)*sqrt(1-c3*c3);

            if(s1<=EPS || s2<=EPS || s3<=EPS)
            {
                continue;
            }

            //Update weights
            mvcW[i][j1] = mvcW[i][j1] + (theta1 -c2*theta3-c3*theta2)/(d1*sin(theta2)*s3);
            mvcW[i][j2] = mvcW[i][j2] + (theta2 -c3*theta1-c1*theta3)/(d2*sin(theta3)*s1);
            mvcW[i][j3] = mvcW[i][j3] + (theta3 -c1*theta2-c2*theta1)/(d3*sin(theta1)*s2);

        }

    }
}

//Compute mesh deformation using deformed cage
void MeanValue::deform(Cage deformedCage){

    //Cage and mesh coordinates
    vector<double> cageCoords = deformedCage.vector_coords();
    vector<double> objCoords = mesh->vector_coords();

    //Coordinates of mesh deformed
    vector<double> newCoords;
    newCoords.resize(objCoords.size());

    //Current vertex of the cage
    vec3d p=vec3d();
    double x, y, z;
    int vc_id=-1;
    //Affine sum of mvc weight functions
    double totalW=0.0;
    vec3d totalF=vec3d();

    //Foreach point v of the original mesh
    for(int i=0; i<mesh->num_vertices(); i++)
    {
        //Index of vertex of the mesh to deform
        int vid = i*3;

        for(unsigned int j=0; j<mvcW[i].size(); j++)
        {
            /*
             * Get index of current cage vertex (vc_id),
             * while j is the index for mvc weight function for the current cage vertex (vc_id)
             **/
            vc_id = 3*j;

            //Coords of p, current cage vertex
            x = cageCoords[vc_id];
            y = cageCoords[vc_id+1];
            z = cageCoords[vc_id+2];
            p.set(x,y,z);

            totalF = totalF +(mvcW[i][j]*p);
            totalW = totalW + mvcW[i][j];

        }

        //Save new coordinates (mesh deformed)
        newCoords[vid+0] = totalF.x()/totalW;
        newCoords[vid+1] = totalF.y()/totalW;
        newCoords[vid+2] = totalF.z()/totalW;

        //Reset values for next mesh vertex
        totalF.set(0,0,0);
        totalW = 0.0;

    }
    //Set new coordinates of mesh deformed
    mesh->setCoords(newCoords);
}

//Return Mean Value weight functions
vector<vector<double> > MeanValue::getCoords(){
    return mvcW;
}

vector<double> MeanValue::getDeformedMesh()
{
    return mesh->vector_coords();
}
