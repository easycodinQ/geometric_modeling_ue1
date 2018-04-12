//
// Created by TorbenLange on 12.04.2018.
//

#ifndef GM_EXERCISE_AXISALIGNEDBOUNDINGBOX_H
#define GM_EXERCISE_AXISALIGNEDBOUNDINGBOX_H

#include <glm/vec3.hpp>
#include <vector>
#include "BezierCurve.h"

using namespace std;
using namespace glm;

class AxisAlignedBoundingBox {
    vec3 min;
    vec3 max;



public:
    AxisAlignedBoundingBox(vec3 min, vec3 max): min(min),max(max){};
    bool intersect(AxisAlignedBoundingBox &other);
    static AxisAlignedBoundingBox createBox(pointList input);
};


#endif //GM_EXERCISE_AXISALIGNEDBOUNDINGBOX_H
