//
// Created by langetor on 18.05.18.
//

#ifndef GM_EXERCISE_BSPLINE_H
#define GM_EXERCISE_BSPLINE_H


#include "BezierCurve.h"

class BSpline : BezierCurve{
public:
    BSpline(const PointList &controlPoints);


private:
    vector<BezierCurve> bezierCurves;
    PointList plot_bezier(PointList input);
};


#endif //GM_EXERCISE_BSPLINE_H
