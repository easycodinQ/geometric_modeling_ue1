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
typedef vector<glm::vec3> pointList ;

const float EPSILON = 0.01f;

class BezierCurve {
    pointList controlPoints;

    glm::vec3 pointColor = glm::vec3(1.0,1.0,1.0);
    glm::vec3 bezierColor = glm::vec3(0.0,1.0,0.0);
    glm::vec3 bezierPointColor = glm::vec3(0.0,1.0,1.0);
    glm::vec3 curveColor = glm::vec3(1.0,0.0,1.0);


    static int offsetCounter;


public:
    pointList bezierPoints;
    int offset;
    BezierCurve(pointList controlPoints);
    void draw();
    void update();
    void select();
    void updatePoint(glm::vec3 point, int n);
    int offsetEnd();
    virtual ~BezierCurve() = default;
    bool flatness(pointList input, float eps= EPSILON);

private:
    glm::vec3 halbwert(glm::vec3 a, glm::vec3 b);
    pointList reduce(pointList input);
    pair<pointList, pointList> deCasteljau(pointList inputList);
    void drawPointList(pointList input, glm::vec3 color);
    pointList plot_bezier(pointList input);
    void drawSinglePoints(pointList input, glm::vec3 color,float pointSize, GLenum mode = GL_RENDER);



};


#endif //GM_EXERCISE_BEZIERCURVE_H
