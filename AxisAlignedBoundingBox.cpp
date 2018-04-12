//
// Created by TorbenLange on 12.04.2018.
//

#include "AxisAlignedBoundingBox.h"

bool AxisAlignedBoundingBox::intersect(AxisAlignedBoundingBox &other) {
    auto&& omin=other.min;
    auto&& omax=other.max;

    bool x = ( (min.x<=omin.x && omin.x<=max.x) || (omin.x <= min.x && min.x<=omax.x) );
    bool y = ( (min.y<=omin.y && omin.y<=max.y) || (omin.y <= min.y && min.y<=omax.y) );
    bool z = ( (min.z<=omin.z && omin.z<=max.z) || (omin.z <= min.z && min.z<=omax.z) );

    return x && y && z;
}

AxisAlignedBoundingBox AxisAlignedBoundingBox::createBox(PointList input) {
    vec3 min, max;

    min=max=input[0];

    for(int i=1; i<input.size(); i++){
        if(input[i].x<min.x) min.x=input[i].x;
        if(input[i].y<min.y) min.y=input[i].y;
        if(input[i].z<min.z) min.z=input[i].z;

        if(input[i].x>max.x) max.x=input[i].x;
        if(input[i].y>max.y) max.y=input[i].y;
        if(input[i].z>max.z) max.z=input[i].z;
    }

    return AxisAlignedBoundingBox(min,max);
}
