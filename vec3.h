/**
 @author    Marco Livesu (marco.livesu@gmail.com)
 @copyright Marco Livesu 2014.
*/

#ifndef VEC3_H
#define VEC3_H

#include<cmath>
#include<iostream>

using namespace std;

template<typename real> class vec3
{
    private:

        union
        {
            struct
            {
                real m_x;
                real m_y;
                real m_z;
            };
            real m_v[3];
        };

    public:

        vec3()
        {
            set( 0.0, 0.0, 0.0 );
        }

        vec3(const real x, const real y=0.0, const real z=0.0 )
        {
            set(x, y, z);
        }

        inline const real *ptr() const
        {
            return m_v;
        }

        inline void set( const real x, const real y, const real z )
        {
            m_x = x;
            m_y = y;
            m_z = z;
        }

        inline real dot(const vec3<real> & in) const
        {
            return m_x * in.m_x +
                   m_y * in.m_y +
                   m_z * in.m_z;
        }

        inline vec3<real> cross(const vec3<real> & in) const
        {
            return vec3<real>(m_y * in.m_z - m_z * in.m_y,
                              m_z * in.m_x - m_x * in.m_z,
                              m_x * in.m_y - m_y * in.m_x);
        }

        inline real length_squared() const
        {
            return m_x * m_x +
                   m_y * m_y +
                   m_z * m_z;
        }

        inline real length() const
        {
            return sqrt(m_x * m_x +
                        m_y * m_y +
                        m_z * m_z);
        }

        inline real normalize()
        {
            real len = std::max(length(), 1e-5);
            m_x /= len;
            m_y /= len;
            m_z /= len;
            return len;
        }

        inline real &x() { return m_x; }
        inline real &y() { return m_y; }
        inline real &z() { return m_z; }

        inline const real &x() const { return m_x; }
        inline const real &y() const { return m_y; }
        inline const real &z() const { return m_z; }

        inline real &operator[](const int id)
        {
            return m_v[id];
        }

        inline const real &operator[](const int id) const
        {
            return m_v[id];
        }

        inline vec3<real> operator-() const
        {
            return vec3<real>(-m_x,
                              -m_y,
                              -m_z);
        }

        inline vec3<real> operator+(const vec3<real> b) const
        {
            return vec3<real>(m_x + b.m_x,
                              m_y + b.m_y,
                              m_z + b.m_z);
        }

        inline vec3<real> operator-(const vec3<real> b) const
        {
            return vec3<real>(m_x - b.m_x,
                              m_y - b.m_y,
                              m_z - b.m_z);
        }

        inline vec3<real> operator*(const real b) const
        {
            return vec3<real>(m_x * b,
                              m_y * b,
                              m_z * b);
        }

        inline vec3<real> operator/(const real b) const
        {
            return vec3<real>(m_x / b,
                              m_y / b,
                              m_z / b);
        }

        inline vec3<real> operator+=(const vec3<real> b)
        {
            vec3<real> tmp(m_x + b.m_x,
                           m_y + b.m_y,
                           m_z + b.m_z);
            *this=tmp;
            return tmp;
        }

        inline vec3<real> operator-=(const vec3<real> b)
        {
            vec3<real> tmp(m_x - b.m_x,
                           m_y - b.m_y,
                           m_z - b.m_z);
            *this = tmp;
            return tmp;
        }

        inline vec3<real> operator*=(const real b)
        {
            vec3<real> tmp(m_x * b,
                           m_y * b,
                           m_z * b);
            *this = tmp;
            return tmp;
        }

        inline vec3<real> operator/=(const real b)
        {
            vec3<real> tmp(m_x / b,
                           m_y / b,
                           m_z / b);
            *this = tmp;
            return tmp;
        }

        inline bool operator<(const vec3<real> in) const
        {
            for( int i=0; i<3; ++i)
            {
                if( this->operator[](i) < in[i] ) return true;
                if( this->operator[](i) > in[i] ) return false;
            }
            return false;
        }

        inline vec3<real> min(const vec3<real> &in) const
        {
            return vec3<real>(std::min(x(), in.x()),
                              std::min(y(), in.y()),
                              std::min(z(), in.z()));
        }

        inline vec3<real> max(const vec3<real> &in) const
        {
            return vec3<real>(std::max(x(), in.x()),
                              std::max(y(), in.y()),
                              std::max(z(), in.z()));
        }
};

template<typename real>
vec3<real> operator*(const double &b, const vec3<real> &a )
{
    return vec3<real>(b * a.x(),
                      b * a.y(),
                      b * a.z());
}

template<typename real>
std::ostream& operator<<(std::ostream &in, const vec3<real> &val)
{
    in << "[" << val.x() << ", " << val.y() << ", " << val.z() << "]";
    return in;
}

typedef vec3<double> vec3d;

template<typename vec3>
void make_rotation_matrix(const vec3 & axis,
						  double       angle,
						  double       m[3][3])
{
	double u    = axis.x();
	double v    = axis.y();
	double w    = axis.z();
	double rcos = cos(angle);
	double rsin = sin(angle);
	m[0][0] =      rcos + u*u*(1-rcos);
	m[1][0] =  w * rsin + v*u*(1-rcos);
	m[2][0] = -v * rsin + w*u*(1-rcos);
	m[0][1] = -w * rsin + u*v*(1-rcos);
	m[1][1] =      rcos + v*v*(1-rcos);
	m[2][1] =  u * rsin + w*v*(1-rcos);
	m[0][2] =  v * rsin + u*w*(1-rcos);
	m[1][2] = -u * rsin + v*w*(1-rcos);
	m[2][2] =      rcos + w*w*(1-rcos);
}

template<typename vec3>
void rotate(vec3 & p, double m[3][3])
{
	vec3d tmp;
	tmp[0] = m[0][0] * p.x() + m[0][1] * p.y() + m[0][2] * p.z();
	tmp[1] = m[1][0] * p.x() + m[1][1] * p.y() + m[1][2] * p.z();
	tmp[2] = m[2][0] * p.x() + m[2][1] * p.y() + m[2][2] * p.z();
	p  = tmp;
}

template<typename vec3>
void rotate(vec3 & p, double m[3][3], const vec3 & centroid)
{
	p -= centroid;
	vec3d tmp;
	tmp[0] = m[0][0] * p.x() + m[0][1] * p.y() + m[0][2] * p.z();
	tmp[1] = m[1][0] * p.x() + m[1][1] * p.y() + m[1][2] * p.z();
	tmp[2] = m[2][0] * p.x() + m[2][1] * p.y() + m[2][2] * p.z();
	p  = tmp;
	p += centroid;
}


#endif // VEC3_H
