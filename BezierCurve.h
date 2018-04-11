//
// Created by langetor on 09.04.18.
//

#ifndef GM_EXERCISE_BEZIERCURVE_H
#define GM_EXERCISE_BEZIERCURVE_H

#include <glm/vec3.hpp>
#include <vector>

using namespace std;
typedef vector<glm::vec3> pointList ;

class BezierCurve {
    pointList controlPoints;
    pointList bezierPoints;
    glm::vec3 pointColor = glm::vec3(1.0,1.0,1.0);
    glm::vec3 bezierColor = glm::vec3(0.0,1.0,0.0);
    glm::vec3 curveColor = glm::vec3(1.0,0.0,1.0);


public:

    BezierCurve(pointList controlPoints);
    void draw();

private:
    glm::vec3 halbwert(glm::vec3 a, glm::vec3 b);
    pointList reduce(pointList input);
    pair<pointList, pointList> deCasteljau(pointList inputList);
    void drawPointList(pointList input, glm::vec3 color);
    void plot_bezier(pointList input, int k);
    void drawSinglePoints(pointList input, glm::vec3 color);

};


#endif //GM_EXERCISE_BEZIERCURVE_H
