#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <GL/glut.h>
#include <GL/glui.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include "vertex.h"
#include "vertex_mod.h"

using namespace std;

enum GLUI_COMP_ID { SUBWIN_T_FACE_CB, //subwindow top checkbox
                    SUBWIN_T_SEGMENT_CB,
                    SUBWIN_T_RESET_BTN,
                    SUBWIN_R_BLENDED_BTN,
                    SUBWIN_R_RESET_BTN,
                    SUBWIN_B_FACE_CB, //subwindow top checkbox
                    SUBWIN_B_SEGMENT_CB,
                    SUBWIN_B_RESET_BTN,
                    SUBWIN_T_SELECT_CB,
                    SUBWIN_B_SELECT_CB, //subwindow top checkbox
                    MODEL_PAIR_LB //listbox to swap model pairs
                    };

int subwinT_cbState = 0; //subwindow top checkbox state
int subwinT_SegmentState = 0;
int subwinB_cbState = 0;
int subwinB_SegmentState = 0;
int subwinT_cbSelect = 0;
int subwinB_cbSelect = 0;
int modelPair_lbID = 0; //mode pair listbox id

GLUI *gluiL, *gluiT, *gluiB, *gluiR;

string fileName_t, surfVert_t, face_t, skelVert_t, skelEdge_t;
string fileName_b, surfVert_b, face_b, skelVert_b, skelEdge_b;

string fileName_r, surfVert_r, face_r, skelVert_r, skelEdge_r;


GLUI_StaticText *fileNameT_txt,*surfVertT_txt,*faceT_txt,*skelVertT_txt,*skelEdgeT_txt;
GLUI_StaticText *fileNameB_txt,*surfVertB_txt,*faceB_txt,*skelVertB_txt,*skelEdgeB_txt;

GLUI_StaticText *fileNameR_txt,*surfVertR_txt,*faceR_txt,*skelVertR_txt,*skelEdgeR_txt;


int WindowWidth = 1280, WindowHeight = 720;

double posX_t = 0.0, posY_t = 0.0, posZ_t = 0.0, posInc_t = 0.1, angleInc_t = 2.0;
double rotateX_t = 0.0, rotateY_t = 0.0, rotateZ_t = 0.0;
static double scaleX_t = 1.0, scaleY_t = 1.0, scaleZ_t = 1.0;
int option_t = 0, axisornot_t = 0;
bool mousePanMode_t=false, mouseZoomMode_t=false, mouseRotationMode_t=false;
int mouseX_t = 0, mouseY_t = 0;
float eyeX_t=0, eyeY_t=0, eyeZ_t=0, focusX_t=0, focusY_t=0, focusZ_t=0, upX_t=0, upY_t=1, upZ_t=0;

double posX_b = 0.0, posY_b = 0.0, posZ_b = 0.0, posInc_b = 0.1, angleInc_b = 2.0;
double rotateX_b = 0.0, rotateY_b = 0.0, rotateZ_b = 0.0;
static double scaleX_b = 1.0, scaleY_b = 1.0, scaleZ_b = 1.0;
int option_b = 0, axisornot_b = 0;
bool mousePanMode_b=false, mouseZoomMode_b=false, mouseRotationMode_b=false;
int mouseX_b = 0, mouseY_b = 0;
float eyeX_b=0, eyeY_b=0, eyeZ_b=0, focusX_b=0, focusY_b=0, focusZ_b=0, upX_b=0, upY_b=1, upZ_b=0;

double posX_r = 0.0, posY_r = 0.0, posZ_r = 0.0, posInc_r = 0.1, angleInc_r = 2.0;
double rotateX_r = 0.0, rotateY_r = 0.0, rotateZ_r = 0.0;
static double scaleX_r = 1.0, scaleY_r = 1.0, scaleZ_r = 1.0;
int option_r = 0, axisornot_r = 0;
bool mousePanMode_r=false, mouseZoomMode_r=false, mouseRotationMode_r=false;
int mouseX_r = 0, mouseY_r = 0;
float eyeX_r=0, eyeY_r=0, eyeZ_r=0, focusX_r=0, focusY_r=0, focusZ_r=0, upX_r=0, upY_r=1, upZ_r=0;

/*//////////////////////////////////////
double posX = 0.0, posY = 0.0, posZ = 0.0, posInc = 0.1, angleInc = 2.0;
double rotateX = 0.0, rotateY = 0.0, rotateZ = 0.0;
static double scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0;
int option = 0, axisornot = 0;
bool mousePanMode=false, mouseZoomMode=false, mouseRotationMode=false;
int mouseX = 0, mouseY = 0;
float eyeX=0, eyeY=0, eyeZ=0, focusX=0, focusY=0, focusZ=0, upX=0, upY=1, upZ=0;
//*//////////////////////////////////////

int main_window, subwinL, subwinT, subwinB, subwinR;

string pathPoints = "data/hand.ply";
string pathSkeleton = "data/hand.skel";
string name = "hand";

GLUI_EditText   *edittext;

vertex model1, model2;
vertex model_blended;
//vertex_mod model_blended;

float transX_t=0, transY_t=0, transZ_t=0, transparent_t=1.0;
bool displayVertices_t = true, displaySegments_t = false, displayFaces_t = false, SubwinT_Segment_Select = false;
int WhichSegment = -1;

float transX_b=0, transY_b=0, transZ_b=0, transparent_b=1.0;
bool displayVertices_b = true, displaySegments_b = false, displayFaces_b = false, SubwinB_Segment_Select = false;
int WhichSegmentRemove = -1;

float transX_r=0, transY_r=0, transZ_r=0, transparent_r=1.0;
bool displayVertices_r = true, displayFaces_r = false, BlendStatus = false;

bool isModelLoaded = false;
bool isMultipleSelect = false;
int displaySegmentation = 0;
float model_1_ArrayXYZ[6], model_2_ArrayXYZ[6];

void gluiComponentCallback(int);
void MeasureFeatureModel1(int WhichSegment);

void refreshDisplay()
{
    //glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

void InitTransparent()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glClearColor(0,0,0,0);
}

void subwinT_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    GLfloat amb_light[] = { 0.5f,0.5f,0.5f,1.0f };
    GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    //GLfloat specular[] = { 0.1, 0.1, 0.1, 0.1};
    GLfloat lightpos[] = {0.f,0.f,200.0f,1.0f};

    glLightfv( GL_LIGHT0, GL_AMBIENT, amb_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
    //glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_LIGHTING);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(posX_t, posY_t, posZ_t);
    glRotatef(rotateX_t, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY_t, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ_t, 0.0f, 0.0f, 1.0f);
    glScalef(scaleX_t, scaleY_t, scaleZ_t);

    //glPushMatrix();
    //*//
    if(axisornot_t%2 == 1)
       model1.DrawAxis();  //Press

    if(displayVertices_t)// || subwinT_cbState == 0)
        model1.DisplayVertices();

    if(displayFaces_t)// || subwinT_cbState == 1)
        model1.DisplayFaces(transparent_t);
    if(SubwinT_Segment_Select == true)
    {
        model1.DisplaySpecificSegmentFaces(WhichSegment);
    }
    //model1.DisplaySkel();
    if(displaySegments_t)// || subwinT_cbState == 0)
    {
        model1.DisplaySegmentFaces();
    }
    //*///

    glPopMatrix();
    refreshDisplay();
}

void subwinB_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    GLfloat amb_light[] = { 0.5f,0.5f,0.5f,1.0f };
    GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    //GLfloat specular[] = { 0.1, 0.1, 0.1, 0.1};
    GLfloat lightpos[] = {0.f,0.f,200.0f,1.0f};

    glLightfv( GL_LIGHT0, GL_AMBIENT, amb_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
    //glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_LIGHTING);
    glPopMatrix();

    glPushMatrix();

    glTranslatef(posX_b, posY_b, posZ_b);
    glRotatef(rotateX_b, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY_b, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ_b, 0.0f, 0.0f, 1.0f);
    glScalef(scaleX_b, scaleY_b, scaleZ_b);

    //glPushMatrix();
    //*//
    if(axisornot_b%2 == 1)
       model2.DrawAxis();  //Press

    //if(displayVertices_b)
        //model2.DisplayVertices();
    if(displayVertices_b)// || subwinT_cbState == 0)
        model2.DisplayVertices();
    if(displayFaces_b)
        model2.DisplayFaces(transparent_b);
    //model2.DisplaySkel();
    if(displaySegments_b)// || subwinT_cbState == 0)
    {
        model2.DisplaySegmentFaces();
    }
    if(SubwinB_Segment_Select == true)
    {
        model2.DisplaySpecificSegmentFaces(WhichSegmentRemove);
    }

    glPopMatrix();
    //glFlush();
    //glutSwapBuffers();
    //glutPostRedisplay();
    refreshDisplay();

}

void subwinR_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    GLfloat amb_light[] = { 0.5f,0.5f,0.5f,1.0f };
    GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    //GLfloat specular[] = { 0.1, 0.1, 0.1, 0.1};
    GLfloat lightpos[] = {0.f,0.f,200.0f,1.0f};

    glLightfv( GL_LIGHT0, GL_AMBIENT, amb_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
    //glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_LIGHTING);
    glPopMatrix();

    glPushMatrix();

    glTranslatef(posX_r, posY_r, posZ_r);
    glRotatef(rotateX_r, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY_r, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ_r, 0.0f, 0.0f, 1.0f);
    glScalef(scaleX_r, scaleY_r, scaleZ_r);

    //glPushMatrix();
    //*//
    if(axisornot_r%2 == 1)
       model_blended.DrawAxis();  //Press

    //if(displayVertices_r)
        //model_blended.DisplayVertices();
//    if(displayFaces_r)
//        model_blended.DisplayFaces(transparent_r);
//    model_blended.DisplayPartialBlendedSkel();
    //*///
    if(subwinT_cbSelect == 1 && BlendStatus == false)
        model1.DisplaySelectedFeature(WhichSegment);
    if(subwinB_cbSelect == 1 && BlendStatus == false)
        model2.DisplayUnselectedFeature(WhichSegmentRemove);
    if(BlendStatus == true)
    {
        model2.Blend(model1, model2, model_1_ArrayXYZ, WhichSegment, model_2_ArrayXYZ, WhichSegmentRemove);
        model1.DisplayRemaining(WhichSegment);
    }
    glPopMatrix();
    //glFlush();
    //glutSwapBuffers();
    //glutPostRedisplay();
    refreshDisplay();

}

void viewing_t()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 0.1, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX_t, eyeY_t, eyeZ_t, focusX_t, focusY_t, focusZ_t, upX_t, upY_t, upZ_t);
}
void viewing_b()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 0.1, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX_b, eyeY_b, eyeZ_b, focusX_b, focusY_b, focusZ_b, upX_b, upY_b, upZ_b);
}
void viewing_r()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 0.1, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX_r, eyeY_r, eyeZ_r, focusX_r, focusY_r, focusZ_r, upX_r, upY_r, upZ_r);
}

        void resetView_t()
        {
            rotateX_t = 0.0, rotateY_t = 0.0, rotateZ_t = 0.0;
            posX_t = 0.0, posY_t = 0.0, posZ_t = 0.0;
            scaleX_t = 1.0, scaleY_t = 1.0, scaleZ_t = 1.0;
            glutSetWindow(subwinT);
            refreshDisplay();
        }
        void resetView_b()
        {
            rotateX_b = 0.0, rotateY_b = 0.0, rotateZ_b = 0.0;
            posX_b = 0.0, posY_b = 0.0, posZ_b = 0.0;
            scaleX_b = 1.0, scaleY_b = 1.0, scaleZ_b = 1.0;
            glutSetWindow(subwinB);
            refreshDisplay();
        }
        void ResetView_r()
        {
            rotateX_r = 0.0, rotateY_r = 0.0, rotateZ_r = 0.0;
            posX_r = 0.0, posY_r = 0.0, posZ_r = 0.0;
            scaleX_r = 1.0, scaleY_r = 1.0, scaleZ_r = 1.0;
            subwinT_cbSelect = 1, subwinB_cbSelect = 1, BlendStatus = false;
            subwinT_display();
            subwinB_display();
            subwinR_display();
            glutSetWindow(subwinR);
            refreshDisplay();
        }
        void BlendView_r()
        {
            int counterModel_1=0, counterModel_2=0;
            model1.MeasureFeatureModel(model_1_ArrayXYZ, 6, WhichSegment, counterModel_1);
            //cout<<"Model_1_XYZ: "<<model_1_ArrayXYZ[0]<<" "<<model_1_ArrayXYZ[1]<<" "<<model_1_ArrayXYZ[2]<<" "<<model_1_ArrayXYZ[3]<<" "<<model_1_ArrayXYZ[4]<<" "<<model_1_ArrayXYZ[5]<<endl;
            //cout<<"Counter_1: "<<counterModel_1<<endl;
            model2.MeasureFeatureModel(model_2_ArrayXYZ, 6, WhichSegmentRemove, counterModel_2);
            //cout<<"Model_2_XYZ: "<<model_2_ArrayXYZ[0]<<" "<<model_2_ArrayXYZ[1]<<" "<<model_2_ArrayXYZ[2]<<" "<<model_2_ArrayXYZ[3]<<" "<<model_2_ArrayXYZ[4]<<" "<<model_2_ArrayXYZ[5]<<endl;
            //cout<<"Counter_2: "<<counterModel_2<<endl;
            BlendStatus = true;

            glutSetWindow(subwinR);
            refreshDisplay();
        }

void subwinT_special(int key, int x, int y)
{//*//
    switch (key)
    {
    case GLUT_KEY_DOWN  :
        rotateX_t += angleInc_t;
        break;
    case GLUT_KEY_UP    :
        rotateX_t -= angleInc_t;
        break;
    case GLUT_KEY_RIGHT :
        rotateY_t += angleInc_t;
        break;
    case GLUT_KEY_LEFT  :
        rotateY_t -= angleInc_t;
        break;
    case GLUT_KEY_HOME  :
        resetView_t();
        break;
    case GLUT_KEY_F1    :
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case GLUT_KEY_F2    :
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    }//*///
    glutPostRedisplay();
}

void subwinB_special(int key, int x, int y)
{//*//
    switch (key)
    {
    case GLUT_KEY_DOWN  :
        rotateX_b += angleInc_b;
        break;
    case GLUT_KEY_UP    :
        rotateX_b -= angleInc_b;
        break;
    case GLUT_KEY_RIGHT :
        rotateY_b += angleInc_b;
        break;
    case GLUT_KEY_LEFT  :
        rotateY_b -= angleInc_b;
        break;
    case GLUT_KEY_HOME  :
        resetView_b();
        break;
    case GLUT_KEY_F1    :
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case GLUT_KEY_F2    :
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    }//*///
    glutPostRedisplay();
}


void subwinR_special(int key, int x, int y)
{//*//
    switch (key)
    {
    case GLUT_KEY_DOWN  :
        rotateX_r += angleInc_r;
        break;
    case GLUT_KEY_UP    :
        rotateX_r -= angleInc_r;
        break;
    case GLUT_KEY_RIGHT :
        rotateY_r += angleInc_r;
        break;
    case GLUT_KEY_LEFT  :
        rotateY_r -= angleInc_r;
        break;
    case GLUT_KEY_HOME  :
        ResetView_r();
        break;
    case GLUT_KEY_F1    :
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case GLUT_KEY_F2    :
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    }//*///
    glutPostRedisplay();
}




        void showFaces_t(int state = 0)
        {
            if(state == 0)
                displayVertices_t = true, displayFaces_t = false;
            else
                displayVertices_t = false, displayFaces_t = true;
            subwinT_cbState = state;
            gluiL->sync_live();

            glutSetWindow (subwinT); //target subwindow
            refreshDisplay();
        }
        void showSegments_t(int state = 0)
        {
            if(state == 0)
                displayVertices_t = true, displaySegments_t = false, displayFaces_t = false;
            else
                displayVertices_t = false, displaySegments_t = true, displayFaces_t = true;
            subwinT_SegmentState = state;
            gluiL->sync_live();

            glutSetWindow (subwinT); //target subwindow
            refreshDisplay();
        }
        void showFaces_b(int state = 0)
        {
            if(state == 0)
                displayVertices_b = true, displayFaces_b = false;
            else
                displayVertices_b = false, displayFaces_b = true;
            subwinB_cbState = state;
            gluiL->sync_live();

            glutSetWindow (subwinB); //target subwindow
            refreshDisplay();
        }
        void showSegments_b(int state = 0)
        {
            if(state == 0)
                displayVertices_b = true, displaySegments_b = false, displayFaces_b = false;
            else
                displayVertices_b = false, displaySegments_b = true, displayFaces_b = true;
            subwinB_SegmentState = state;
            gluiL->sync_live();

            glutSetWindow (subwinB); //target subwindow
            refreshDisplay();
        }
        void showSelect_t(int state = 0)
        {
            if(state == 1)
            {
                displayVertices_t = false, displaySegments_t = false, displayFaces_t = true, SubwinT_Segment_Select = true;
                //glutDisplayFunc (subwinT_display);
                cout<<"Model 1 --> Single digit number, choose between: 0 to "<<model1.segMax<<endl;
            }
            else
                SubwinT_Segment_Select = false;
            subwinT_SegmentState = 0;
            subwinT_cbSelect = state;
            gluiL->sync_live();

            glutSetWindow (subwinT); //target subwindow
            refreshDisplay();
        }
        void showSelect_b(int state = 0)
        {
            if(state == 1)
            {
                displayVertices_b = false, displaySegments_b = false, displayFaces_r = true, SubwinB_Segment_Select = true;
                cout<<"Model 2 --> Single digit number, choose between: 0 to "<<model2.segMax-model1.segMax-2<<endl;
            }
            else
                SubwinB_Segment_Select = false;
            subwinB_SegmentState = 0;
            subwinB_cbSelect = state;
            gluiL->sync_live();

            glutSetWindow (subwinB); //target subwindow
            refreshDisplay();
        }



void subwinT_keyboard(unsigned char key, int x, int y) //glut callback keyboard
{
    switch (key)
    {
        case 's': axisornot_t += 1; //On or Off the axis
                  break;
        case 'v':
        case 'V': showFaces_t(0); break; // toggle off the faces
        case 'f':
        case 'F': showFaces_t(1); break; // toggle on the faces

        // Choose which segment is to be displayed
        case '0': WhichSegment = 0; break;
        case '1': WhichSegment = 1; break;
        case '2': WhichSegment = 2; break;
        case '3': WhichSegment = 3; break;
        case '4': WhichSegment = 4; break;
        case '5': WhichSegment = 5; break;
        case '6': WhichSegment = 6; break;
        case '7': WhichSegment = 7; break;
        case '8': WhichSegment = 8; break;
        case '9': WhichSegment = 9; break;
    }

    glutPostRedisplay();
}

void subwinB_keyboard(unsigned char key, int x, int y) //glut callback keyboard
{//*
    switch (key)
    {
        case 's': axisornot_b += 1; //On or Off the axis
                  break;
        case 'x': transX_b +=0.005; break;
        case 'X': transX_b -=0.005; break;
        case 'y': transY_b +=0.005; break;
        case 'Y': transY_b -=0.005; break;
        case 'z': transZ_b +=0.005; break;
        case 'Z': transZ_b -=0.005; break;
        case 'v':
        case 'V': showFaces_b(0); break;
        case 'f':
        case 'F': showFaces_b(1); break;
        case '0': WhichSegmentRemove = 0; break;
        case '1': WhichSegmentRemove = 1; break;
        case '2': WhichSegmentRemove = 2; break;
        case '3': WhichSegmentRemove = 3; break;
        case '4': WhichSegmentRemove = 4; break;
        case '5': WhichSegmentRemove = 5; break;
        case '6': WhichSegmentRemove = 6; break;
        case '7': WhichSegmentRemove = 7; break;
        case '8': WhichSegmentRemove = 8; break;
        case '9': WhichSegmentRemove = 9; break;
    }//*/

    glutPostRedisplay();
}

void subwinR_keyboard(unsigned char key, int x, int y) //glut callback keyboard
{//*
    switch (key)
    {
        case 's': axisornot_r += 1; //On or Off the axis
                  break;
        case 'x': transX_r +=0.005; break;
        case 'X': transX_r -=0.005; break;
        case 'y': transY_r +=0.005; break;
        case 'Y': transY_r -=0.005; break;
        case 'z': transZ_r +=0.005; break;
        case 'Z': transZ_r -=0.005; break;
//        case 'v':
//        case 'V': showFaces_r(0); break;
//        case 'f':
//        case 'F': showFaces_r(1); break;

    }//*/

    glutPostRedisplay();
}

/// Process mouse click hit on view port
void processHits( GLint hits, GLuint nameBuffer[] )
{
    unsigned int i;
    unsigned int j;
    GLuint names;
    GLuint* pPtr;


    float minTemp = 1.0f;
    GLuint namesTemp = -1;

    pPtr = nameBuffer;

    cout<<"*pPtr: "<<*pPtr<<" hit: "<<hits<<endl;
    if( hits > 0 )
    {
        names = *pPtr;
        ++pPtr;
        float tempp = *pPtr/(pow(2,32)-1.0);
        ++pPtr;
        ++pPtr;
        for( j = 0; j < names; ++j, ++pPtr )
        {
            namesTemp = *pPtr;
            if(tempp < minTemp)
            {
                //namesTemp = *pPtr;
                minTemp = tempp;
            }
        }
    }
    cout<<"namesTemp: "<<namesTemp<<endl;
//    model1.selectedFace = namesTemp;
//    if( namesTemp != -1 ){
//        model1.selectedPart = model1.tri_triags[namesTemp].color;
//        if( displaySegmentation )
//        {
//            model1.addSelectedFace(model1.selectedFace);
//        }
//    }else {
//        model1.selectedPart = -1;
//    }


    //model1.selectedPart = namesTemp;

    //cout << " > Selected Face = " << model1.selectedFace << endl;
    //cout << " > Selected Part = " << model1.selectedPart << endl;
}

void mousePickingFunction(int x, int y)
{
    GLuint nameBuffer[200];
    GLint hits;
    GLint viewport[4];
    GLfloat projMatrix[16];

    glSelectBuffer( 200, nameBuffer ); /* select a buffer for the hit results */
    glRenderMode( GL_SELECT ); /* must change to select mode _before_ you touch the name stack */

    /* initialise the name stack */
    glInitNames();
    glPushName( -1 ); /* -1 put a dummy on the stack so we can call glLoadName without error */

    /*
        set up the viewing volume for selection
    */

    /*
        we need to premultiply the current projection matrix by
        the gluPickMatrix.  OpenGL always postmultiplies though.
        Thus we need to get a copy of the current projection
        matrix, load up gluPickMatrix, and then postmultiply
        gluPickMatrix by the current projection matrix.
    */

    /* get a copy of the current projection matrix */
    glGetFloatv( GL_PROJECTION_MATRIX, projMatrix );

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();

    /* pick a 20x20 region around the cursor */
    glGetIntegerv( GL_VIEWPORT, viewport );
    gluPickMatrix( (GLdouble)( x ), (GLdouble)( WindowHeight - y ), 0.5, 0.5, viewport );

    glMultMatrixf( projMatrix ); /* post multiply the "current" projection matrix */
    glMatrixMode( GL_MODELVIEW );

    subwinT_display(); //myDisplayFunc();
    glFlush();
//
//        /* restore the projection matrix */
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );

    /*
        Now process the information obtained
    */
    hits = glRenderMode( GL_RENDER );
    cout<<"hit: "<<hits<<endl;
    if( -1 != hits )
    {
        processHits( hits, nameBuffer );
        glutPostRedisplay();
    }
    else
    {
        cout<<"Picking hits overflowed the name buffer\n";
        //fprintf( stderr, "Picking hits overflowed the name buffer\n" );
    }
}
void glutMouseClickVP(int button, int state, int x, int y)
{
    // No model loaded do not perform click selection
    if( !isModelLoaded ) return;

    /// Mouse click selection
    if( GLUT_LEFT_BUTTON == button && state==GLUT_DOWN)
    {
        //mousePickingFunction(x, y);
        if( displaySegmentation && !isMultipleSelect ){
            isMultipleSelect = true;
            return;
        }
    }
    else if( GLUT_LEFT_BUTTON == button && state==GLUT_UP )
    {
        if( isMultipleSelect ) isMultipleSelect = false;
    }
}
void mainWindow_reshape(int width, int height)
{
    //int x = glutGet((GLenum)GLUT_WINDOW_X);
    //int y = glutGet((GLenum)GLUT_WINDOW_Y);
    WindowWidth  = width;
    WindowHeight = height;
    //glViewport(0, 0, width, height);
    glutReshapeWindow(1280,720);
    //glutPositionWindow(100,y);
}

void subwinT_motion(int x, int y)
{//*
    y = WindowHeight - y;
    if(mousePanMode_t)
    {
        posX_t += (x - mouseX_t)*0.005;
        posY_t += (y - mouseY_t)*0.005;
        mouseX_t = x;
        mouseY_t = y;
        glutPostRedisplay();
    }
    if(mouseZoomMode_t)
    {
        GLdouble inc = (mouseY_t - y)*0.005;
        scaleX_t += inc;
        scaleY_t += inc;
        scaleZ_t += inc;
        mouseY_t = y;
        glutPostRedisplay();
    }
    if(mouseRotationMode_t)
    {
        rotateX_t += (mouseY_t - y)*0.5;
        rotateY_t += (x - mouseX_t)*0.5;
        mouseX_t = x;
        mouseY_t = y;
        glutPostRedisplay();
    }
//*/
}

void subwinB_motion(int x, int y)
{//*
    y = WindowHeight - y;
    if(mousePanMode_b)
    {
        posX_b += (x - mouseX_b)*0.005;
        posY_b += (y - mouseY_b)*0.005;
        mouseX_b = x;
        mouseY_b = y;
        glutPostRedisplay();
    }
    if(mouseZoomMode_b)
    {
        GLdouble inc = (mouseY_b - y)*0.005;
        scaleX_b += inc;
        scaleY_b += inc;
        scaleZ_b += inc;
        mouseY_b = y;
        glutPostRedisplay();
    }
    if(mouseRotationMode_b)
    {
        rotateX_b += (mouseY_b - y)*0.5;
        rotateY_b += (x - mouseX_b)*0.5;
        mouseX_b = x;
        mouseY_b = y;
        glutPostRedisplay();
    }
//*/
}

void subwinR_motion(int x, int y)
{//*
    y = WindowHeight - y;
    if(mousePanMode_r)
    {
        posX_r += (x - mouseX_r)*0.005;
        posY_r += (y - mouseY_r)*0.005;
        mouseX_r = x;
        mouseY_r = y;
        glutPostRedisplay();
    }
    if(mouseZoomMode_r)
    {
        GLdouble inc = (mouseY_r - y)*0.005;
        scaleX_r += inc;
        scaleY_r += inc;
        scaleZ_r += inc;
        mouseY_r = y;
        glutPostRedisplay();
    }
    if(mouseRotationMode_r)
    {
        rotateX_r += (mouseY_r - y)*0.5;
        rotateY_r += (x - mouseX_r)*0.5;
        mouseX_r = x;
        mouseY_r = y;
        glutPostRedisplay();
    }
//*/
}


void subwinT_mouse(int button, int state, int x, int y)
{
    y = WindowHeight - y;
//    if(state != GLUT_DOWN)
//    return;
//
//    GLbyte color[4];
//    GLfloat depth;
//    GLuint index;
//
//    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
//    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
//    glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

//    cout<<"Click:x: "<<x<<" y: "<<y<<" c0: "<<color[0]<<" c1: "<<color[1]<<" c2: "<<color[2]<<" c3: "<<color[3]<<" d: "<<depth<<" ind: "<<index<<endl;
//    glm::vec4 viewport = glm::vec4(0, 0, window_width, window_height);
//    glm::vec3 wincoord = glm::vec3(x, window_height - y - 1, depth);
//    glm::vec3 objcoord = glm::unProject(wincoord, view, projection, viewport);
//
//    cout<<"Coordinates in object space: "<<objcoord.x, objcoord.y, objcoord.z);
    glutMouseClickVP(button, state, x, y);
    switch (button)
    {
    case GLUT_MIDDLE_BUTTON:
        if (state==GLUT_DOWN && !mousePanMode_t)
        {
            mouseX_t = x;
            mouseY_t = y;
            mousePanMode_t = true;
        }
        if (state==GLUT_UP &&  mousePanMode_t)
        {
            mousePanMode_t = false;
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state==GLUT_DOWN && !mouseZoomMode_t)
        {
            mouseY_t = y;
            mouseZoomMode_t = true;
        }
        if (state==GLUT_UP && mouseZoomMode_t)
        {
            mouseZoomMode_t = false;
        }
        break;
    case GLUT_LEFT_BUTTON:
        if (state==GLUT_DOWN && !mouseRotationMode_t)
        {
            mouseX_t = x;
            mouseY_t = y;
            mouseRotationMode_t = true;
        }
        if (state==GLUT_UP &&  mouseRotationMode_t)
        {
            mouseRotationMode_t = false;
        }
        break;
    }
}

void subwinB_mouse(int button, int state, int x, int y)
{
    y = WindowHeight - y;
    switch (button)
    {
    case GLUT_MIDDLE_BUTTON:
        if (state==GLUT_DOWN && !mousePanMode_b)
        {
            mouseX_b = x;
            mouseY_b = y;
            mousePanMode_b = true;
        }
        if (state==GLUT_UP &&  mousePanMode_b)
        {
            mousePanMode_b = false;
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state==GLUT_DOWN && !mouseZoomMode_b)
        {
            mouseY_b = y;
            mouseZoomMode_b = true;
        }
        if (state==GLUT_UP && mouseZoomMode_b)
        {
            mouseZoomMode_b = false;
        }
        break;
    case GLUT_LEFT_BUTTON:
        if (state==GLUT_DOWN && !mouseRotationMode_b)
        {
            mouseX_b = x;
            mouseY_b = y;
            mouseRotationMode_b = true;
        }
        if (state==GLUT_UP &&  mouseRotationMode_b)
        {
            mouseRotationMode_b = false;
        }
        break;
    }
}

void subwinR_mouse(int button, int state, int x, int y)
{
    y = WindowHeight - y;
    switch (button)
    {
    case GLUT_MIDDLE_BUTTON:
        if (state==GLUT_DOWN && !mousePanMode_r)
        {
            mouseX_r = x;
            mouseY_r = y;
            mousePanMode_r = true;
        }
        if (state==GLUT_UP &&  mousePanMode_r)
        {
            mousePanMode_r = false;
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state==GLUT_DOWN && !mouseZoomMode_r)
        {
            mouseY_r = y;
            mouseZoomMode_r = true;
        }
        if (state==GLUT_UP && mouseZoomMode_r)
        {
            mouseZoomMode_r = false;
        }
        break;
    case GLUT_LEFT_BUTTON:
        if (state==GLUT_DOWN && !mouseRotationMode_r)
        {
            mouseX_r = x;
            mouseY_r = y;
            mouseRotationMode_r = true;
        }
        if (state==GLUT_UP &&  mouseRotationMode_r)
        {
            mouseRotationMode_r = false;
        }
        break;
    }
}

void myGlutIdle(void)
{
    glutSetWindow(main_window);
    glutPostRedisplay();
}

void switchModel_t(int model_idx)
{
    //string skeleton_filename = "../data/chair8Seg.skel";
    string segment_filename = "../data/chair8Seg.seg";
    string points_filename = "../data/chair8Seg.ply";
    //string skelSegment_filename = "../data/SkeletonVerticesForHandModel.txt";
    eyeX_t=0, eyeY_t=0, eyeZ_t=3.0, focusX_t=0, focusY_t=0, focusZ_t=0, upX_t=0, upY_t=1, upZ_t=0;
    fileName_t = "Bird 5";

    model1.read3DModel(points_filename.c_str());
    //model1.read3DSkeleton(skeleton_filename.c_str());
    //model1.readSkelSegment(skelSegment_filename.c_str());
    model1.readSegment(segment_filename.c_str());
    isModelLoaded = true;
    /*//
    ostringstream sstream;
    sstream << model1.noOfVertices << " surface vertices";
    surfVert_t = sstream.str();
    sstream.str("");
    sstream.clear();
    sstream << model1.noOfFaces << " faces";
    face_t = sstream.str();
    sstream.str("");
    sstream.clear();
    sstream << model1.countSV << " skeleton vertices";
    skelVert_t = sstream.str();
    sstream.str("");
    sstream.clear();
    sstream << model1.countSE << " skeleton edges";
    skelEdge_t = sstream.str();
    //*///
}

void switchModel_b(int model_idx)
{
    // Input files
    string points_filename = "../data/chair10Seg.ply";
    string segment_filename = "../data/chair10Seg.seg";

    eyeX_b=0, eyeY_b=0, eyeZ_b=3.0, focusX_b=0, focusY_b=0, focusZ_b=0, upX_b=0, upY_b=1, upZ_b=0;
    fileName_b = "Bird 7";

    model2.read3DModel(points_filename.c_str()); // read .ply file
    model2.readSegment(segment_filename.c_str()); // read .seg file
}



void init_t(int model_idx)
{
    switchModel_t(model_idx);
    InitTransparent();
        viewing_t();
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glEnable(GL_NORMALIZE);
    glClearColor(1.0,1.0,1.0, 1.0);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    refreshDisplay();
}

void init_b(int model_idx)
{
    switchModel_b(model_idx);
    InitTransparent();
        viewing_b();
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glEnable(GL_NORMALIZE);
    glClearColor(1.0,1.0,1.0, 1.0);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    refreshDisplay();
}
void switchModel_r()
{
//    string points_filename = "../data/DiffColorHand.ply";
//    string skeleton_filename = "../data/DiffColorHand.skel";
//    string segment_filename = "../data/DiffColorHandSBS.seg";
//    string skelSegment_filename = "../data/SkeletonVerticesForHandModel.txt";
//    string blended_filename = "../data/blendedSegment.txt";
    eyeX_r=0, eyeY_r=0, eyeZ_r=3.0, focusX_r=0, focusY_r=0, focusZ_r=0, upX_r=0, upY_r=1, upZ_r=0;
    fileName_r = "Partially blended model";
//
//    model_blended.read3DModel(points_filename.c_str());
//    model_blended.read3DSkeleton(skeleton_filename.c_str());
//    model_blended.readSkelSegment(skelSegment_filename.c_str());
//    model_blended.readBlendedSegment(blended_filename.c_str());
//    model_blended.prepareBlended();
    //model_blended.deleteOldSegments();
}

void init_r()
{
    switchModel_r();
    InitTransparent();
        viewing_r();
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glEnable(GL_NORMALIZE);
    glClearColor(1.0,1.0,1.0, 1.0);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    refreshDisplay();
}





///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

void mainWindow_display(void)
{
    glClearColor(0.8, 0.8, 0.8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers ();
}

void subwindowL_display (void)
{
	glClearColor(0.8, 0.8, 0.8, 1.0); //hide subwindow for aesthetic purposes
    glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers ();
}

void subwindowR_display (void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers ();
}










///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////


void createGlutMainWindow()
{
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition(0, 0); // Set top-left position
    glutInitWindowSize(WindowWidth, WindowHeight); //Set width and height
    main_window = glutCreateWindow("3D Blending Prototype");// Create display window

    glutDisplayFunc(mainWindow_display);  // Specify the display callback function
    glutReshapeFunc(mainWindow_reshape);
}

void createGlutSubwinT()
{
    subwinT = glutCreateSubWindow(main_window,230,20,330,330);
    glutDisplayFunc (subwinT_display);
	glutMouseFunc (subwinT_mouse);
	glutMotionFunc (subwinT_motion);
	glutKeyboardFunc (subwinT_keyboard);
	glutSpecialFunc(subwinT_special);
	init_t(0);
}

void createGlutSubwinB()
{
    subwinB = glutCreateSubWindow(main_window,230,370,330,330);
    glutDisplayFunc (subwinB_display);
	glutMouseFunc (subwinB_mouse);
	glutMotionFunc (subwinB_motion);
	glutKeyboardFunc (subwinB_keyboard);
	glutSpecialFunc(subwinB_special);
	init_b(1);
}

void createGlutSubwinR()
{
    subwinR = glutCreateSubWindow(main_window,580,20,680,680);
    glutDisplayFunc (subwinR_display);
	glutMouseFunc (subwinR_mouse);
	glutMotionFunc (subwinR_motion);
	glutKeyboardFunc (subwinR_keyboard);
	glutSpecialFunc(subwinR_special);
    init_r();
}

void createGlutSubwinL()
{
    subwinL = glutCreateSubWindow(main_window,20,20,190,680);
    glutDisplayFunc (subwindowL_display);

}

void createGluiSubwinT()
{
    gluiT = GLUI_Master.create_glui_subwindow( subwinT, GLUI_SUBWINDOW_TOP );
    new GLUI_StaticText (gluiT, "Model 1");
    gluiT->set_main_gfx_window( main_window );
}
void createGluiSubwinB()
{
    gluiB = GLUI_Master.create_glui_subwindow( subwinB, GLUI_SUBWINDOW_TOP );
    new GLUI_StaticText (gluiB, "Model 2");
    gluiB->set_main_gfx_window( main_window );
}

void createGluiSubwinR()
{
    gluiR = GLUI_Master.create_glui_subwindow( subwinR, GLUI_SUBWINDOW_TOP );
    new GLUI_StaticText (gluiR, "Blended model");
    gluiR->set_main_gfx_window( main_window );
}

void createGluiSubwinL()
{
    gluiL = GLUI_Master.create_glui_subwindow( subwinL, GLUI_SUBWINDOW_TOP );
    new GLUI_StaticText(gluiL, "3D Blending");
    gluiL->add_separator();
    //////////////////////////////////////
    //////////////////////////////////////
    //////////////////////////////////////
    /*//
    GLUI_Listbox *listboxL = gluiL->add_listbox("Model pairs: ",&modelPair_lbID,
                                                MODEL_PAIR_LB,gluiComponentCallback);
    listboxL->add_item(0, "Hands");
    listboxL->add_item(1, "Demo");
    gluiL->add_separator();
    //*///
    //////////////////////////////////////
    //////////////////////////////////////
    //////////////////////////////////////
    new GLUI_StaticText(gluiL, "View controls:");
    new GLUI_StaticText(gluiL, "Left mouse drag to rotate.");
    new GLUI_StaticText(gluiL, "Right mouse drag to zoom.");
    new GLUI_StaticText(gluiL, "Middle mouse drag to pan.");
    new GLUI_StaticText(gluiL, "");
    gluiL->add_separator();
    //////////////////////////////////////
    //////////////////////////////////////
    //////////////////////////////////////
    new GLUI_StaticText(gluiL, "Model 1:");
    fileNameT_txt = gluiL->add_statictext(fileName_t.c_str());
    new GLUI_StaticText(gluiL, "");
    /*//
    surfVertT_txt = gluiL->add_statictext(surfVert_t.c_str());
    faceT_txt = gluiL->add_statictext(face_t.c_str());
    skelVertT_txt = gluiL->add_statictext(skelVert_t.c_str());
    skelEdgeT_txt = gluiL->add_statictext(skelEdge_t.c_str());
    //*///
    GLUI_Checkbox *checkT = gluiL->add_checkbox("Show faces ",&subwinT_cbState,
                                                SUBWIN_T_FACE_CB,gluiComponentCallback);
    GLUI_Checkbox *checkTSegment = gluiL->add_checkbox("Show Segments ",&subwinT_SegmentState,
                                                SUBWIN_T_SEGMENT_CB,gluiComponentCallback);
    GLUI_Button *btnResetViewT = gluiL->add_button("Reset view",
                                                   SUBWIN_T_RESET_BTN, gluiComponentCallback);
    btnResetViewT->set_alignment(GLUI_ALIGN_LEFT);
    new GLUI_StaticText(gluiL, "");
    gluiL->add_separator();
    //////////////////////////////////////
    //////////////////////////////////////
    //////////////////////////////////////
    new GLUI_StaticText(gluiL, "Model 2:");
    fileNameB_txt = gluiL->add_statictext(fileName_b.c_str());
    new GLUI_StaticText(gluiL, "");
    /*//
    surfVertB_txt = gluiL->add_statictext(surfVert_b.c_str());
    faceB_txt = gluiL->add_statictext(face_b.c_str());
    skelVertB_txt = gluiL->add_statictext(skelVert_b.c_str());
    skelEdgeB_txt = gluiL->add_statictext(skelEdge_b.c_str());
    //*///
    GLUI_Checkbox *checkB = gluiL->add_checkbox("Show faces ",&subwinB_cbState,
                                                SUBWIN_B_FACE_CB,gluiComponentCallback);
    GLUI_Checkbox *checkBSegment = gluiL->add_checkbox("Show Segments ",&subwinB_SegmentState,
                                                SUBWIN_B_SEGMENT_CB,gluiComponentCallback);
    GLUI_Button *btnResetViewB = gluiL->add_button("Reset view",
                                                   SUBWIN_B_RESET_BTN, gluiComponentCallback);
    btnResetViewB->set_alignment(GLUI_ALIGN_LEFT);
    new GLUI_StaticText(gluiL, "");
    gluiL->add_separator();
     //////////////////////////////////////
    //////////////////////////////////////
    //////////////////////////////////////
    new GLUI_StaticText(gluiL, "Blended model");
    new GLUI_StaticText(gluiL, "Instruction:");
    new GLUI_StaticText(gluiL, "1) Tick a box below");
    new GLUI_StaticText(gluiL, "2) Click on respective window");
    new GLUI_StaticText(gluiL, "3) Input single digit number");
    new GLUI_StaticText(gluiL, "   from keyboard");
    /*//
    new GLUI_StaticText(gluiL, "(input_file_name)");
    new GLUI_StaticText(gluiL, "### surface vertices");
    new GLUI_StaticText(gluiL, "### faces");
    new GLUI_StaticText(gluiL, "### skeleton vertices");
    new GLUI_StaticText(gluiL, "### skeleton edges");
    //*///
    new GLUI_StaticText(gluiL, "");
    GLUI_Checkbox *SelectFeatureToRemove = gluiL->add_checkbox("Select 1 feature fr Model 1",&subwinT_cbSelect,
                                                SUBWIN_T_SELECT_CB,gluiComponentCallback);
    GLUI_Checkbox *ReplaceByFeatureModel2 = gluiL->add_checkbox("Replace 1 feature fr Model 2",&subwinB_cbSelect,
                                                SUBWIN_B_SELECT_CB,gluiComponentCallback);
    GLUI_Button *btnBlendedR = gluiL->add_button("Blended Result", SUBWIN_R_BLENDED_BTN, gluiComponentCallback);
    GLUI_Button *btnResetViewR = gluiL->add_button("Reset View", SUBWIN_R_RESET_BTN, gluiComponentCallback);

    btnBlendedR->set_alignment(GLUI_ALIGN_LEFT);
    btnResetViewR->set_alignment(GLUI_ALIGN_LEFT);
    new GLUI_StaticText(gluiL, "");
    new GLUI_StaticText(gluiL, "");
    gluiL->add_separator();
     //////////////////////////////////////

    GLUI_Button *quit_btn =  gluiL->add_button( "Quit", 0,(GLUI_Update_CB)exit );
    quit_btn->set_alignment(GLUI_ALIGN_LEFT);
    new GLUI_StaticText(gluiL, "");

    GLUI_Master.set_glutIdleFunc( NULL );
    gluiL->set_main_gfx_window( main_window );
}



void gluiComponentCallback(int component_id)
{
    switch(component_id)
    {
        //btns
        case SUBWIN_T_RESET_BTN:
        {
            resetView_t();
            break;
        }
        case SUBWIN_B_RESET_BTN:
        {
            resetView_b();
            break;
        }
        case SUBWIN_R_RESET_BTN:
        {
            ResetView_r();
            break;
        }
        case SUBWIN_R_BLENDED_BTN:
        {
            BlendView_r();
            BlendStatus = true;
            break;
        }

        //checkboxes
        case SUBWIN_T_FACE_CB:
        {
            showFaces_t(subwinT_cbState);
            break;
        }
        case SUBWIN_T_SEGMENT_CB:
        {
            showSegments_t(subwinT_SegmentState);
            break;
        }
        case SUBWIN_B_FACE_CB:
        {
            showFaces_b(subwinB_cbState);
            break;
        }
        case SUBWIN_B_SEGMENT_CB:
        {
            showSegments_b(subwinB_SegmentState);
            break;
        }
        case SUBWIN_T_SELECT_CB:
        {
            showSelect_t(subwinT_cbSelect);
            break;
        }
        case SUBWIN_B_SELECT_CB:
        {

            showSelect_b(subwinB_cbSelect);
            break;
        }
    }
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    createGlutMainWindow(); //main window

    createGlutSubwinT(); //subwindow top, model 1
    createGlutSubwinB(); //subwindow bottom, model 2
    createGlutSubwinR(); //subwindow right, blended model
    createGlutSubwinL(); //subwindow left, UI controls

    createGluiSubwinT(); //label for subwindow top
    createGluiSubwinB(); //label for subwindow bottom
    createGluiSubwinR(); //label for subwindow right
    createGluiSubwinL(); //ui controls in subwindow left

    glutMainLoop();
    return 0;
}














