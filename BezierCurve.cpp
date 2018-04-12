//
// Created by langetor on 09.04.18.
//

#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>
#include <vector>
#include <GL/gl.h>
#include <iostream>
#include "BezierCurve.h"
#include "Line.h"

using namespace std;
using namespace glm;

typedef vector<glm::vec3> pointList ;

int BezierCurve::offsetCounter = 0;



void BezierCurve::draw(){
   drawPointList(bezierPoints,curveColor);
    drawPointList(controlPoints,bezierColor);
    drawSinglePoints(bezierPoints,bezierPointColor,4.0f);
    drawSinglePoints(controlPoints, pointColor,8.0f);
}

void BezierCurve::update(){
    bezierPoints.clear();
    bezierPoints = plot_bezier(controlPoints);
    draw();
}

void BezierCurve::select(){
    drawSinglePoints(controlPoints,pointColor,8.0f,GL_SELECT);
}

BezierCurve::BezierCurve(pointList controlPoints): offset(offsetCounter) {
    this->offsetCounter+=controlPoints.size();
    this->controlPoints = std::move(controlPoints);
    update();
}

vec3 BezierCurve::halbwert(vec3 a, vec3 b){
    vec3 result;
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

void BezierCurve::drawSinglePoints(pointList input, glm::vec3 color, float pointSize, GLenum mode) {
    glPointSize(pointSize);
    glColor3fv(glm::value_ptr(color));

    for(int i = 0; i< input.size(); i++){
        if(mode==GL_SELECT)
            glLoadName(i+offset);
        glBegin(GL_POINTS);
        glVertex3f((GLfloat)input[i].x,(GLfloat)input[i].y,(GLfloat)input[i].z);
        glEnd();
    }
}

pointList BezierCurve::plot_bezier(pointList input){
    if(flatness(input)){
        return input;
    }
    else{
        pair<pointList, pointList> deCasteljauPair = deCasteljau(input);
        deCasteljauPair.first = plot_bezier(deCasteljauPair.first);
        deCasteljauPair.second = plot_bezier(deCasteljauPair.second);
        deCasteljauPair.first.insert(
                deCasteljauPair.first.end(),
                deCasteljauPair.second.begin(),
                deCasteljauPair.second.end()
                );
        return deCasteljauPair.first;
    }
}


int BezierCurve::offsetEnd() {
    return offset+controlPoints.size();
}

void BezierCurve::updatePoint(glm::vec3 point, int n) {
    controlPoints[n] = point;
    update();
}

bool BezierCurve::flatness(pointList input, float eps){
    for(int i =1; i<input.size()-1;i++){
        if ( Line(input[i+1]-input[i], input[i]-input[i-1]).magnitude > eps){
            return false;
        }
    }
    return true;
}


