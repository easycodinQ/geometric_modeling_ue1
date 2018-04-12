#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <assert.h>
#include <glm/glm.hpp>
#include <vector>
#include "BezierCurve.h"
#include "AxisAlignedBoundingBox.h"

#if !defined(BUFSIZE)
#define BUFSIZE 512
#endif

using namespace std;
typedef vector<glm::vec3> pointList ;

int num_points=4;
glm::vec3* points = new glm::vec3[num_points];
int picked_pos=-1;

vector<BezierCurve> bezierCurveList;

void intersect(pointList c1, pointList c2){
    AxisAlignedBoundingBox box1 = AxisAlignedBoundingBox::createBox(c1);
    AxisAlignedBoundingBox box2 = AxisAlignedBoundingBox::createBox(c2);

    if(box1.intersect(box2)){

    }

}

void drawAllCurves(){
    for(auto&& curve : bezierCurveList){
        curve.draw();
    }
}
void selectCurves(){
    for(auto&& curve : bezierCurveList){
        curve.select();
    }
    glPopName();
}


int processHits (GLint hits, GLuint buffer[])
{
   unsigned int i, j;
   GLuint ii, jj, names, *ptr;
   int result=-1;  

   ptr = (GLuint *) buffer;
   for (i = 0; i < hits; i++) { /*  for each hit  */
      names = *ptr;

      ptr++;
      ptr++; 
      ptr++;
      
      for (j = 0; j < names; j++) { /*  for each name */
	  printf ("%d ", *ptr);
      
	 result=(int)*ptr;
	 
         ptr++;
      }
      printf ("\n");
      
   }
   return result;
}




int pickPoints(int x, int y)
{
	GLuint selectBuf[BUFSIZE];
	GLint hits;
	GLint viewport[4];

	glGetIntegerv (GL_VIEWPORT, viewport);
	glSelectBuffer (BUFSIZE, selectBuf);
	(void) glRenderMode(GL_SELECT);
        glInitNames();
        glPushName(0);

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
      
	gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3] - y),8.0, 8.0, viewport);    	 
	gluPerspective(60.0,(GLfloat) viewport[2] / (GLfloat) viewport[3], 1.0,20.0);
	selectCurves();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();	
	    
	hits = glRenderMode (GL_RENDER);
	int result = processHits (hits, selectBuf);
	
	return result;
}

void mousePress(int button, int state, int x, int y)
{
    if((button == GLUT_LEFT_BUTTON)&&(state == GLUT_DOWN)) // && (glutGetModifiers()==GLUT_ACTIVE_CTRL))
       picked_pos=pickPoints(x,y);

    if((button == GLUT_LEFT_BUTTON)&&(state == GLUT_UP))
       picked_pos=-1;


    glutPostRedisplay();
}

void mouseMove(int x, int y)
{
     GLint viewport[4];
     glGetIntegerv(GL_VIEWPORT, viewport);
     GLfloat w=(GLfloat)viewport[2];	
     GLfloat h=(GLfloat)viewport[3];	

     GLint new_pos_x=x;
     GLint new_pos_y=viewport[3]-y;	

     GLdouble cpm[16];
     glGetDoublev(GL_PROJECTION_MATRIX, cpm);
	
     GLdouble cmvm[16];
     glGetDoublev(GL_MODELVIEW_MATRIX, cmvm);
	
     GLdouble objx, objy, objz;
     GLfloat z;	

     glReadPixels((GLdouble)new_pos_x,(GLdouble)new_pos_y,1, 1, GL_DEPTH_COMPONENT, GL_FLOAT,&z);
     gluUnProject((GLdouble)new_pos_x,(GLdouble)new_pos_y,z,cmvm,cpm,viewport,&objx,&objy,&objz);
        
     if(picked_pos>=0)
         for(auto&& curve: bezierCurveList){
            if(picked_pos>= curve.offset && picked_pos< curve.offsetEnd()){
                curve.updatePoint(glm::vec3((double)objx,(double)objy,round((double)objz)),picked_pos-curve.offset);
            }
     }


    

     glutPostRedisplay();
}


void display(void)
{
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);
    drawAllCurves();
    glutSwapBuffers();
}

void init(void) 
{
    glClearColor(0.0,0.0,0.0,0.0);
    glShadeModel(GL_FLAT);
    pointList c1;
    c1.push_back(glm::vec3(-5.0,0.0,-15.0));
    c1.push_back(glm::vec3(-5.0,5.0,-15.0));
    c1.push_back(glm::vec3(3.0,3.0,-15.0));
    c1.push_back(glm::vec3(3.0,0.0,-15.0));

    bezierCurveList.emplace_back(c1);

    pointList c2;
    c2.push_back(glm::vec3(5.0,0.0,-15.0));
    c2.push_back(glm::vec3(5.0,-5.0,-15.0));
    c2.push_back(glm::vec3(-3.0,-3.0,-15.0));
    c2.push_back(glm::vec3(-3.0,0.0,-15.0));

    bezierCurveList.emplace_back(c2);

	 
}

void reshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,(GLfloat) w / (GLfloat) h, 1.0,20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef (0.0, 0.0, -5.0);
}


void keyboard(unsigned char key, int x, int y)
{
    switch(key) {

	case 's':   
           // do something
            glutPostRedisplay();
	    break;
	case 'i':
		// do something
           glutPostRedisplay();
	    break;
	case 'c':   
         // do something
            glutPostRedisplay();
	    break;
	case 'y':
	    // do something
            glutPostRedisplay();
	    break;
	case 'z':
	    // do something
            glutPostRedisplay();
	    break;

	}
	
}

int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowPosition( 0, 0 );
    glutInitWindowSize( 800, 600);
    glutCreateWindow( "GM Uebung SoSe 2018" );

    init();

    glutMouseFunc(mousePress);
    glutMotionFunc(mouseMove);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
	
    glutMainLoop();
    return 0;		
}
