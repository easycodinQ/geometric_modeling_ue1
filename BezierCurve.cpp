//
// Created by langetor on 09.04.18.
//

#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility> #include <vector>
#include <GL/gl.h>
#include <iostream>
#include "BezierCurve.h"

using namespace std;

typedef vector<glm::vec3> pointList ;

int BezierCurve::offsetCounter = 0;

void BezierCurve::draw(){
    plot_bezier(controlPoints,5);
    drawPointList(controlPoints,bezierColor);
    drawSinglePoints(controlPoints, pointColor);
}

void BezierCurve::select(){
    drawSinglePoints(controlPoints,pointColor,GL_SELECT);
}

BezierCurve::BezierCurve(pointList controlPoints): offset(offsetCounter) {
    this->offsetCounter+=controlPoints.size();
    this->controlPoints = std::move(controlPoints);
}

glm::vec3 BezierCurve::halbwert(glm::vec3 a, glm::vec3 b){
    glm::vec3 result;
    result.x = (a.x + b.x) / 2;
    result.y = (a.y + b.y) / 2;
    result.z = (a.z + b.z) / 2;

    return result;
}

pointList BezierCurve::reduce(pointList input){
    pointList result;

    for( int i=0; i< input.size()-1;i++){
        result.push_back(halbwert(input[i],input[i+1]));
    }

    return result;
}

pair<pointList, pointList> BezierCurve::deCasteljau(pointList inputList){
    int n = inputList.size();
    pointList first(n);
    pointList second(n);
    vector<pointList> resultMap;

    resultMap.push_back(inputList);

    for(int i = 1; i<n;i++){
        resultMap.push_back(reduce(resultMap[i-1]));
    }

    for(int i = 0; i<resultMap.size();i++){
        first[i] = resultMap[i].front();
        second[n-1-i]=resultMap[i].back();
    }

    return pair<pointList,pointList> (first,second);
}
void BezierCurve::drawPointList(pointList input, glm::vec3 color) {
    glColor3fv(glm::value_ptr(color));
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i< input.size(); i++){
        glVertex3f((GLfloat)input[i].x,(GLfloat)input[i].y,(GLfloat)input[i].z);
    }
    glEnd();

}

void BezierCurve::drawSinglePoints(pointList input, glm::vec3 color, GLenum mode) {
    glPointSize(8.0);
    glColor3fv(glm::value_ptr(color));

    for(int i = 0; i< input.size(); i++){
        if(mode==GL_SELECT)
            glLoadName(i+offset);
        glBegin(GL_POINTS);
        glVertex3f((GLfloat)input[i].x,(GLfloat)input[i].y,(GLfloat)input[i].z);
        glEnd();
    }
}

void BezierCurve::plot_bezier(pointList input, int k){
    if(k==0){
        drawPointList(input, curveColor);
    }
    else{
        pair<pointList, pointList> deCasteljauPair = deCasteljau(input);
        plot_bezier(deCasteljauPair.first,k-1);
        plot_bezier(deCasteljauPair.second,k-1);

    }
}

int BezierCurve::offsetEnd() {
    return offset+controlPoints.size();
}

void BezierCurve::updatePoint(glm::vec3 point, int n) {
    controlPoints[n] = point;
}

