/**
  @author    Marco Livesu (marco.livesu@gmail.com)
 @copyright Marco Livesu 2014.
*/

#ifndef BBOX_H
#define BBOX_H

#include <float.h>
#include "vec3.h"

class Bbox
{
    public:

        Bbox() { reset(); }

        void reset()
        {
            min = vec3d( FLT_MAX,  FLT_MAX,  FLT_MAX);
            max = vec3d(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        }

        vec3d  min, max;
        vec3d  center() const { return (min + max) * 0.5;    }
        double diag()   const { return (min - max).length(); }
};

#endif // BBOX_H
