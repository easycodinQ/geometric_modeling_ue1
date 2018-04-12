//
// Created by langetor on 09.04.18.
//

#ifndef GM_EXERCISE_BEZIERCURVE_H
#define GM_EXERCISE_BEZIERCURVE_H


#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>
#include <vector>
#include <GL/gl.h>

using namespace std;
using namespace glm;
typedef vector<glm::vec3> PointList ;

const float EPSILON = 0.001f;

class BezierCurve {
    PointList controlPoints;
    PointList intersectionPoints;

    glm::vec3 pointColor = glm::vec3(0.7,1.0,0.4);
    glm::vec3 controlColor = glm::vec3(0.0,1.0,0.0);
    glm::vec3 bezierPointColor = glm::vec3(0.0,1.0,1.0);
    glm::vec3 bezierColor = glm::vec3(1.0,0.0,1.0);
    glm::vec3 intersectionColor = glm::vec3(1.0,0.1,0.2);


    static int offsetCounter;


public:
    PointList bezierPoints;
    int offset;
    BezierCurve(PointList controlPoints);
    void draw();
    void drawIntersectionPoints();
    void drawSingleBezierPoints();
    void update();
    void select();
    void updatePoint(glm::vec3 point, int n);
    int offsetEnd();
    virtual ~BezierCurve() = default;
    bool flatness(PointList input, float eps= EPSILON);

    void intersectWithBezierCurves(vector<BezierCurve> &bezierList);

private:
    PointList reduce(PointList input);
    pair<PointList, PointList> deCasteljau(PointList inputList);
    void drawPointList(PointList input, glm::vec3 color, float pointSize=1.0f);
    PointList plot_bezier(PointList input);
    void drawSinglePoints(PointList input, glm::vec3 color,float pointSize, GLenum mode = GL_RENDER);
    pair<PointList, PointList> intersectionListCut(PointList in);
    PointList computeIntersectionPoints(PointList a, PointList b);
    PointList computeSelfIntersectionPoints(PointList a, PointList b);


};


#endif //GM_EXERCISE_BEZIERCURVE_H
