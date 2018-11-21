/**
 @author    Marco Livesu (marco.livesu@gmail.com)
 @copyright Marco Livesu 2014.
*/

#ifndef TRIMESH_H
#define TRIMESH_H

//// V0
//coords[0]  = 0.1;
//coords[1]  = 0.2;
//coords[2]  = 0.3;
//// V1
//coords[3]  = 0.1;
//coords[4]  = 0.45876;
//coords[5]  = 0.253;
//// V2
//coords[6]  = 0.1;
//coords[7]  = 0.45876;
//coords[8]  = 0.253;
//// V3
//coords[9]  = 0.1;
//coords[10]  = 0.45876;
//coords[11]  = 0.253;
//// V4
//coords[12]  = 0.1;
//coords[13]  = 0.45876;
//coords[14]  = 0.253;

//vid = 12;
//coords[12*3+0]; // X di V12
//coords[12*3+1]; // Y di V12
//coords[12*3+2]; // Z di V12

//// T0
//tris[0] = 0;
//tris[1] = 2;
//tris[2] = 3;
//// T1
//tris[3] = 1;
//tris[4] = 4;
//tris[5] = 5;



#include <assert.h>
#include <float.h>
#include <vector>
#include <map>
#include <set>
#include <stdlib.h>

#include "bbox.h"
#include "vec3.h"
#include "utils.h"
#include "load_save_trimesh.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>

using namespace std;

template<typename C>
inline void CHECK_BOUNDS(const C & container, int index)
{
    assert(index < (int)container.size());
}

class Trimesh
{

    public:

        Trimesh(){}

        Trimesh(std::string filename)
        {
            load(filename.c_str());
            init();
        }

        Trimesh(const vector<double> & coords,
                const vector<int>  & tris)
        {
            this->coords = coords;
            this->tris   = tris;
            init();
        }

    protected:

        Bbox                  bbox;
        vector<double>          coords;
        vector<int>           tris;
        vector<double>          v_norm;
        vector<double>          t_norm;
        vector< vector<int> > vtx2tri;
        vector< vector<int> > vtx2vtx;
        vector< vector<int> > tri2tri;

        vector<double> to_double_array (const vector<double> & in) const
        {
            vector<double> res(in.size());
            for(int i=0; i<(int)in.size(); i++)
            {
                res[i] = in[i];
            }
            return res;
        }

        vector<double> from_double_array(const vector<double> & in) const
        {
            vector<double> res(in.size());
            for( int i=0; i<in.size(); i++ )
            {
                res[i] = in[i];
            }
            return res;
        }

        void clear()
        {
            coords.clear();
            tris.clear();

            vtx2vtx.clear();
            vtx2tri.clear();
            tri2tri.clear();
        }

        void init()
        {
            build_adjacency();
            update_normals();
            update_bbox();
        }

        void build_adjacency()
        {
            vtx2vtx.clear();
            vtx2tri.clear();
            tri2tri.clear();

            vtx2vtx.resize(num_vertices());
            vtx2tri.resize(num_vertices());
            tri2tri.resize(num_triangles());

            set<edge>     edges;
            map<edge,int> edge2tri;

            for(int tid=0; tid<num_triangles(); ++tid)
            {
                int tid_ptr = tid * 3;
                for(int i=0; i<3; ++i)
                {
                    int vid = tris[tid_ptr + i];
                    vtx2tri[vid].push_back(tid);

                    int adj = tris[tid_ptr + (i+1)%3];
                    edge e = unique_edge(vid,adj);
                    edges.insert(e);

                    map<edge,int>::iterator query = edge2tri.find(e);
                    if (query == edge2tri.end())
                    {
                        edge2tri[e] = tid;
                    }
                    else
                    {
                        int nbr_tri = query->second;
                        tri2tri[tid].push_back(nbr_tri);
                        tri2tri[nbr_tri].push_back(tid);
                    }
                }
            }

            for(set<edge>::iterator it=edges.begin(); it!=edges.end(); ++it)
            {
                edge e = *it;
                vtx2vtx[e.first].push_back(e.second);
                vtx2vtx[e.second].push_back(e.first);
            }
        }

        void update_t_normals()
        {
            t_norm.clear();
            t_norm.resize(num_triangles()*3);

            for(int tid=0; tid<num_triangles(); ++tid)
            {
                int tid_ptr = tid * 3;

                vec3d v0 = vertex(tris[tid_ptr+0]);
                vec3d v1 = vertex(tris[tid_ptr+1]);
                vec3d v2 = vertex(tris[tid_ptr+2]);

                vec3d u = v1 - v0;    u.normalize();
                vec3d v = v2 - v0;    v.normalize();
                vec3d n = u.cross(v); n.normalize();

                t_norm[tid_ptr + 0] = n.x();
                t_norm[tid_ptr + 1] = n.y();
                t_norm[tid_ptr + 2] = n.z();
            }
        }

        void update_v_normals()
        {
            v_norm.clear();
            v_norm.resize(num_vertices()*3);

            for(int vid=0; vid<num_vertices(); ++vid)
            {
                vector<int> nbrs = adj_vtx2tri(vid);

                vec3d sum(0,0,0);
                for(int i=0; i<(int)nbrs.size(); ++i)
                {
                    sum += triangle_normal(nbrs[i]);
                }

                assert(nbrs.size() > 0);
                sum /= nbrs.size();
                sum.normalize();

                int vid_ptr = vid * 3;
                v_norm[vid_ptr + 0] = sum.x();
                v_norm[vid_ptr + 1] = sum.y();
                v_norm[vid_ptr + 2] = sum.z();
            }
        }

        void load(const char * filename)
        {
            clear();
            vector<double> tmp_coords;

            string str(filename);
            string filetype = str.substr(str.size()-3,3);

            if (filetype.compare("obj") == 0 ||
                filetype.compare("OBJ") == 0)
            {
                load_OBJ(filename, tmp_coords, tris);
            }
            else
            {
                cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : load() : file format not supported yet " << endl;
                exit(-1);
            }

            coords = to_double_array(tmp_coords);

            cout << tris.size() / 3   << " triangles read" << endl;
            cout << coords.size() / 3 << " vertices  read" << endl;

            build_adjacency();
            update_normals();
            update_bbox();
        }

        void load_OBJ(const char     * filename,
                      vector<double> & xyz,
                      vector<int>    & tri)
        {
            ifstream file(filename);

            if (!file.is_open())
            {
                //cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : load_OBJ() : couldn't open input file " << filename << endl;
                exit(-29);
            }

            string line;
            while (getline(file, line))
            {
                istringstream iss(line);

                string token;
                iss >> token;
                if (token.size() > 1) continue; // vn,fn  .... I don't care

                if (token[0] == 'v')
                {
                    double x, y, z;
                    iss >> x >> y >> z;
                    xyz.push_back(x);
                    xyz.push_back(y);
                    xyz.push_back(z);
                    //cout << "v " << x << " " << y << " " << z << endl;
                }
                else if (token[0] == 'f')
                {
                    int v0, v1, v2;
                    iss >> v0 >> v1 >> v2;
                    tri.push_back(v0-1);
                    tri.push_back(v1-1);
                    tri.push_back(v2-1);
                    //cout << "f " << v0 << " " << v1 << " " << v2 << endl;
                }
            }
            file.close();
        }

    public:

        inline const vector<double> & vector_coords()    const { return coords; }
        inline const vector<int>  & vector_triangles() const { return tris;   }

        inline int num_vertices()  const { return coords.size()/3; }
        inline int num_triangles() const { return tris.size()/3;   }

        inline vector<int> adj_vtx2tri(int vid) const { CHECK_BOUNDS(vtx2tri, vid); return vtx2tri[vid]; }
        inline vector<int> adj_vtx2vtx(int vid) const { CHECK_BOUNDS(vtx2vtx, vid); return vtx2vtx[vid]; }
        inline vector<int> adj_tri2tri(int tid) const { CHECK_BOUNDS(tri2tri, tid); return tri2tri[tid]; }

        inline vec3d triangle_normal(int tid) const
        {
            int tid_ptr = tid * 3;
            CHECK_BOUNDS(t_norm, tid_ptr+2);
            return vec3d     (t_norm[tid_ptr + 0],
                              t_norm[tid_ptr + 1],
                              t_norm[tid_ptr + 2]);
        }

        inline vec3d vertex_normal(int vid) const
        {
            int vid_ptr = vid * 3;
            CHECK_BOUNDS(v_norm, vid_ptr+2);
            return vec3d(v_norm[vid_ptr + 0],
                              v_norm[vid_ptr + 1],
                              v_norm[vid_ptr + 2]);
        }

        inline vec3d vertex(int vid) const
        {
            int vid_ptr = vid * 3;
            CHECK_BOUNDS(coords, vid_ptr+2);
            return vec3d(coords[vid_ptr + 0],
                              coords[vid_ptr + 1],
                              coords[vid_ptr + 2]);
        }

        inline void set_vertex(int vid, vec3d pos)
        {
            int vid_ptr = vid * 3;
            CHECK_BOUNDS(coords, vid_ptr+2);
            coords[vid_ptr + 0] = pos.x();
            coords[vid_ptr + 1] = pos.y();
            coords[vid_ptr + 2] = pos.z();
        }

        void update_normals()
        {
            update_t_normals();
            update_v_normals();
        }

        void update_bbox()
        {
            bbox.reset();
            for(int vid=0; vid<num_vertices(); ++vid)
            {
                vec3d v = vertex(vid);
                bbox.min = bbox.min.min(v);
                bbox.max = bbox.max.max(v);
            }
        }

        inline void setCoords(const vector<double> newCoords){
            coords = newCoords;
        }

        inline vec3d faceCenter(int tid){
            int tid_ptr = tid * 3;
            CHECK_BOUNDS(t_norm, tid_ptr+2);
            //Retrieve the pointer to the coordinates of the vertices of the face (tid)
            int v1_id = tris[tid_ptr+0];
            int v2_id = tris[tid_ptr+1];
            int v3_id = tris[tid_ptr+2];

            //Retrieve coordinates for v1,v2,v3
            double x1 = coords[3*v1_id];
            double x2 = coords[3*v2_id];
            double x3 = coords[3*v3_id];

            double y1 = coords[3*v1_id+1];
            double y2 = coords[3*v2_id+1];
            double y3 = coords[3*v3_id+1];

            double z1 = coords[3*v1_id+2];
            double z2 = coords[3*v2_id+2];
            double z3 = coords[3*v3_id+2];


            //Center of the face tid
            vec3d center;
            center.set( ((x1+x2+x3)/3), ((y1+y2+y3)/3), ((z1+z2+z3)/3) );

            //Return center of face tid
            return center;
        }

        void save(const char * filename){
            save_OBJ(filename,coords,tris);
        }
};

#endif // TRIMESH_H
