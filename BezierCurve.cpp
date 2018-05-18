//
// Created by langetor on 09.04.18.
//

#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>
#include <vector>
#include <GL/gl.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include "BezierCurve.h"
#include "Line.h"
#include "AxisAlignedBoundingBox.h"

using namespace std;
using namespace glm;

typedef vector<glm::vec3> PointList ;

int BezierCurve::offsetCounter = 0;



void BezierCurve::draw(){

    drawPointList(controlPoints,controlColor);
    drawPointList(bezierPoints,bezierColor,2.0f);

    //drawSinglePoints(bezierPoints,bezierPointColor,4.0f);

    drawSinglePoints(controlPoints, pointColor,8.0f);

}

void BezierCurve::update(){
    bezierPoints.clear();
    bezierPoints = plot_bezier(controlPoints);

    draw();
}

void BezierCurve::drawIntersectionPoints(){
    if(intersectionPoints.size()>0){
        glEnable(GL_POINT_SMOOTH);
        drawSinglePoints(intersectionPoints,intersectionColor,15.0f);
        glDisable(GL_POINT_SMOOTH);
    }
};
void BezierCurve::drawSingleBezierPoints(){
    drawSinglePoints(bezierPoints,bezierPointColor, 3.0f);
}

void BezierCurve::select(){
    drawSinglePoints(controlPoints,pointColor,8.0f,GL_SELECT);
}

BezierCurve::BezierCurve(PointList controlPoints): offset(offsetCounter) {
    this->offsetCounter+=controlPoints.size();
    this->controlPoints = std::move(controlPoints);
    update();
}

PointList BezierCurve::reduce(PointList input){
    PointList result;
    for( int i=0; i< input.size()-1;i++){
        result.push_back((input[i]+input[i+1])*0.5f);
    }
    return result;
}

pair<PointList, PointList> BezierCurve::deCasteljau(PointList inputList){
    int n = inputList.size();
    PointList first(n);
    PointList second(n);
    vector<PointList> resultMap;
    resultMap.push_back(inputList);

    for(int i = 1; i<n;i++){
        resultMap.push_back(reduce(resultMap[i-1]));
    }
    for(int i = 0; i<resultMap.size();i++){
        first[i] = resultMap[i].front();
        second[n-1-i]=resultMap[i].back();
    }
    return pair<PointList,PointList> (first,second);
}
void BezierCurve::drawPointList(PointList input, glm::vec3 color, float pointSize) {
    glColor3fv(glm::value_ptr(color));
    glLineWidth(pointSize);
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i< input.size(); i++){
        glVertex3f((GLfloat)input[i].x,(GLfloat)input[i].y,(GLfloat)input[i].z);
    }
    glEnd();
}

void BezierCurve::drawSinglePoints(PointList input, glm::vec3 color, float pointSize, GLenum mode) {
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

PointList BezierCurve::plot_bezier(PointList input){
    if(flatness(input)){
        return input;
    }
    else{
        pair<PointList, PointList> deCasteljauPair = deCasteljau(input);
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

bool BezierCurve::flatness(PointList input, float eps){
    for(int i =1; i<input.size()-1;i++){
        if ( Line(input[i+1]-input[i], input[i]-input[i-1]).magnitude > eps){
            return false;
        }
    }
    return true;
}

void BezierCurve::intersectWithBezierCurves(vector<BezierCurve> &bezierList) {
    intersectionPoints.clear();
    for (auto&& other : bezierList){
        if(this->offset != other.offset){
            auto tmp = computeIntersectionPoints(controlPoints, other.controlPoints);
            intersectionPoints.insert(intersectionPoints.end(),tmp.begin(),tmp.end());
        }
        else{

            auto listOfLists = computeSelfIntersectionLists(controlPoints);
            PointList finalPoints;
            for(int i = 0; i<listOfLists.size();i++){
                PointList a = listOfLists[i];
                for(int j = i+1; j<listOfLists.size();j++){
                    auto tmpPoints = computeIntersectionPoints(a,listOfLists[j]);
                    intersectionPoints.insert(intersectionPoints.end(),tmpPoints.begin(),tmpPoints.end());
                }
            }

            // intersectionPoints.insert(intersectionPoints.end(),finalPoints.begin(),finalPoints.end());
        }


    }
}

bool over180Degrees(PointList input) {
    auto currentAngle = 0.0;
        for(int i=0; i<input.size()-1;i++) {
            auto a = Line(input[i], input[i + 1]);
            auto b = Line(input[i + 1], input[i + 2]);
            currentAngle += Line::getAngle(a, b);

            if (currentAngle > M_PI || currentAngle < -M_PI) {
                return true;
            }
        }

    return false;
}

vector<PointList> BezierCurve::computeSelfIntersectionLists(PointList input){
    vector<PointList> result;
    if(over180Degrees(input)){
        pair<PointList, PointList> deCasteljauPair = deCasteljau(input);
        vector<PointList> i1 = computeSelfIntersectionLists(deCasteljauPair.first);
        vector<PointList> i2 = computeSelfIntersectionLists(deCasteljauPair.second);

        result.insert(result.end(),i1.begin(),i1.end());
        result.insert(result.end(),i2.begin(),i2.end());

        return result;

    }
    else{
        result.push_back(input);
        return result;
    }

}

PointList BezierCurve::computeIntersectionPoints(PointList a, PointList b){

    AxisAlignedBoundingBox boxA = AxisAlignedBoundingBox::createBox(a);
    AxisAlignedBoundingBox boxB = AxisAlignedBoundingBox::createBox(b);
    PointList result{};

    if(boxA.intersect(boxB)){

        if(!flatness(a)){
            //cut a in half
            pair<PointList, PointList> cutA = deCasteljau(a);

            PointList a1 = computeIntersectionPoints(cutA.first,b);
            PointList a2 = computeIntersectionPoints(cutA.second,b);


            result.insert(result.end(),a1.begin(),a1.end());
            result.insert(result.end(),a2.begin(),a2.end());
            return result;


        }
        else if(!flatness(b)){
            //cut b in half

            pair<PointList, PointList> cutB = deCasteljau(b);

            PointList b1 = computeIntersectionPoints(a,cutB.first);
            PointList b2 = computeIntersectionPoints(a,cutB.second);


            result.insert(result.end(),b1.begin(),b1.end());
            result.insert(result.end(),b2.begin(),b2.end());
            return result;

        }
        else{

            auto point = Line::intersectLines( Line(a.front(),a.back()), Line(b.front(),b.back()));
            if(point)
                result.push_back(*point);
            return result;

        }


    }
    else{
        return result;
    }

}