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

using namespace std;

int WindowWidth = 500, WindowHeight = 500;
double posX = 0.0, posY = 0.0, posZ = 0.0, posInc = 0.1, angleInc = 2.0;
double rotateX = 0.0, rotateY = 0.0, rotateZ = 0.0;
static double scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0;
int option = 0, axisornot = 0;
bool mousePanMode=false, mouseZoomMode=false, mouseRotationMode=false;
int mouseX = 0, mouseY = 0;
float eyeX=0, eyeY=0, eyeZ=0, focusX=0, focusY=0, focusZ=0, upX=0, upY=1, upZ=0;

int main_window, subwinL, subwinT, subwinB, subwinR;

string pathPoints = "data/hand.ply";
string pathSkeleton = "data/hand.skel";
string name = "hand";

GLUI_EditText   *edittext;

//vertex Labelling;
float transX=0, transY=0, transZ=0, transparent=1.0;
bool displayVertices = true, displayFaces = false;

void InitTransparent()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glClearColor(0,0,0,0);
}
void myDisplayFunc(void)
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

    glTranslatef(posX, posY, posZ);
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);
    glScalef(scaleX, scaleY, scaleZ);

    //glPushMatrix();
    /*
    if(axisornot%2 == 1)
       Labelling.DrawAxis();  //Press

    if(displayVertices)
        Labelling.DisplayVertices();
    if(displayFaces)
        Labelling.DisplayFaces(transparent);
    Labelling.DisplaySkel();
    */

    glPopMatrix();
    glFlush();
    glutSwapBuffers();
    //glutPostRedisplay();
}

void viewing()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 0.1, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, focusX, focusY, focusZ, upX, upY, upZ);
}
void mySpecialFunc(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_DOWN  :
        rotateX += angleInc;
        break;
    case GLUT_KEY_UP    :
        rotateX -= angleInc;
        break;
    case GLUT_KEY_RIGHT :
        rotateY += angleInc;
        break;
    case GLUT_KEY_LEFT  :
        rotateY -= angleInc;
        break;
    case GLUT_KEY_HOME  :
        rotateX = 0.0, rotateY = 0.0, rotateZ = 0.0;
        posX = 0.0, posY = 0.0, posZ = 0.0;
        scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0;
        break;
    case GLUT_KEY_F1    :
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case GLUT_KEY_F2    :
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    }
    glutPostRedisplay();
}
void myKeyboardFunc(unsigned char key, int x, int y) //glut callback keyboard
{
    switch (key)
    {
        case 's': axisornot += 1; //On or Off the axis
                  break;
        case 'x': transX +=0.005; break;
        case 'X': transX -=0.005; break;
        case 'y': transY +=0.005; break;
        case 'Y': transY -=0.005; break;
        case 'z': transZ +=0.005; break;
        case 'Z': transZ -=0.005; break;
        case 'v':
        case 'V': displayVertices = true, displayFaces = false; break;
        case 'f':
        case 'F': displayVertices = false, displayFaces = true; break;

    }

    glutPostRedisplay();
}
void myReshapeFunc(int width, int height)
{
    //int x = glutGet((GLenum)GLUT_WINDOW_X);
    //int y = glutGet((GLenum)GLUT_WINDOW_Y);
    WindowWidth  = width;
    WindowHeight = height;
    //glViewport(0, 0, width, height);
    glutReshapeWindow(1280,720);
    //glutPositionWindow(100,y);
}
void myMotionFunc(int x, int y)
{
    y = WindowHeight - y;
    if(mousePanMode)
    {
        posX += (x - mouseX)*0.5;
        posY += (y - mouseY)*0.5;
        mouseX = x;
        mouseY = y;
        glutPostRedisplay();
    }
    if(mouseZoomMode)
    {
        GLdouble inc = (mouseY - y)*0.005;
        scaleX += inc;
        scaleY += inc;
        scaleZ += inc;
        mouseY = y;
        glutPostRedisplay();
    }
    if(mouseRotationMode)
    {
        rotateX += (mouseY - y)*0.5;
        rotateY += (x - mouseX)*0.5;
        mouseX = x;
        mouseY = y;
        glutPostRedisplay();
    }

}
void myMouseFunc(int button, int state, int x, int y)
{
    y = WindowHeight - y;
    switch (button)
    {
    case GLUT_MIDDLE_BUTTON:
        if (state==GLUT_DOWN && !mousePanMode)
        {
            mouseX = x;
            mouseY = y;
            mousePanMode = true;
        }
        if (state==GLUT_UP &&  mousePanMode)
        {
            mousePanMode = false;
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state==GLUT_DOWN && !mouseZoomMode)
        {
            mouseY = y;
            mouseZoomMode = true;
        }
        if (state==GLUT_UP && mouseZoomMode)
        {
            mouseZoomMode = false;
        }
        break;
    case GLUT_LEFT_BUTTON:
        if (state==GLUT_DOWN && !mouseRotationMode)
        {
            mouseX = x;
            mouseY = y;
            mouseRotationMode = true;
        }
        if (state==GLUT_UP &&  mouseRotationMode)
        {
            mouseRotationMode = false;
        }
        break;
    }
}

void myGlutIdle(void)
{
    glutSetWindow(main_window);
    glutPostRedisplay();
}

void switch_models(int index)
{
    string skeleton_filename;
    string points_filename;

    switch(index)
    {
    case 0:
        points_filename = "data/hand.ply";
        skeleton_filename = "data/hand.skel";
        eyeX=0, eyeY=0, eyeZ=1.5, focusX=0, focusY=0, focusZ=0, upX=0, upY=1, upZ=0;
        break;

    }
    cout << "Switched model to " << points_filename << endl;
    cout << "Display skeleton of " << skeleton_filename << endl;
    /*
    Labelling.read3DModel(points_filename.c_str());
    Labelling.read3DSkeleton(skeleton_filename.c_str());
    */
    //Koon Xian, add your code here

}
void myInit()
{


    switch_models(0);
    InitTransparent();

    viewing();
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    glEnable(GL_NORMALIZE);

    glClearColor(0.8, 0.8, 0.8, 1.0);



    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);


    cout<<"------------------------------------------------------------"<<endl;
    cout<<"Instruction:"<<endl;
    cout<<"s - On or Off the Axis"
        <<"\nHome - to reset model"
        <<"\nv or V - to display model vertices"
        <<"\nf or F - to display model surfaces"
        <<endl;

    cout<<"------------------------------------------------------------"<<endl;
}

void toggleAxis(int input)
{
    axisornot += input;

    if(axisornot>=2){
        axisornot = 0;
    }

    glutPostRedisplay();
};

void toggleSkin(int input)
{
    displayFaces = !displayFaces;
    glutPostRedisplay();
}

void toggleWireframe(int input)
{
    displayVertices = !displayVertices;
    glutPostRedisplay();
}

void changeName(int input)
{
    cout << name << endl;

}

void loadModel(int input)
{
pathPoints = "data/"+name+".ply";
pathSkeleton = "data/"+name+".skel";

eyeX=0, eyeY=0, eyeZ=1.5, focusX=0, focusY=0, focusZ=0, upX=0, upY=1, upZ=0;

//Labelling.read3DModel(pathPoints.c_str());
    //Labelling.read3DSkeleton(pathSkeleton.c_str());
    glutPostRedisplay();
}



///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

void mainWindow_display(void)
{
    glClearColor(0.8, 0.8, 0.8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers ();
}

void subwindowT_display (void)
{
	//  Notify that we are displaying subwindow 1
	printf ("Drawing SubWindow T contents.\n");

	//  Set background color to blue
	glClearColor(0.0, 0.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

	//  Set drawing color to yellow
    glColor3f (1, 1, 0);

	//  Draw teapot
	//glutWireTeapot (0.5);

	//  Swap front and back buffers
	glutSwapBuffers ();
}

void subwindowT_mouse (int button, int state, int x, int y)
{
	//  Notify that we are clicking on subwindow 1
	printf ("SubWindow T: ");

	switch (button)
	{
		//  Left Button Clicked
		case GLUT_LEFT_BUTTON:

			switch (state)
			{
				//  Pressed
				case GLUT_DOWN:
					printf ("Mouse Left Button Pressed (Down)...\n");
					break;
				//  Released
				case GLUT_UP:
					printf ("Mouse Left Button Released (Up)...\n");
					break;
			}

			break;

		//  Middle Button clicked
		case GLUT_MIDDLE_BUTTON:

			switch (state)
			{
				//  Pressed
				case GLUT_DOWN:
					printf ("Mouse Middle Button Pressed (Down)...\n");
					break;
				//  Released
				case GLUT_UP:
					printf ("Mouse Middle Button Released (Up)...\n");
					break;
			}

			break;

		//  Right Button Clicked
		case GLUT_RIGHT_BUTTON:

			switch (state)
			{
				//  Pressed
				case GLUT_DOWN:
					printf ("Mouse Right Button Pressed (Down)...\n");
					break;
				//  Released
				case GLUT_UP:
					printf ("Mouse Right Button Released (Up)...\n");
					break;
			}

			break;

	}
}

void subwindowT_keyboard (unsigned char key, int x, int y)
{
	//  Notify that an ASCII key is pressed on subwindow 1
	printf ("SubWindow T: ");

	//  Print what key the user is hitting
	printf ("User is hitting the '%c' key.\n", key);
	printf ("ASCII code is %d.\n", key);

	switch (key)
	{
		//  User hits A key
		case 'a':

			break;

		//  User hits Shift + A key
		case 'A':

			break;

		//  User hits Enter
		case '\r':
			printf ("User is hitting the Return key.\n");
			break;

		//  User hits Space
		case ' ':
			printf ("User is hitting the Space key.\n");
			break;

		//  User hits back space
		case 8:
			printf ("User is hitting the Back Space key.\n");
			break;

		//  User hits ESC key
		case 27:
			exit (1);
			break;
	}

	glutPostRedisplay ();
}



void subwindowB_display (void)
{
	//  Notify that we are displaying subwindow 2
	printf ("Drawing SubWindow B contents.\n");

	//  Set background color to yellow
	glClearColor(1.0, 1.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

	//  Set drawing color to blue
    glColor3f (0, 0, 1);

	//  Draw teapot
	//glutWireTeapot (0.5);

	//  Swap front and back buffers
	glutSwapBuffers ();
}

void subwindowB_mouse (int button, int state, int x, int y)
{
	//  Notify that we are clicking on subwindow 2
	printf ("SubWindow B: ");

	switch (button)
	{
		//  Left Button Clicked
		case GLUT_LEFT_BUTTON:

			switch (state)
			{
				//  Pressed
				case GLUT_DOWN:
					printf ("Mouse Left Button Pressed (Down)...\n");
					break;
				//  Released
				case GLUT_UP:
					printf ("Mouse Left Button Released (Up)...\n");
					break;
			}

			break;

		//  Middle Button clicked
		case GLUT_MIDDLE_BUTTON:

			switch (state)
			{
				//  Pressed
				case GLUT_DOWN:
					printf ("Mouse Middle Button Pressed (Down)...\n");
					break;
				//  Released
				case GLUT_UP:
					printf ("Mouse Middle Button Released (Up)...\n");
					break;
			}

			break;

		//  Right Button Clicked
		case GLUT_RIGHT_BUTTON:

			switch (state)
			{
				//  Pressed
				case GLUT_DOWN:
					printf ("Mouse Right Button Pressed (Down)...\n");
					break;
				//  Released
				case GLUT_UP:
					printf ("Mouse Right Button Released (Up)...\n");
					break;
			}

			break;

	}
}

void subwindowB_keyboard (unsigned char key, int x, int y)
{
	//  Notify that an ASCII key is pressed on subwindow 2
	printf ("SubWindow B: ");

	//  Print what key the user is hitting
	printf ("User is hitting the '%c' key.\n", key);
	printf ("ASCII code is %d.\n", key);

	switch (key)
	{
		//  User hits A key
		case 'a':

			break;

		//  User hits Shift + A key
		case 'A':

			break;

		//  User hits Enter
		case '\r':
			printf ("User is hitting the Return key.\n");
			break;

		//  User hits Space
		case ' ':
			printf ("User is hitting the Space key.\n");
			break;

		//  User hits back space
		case 8:
			printf ("User is hitting the Back Space key.\n");
			break;

		//  User hits ESC key
		case 27:
			exit (2);
			break;
	}

	glutPostRedisplay ();
}



void subwindowL_display (void)
{
	//  Notify that we are displaying subwindow 2
	printf ("Drawing SubWindow L contents.\n");

	//  Set background color to yellow
	glClearColor(0.8, 0.8, 0.8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

	//  Set drawing color to blue
    glColor3f (0, 0, 1);

	//  Draw teapot
	//glutWireTeapot (0.5);

	//  Swap front and back buffers
	glutSwapBuffers ();
}

void subwindowL_mouse (int button, int state, int x, int y)
{
	//  Notify that we are clicking on subwindow 2
	printf ("SubWindow L: ");

	switch (button)
	{
		//  Left Button Clicked
		case GLUT_LEFT_BUTTON:

			switch (state)
			{
				//  Pressed
				case GLUT_DOWN:
					printf ("Mouse Left Button Pressed (Down)...\n");
					break;
				//  Released
				case GLUT_UP:
					printf ("Mouse Left Button Released (Up)...\n");
					break;
			}

			break;

		//  Middle Button clicked
		case GLUT_MIDDLE_BUTTON:

			switch (state)
			{
				//  Pressed
				case GLUT_DOWN:
					printf ("Mouse Middle Button Pressed (Down)...\n");
					break;
				//  Released
				case GLUT_UP:
					printf ("Mouse Middle Button Released (Up)...\n");
					break;
			}

			break;

		//  Right Button Clicked
		case GLUT_RIGHT_BUTTON:

			switch (state)
			{
				//  Pressed
				case GLUT_DOWN:
					printf ("Mouse Right Button Pressed (Down)...\n");
					break;
				//  Released
				case GLUT_UP:
					printf ("Mouse Right Button Released (Up)...\n");
					break;
			}

			break;

	}
}

void subwindowL_keyboard (unsigned char key, int x, int y)
{
	//  Notify that an ASCII key is pressed on subwindow 2
	printf ("SubWindow L: ");

	//  Print what key the user is hitting
	printf ("User is hitting the '%c' key.\n", key);
	printf ("ASCII code is %d.\n", key);

	switch (key)
	{
		//  User hits A key
		case 'a':

			break;

		//  User hits Shift + A key
		case 'A':

			break;

		//  User hits Enter
		case '\r':
			printf ("User is hitting the Return key.\n");
			break;

		//  User hits Space
		case ' ':
			printf ("User is hitting the Space key.\n");
			break;

		//  User hits back space
		case 8:
			printf ("User is hitting the Back Space key.\n");
			break;

		//  User hits ESC key
		case 27:
			exit (2);
			break;
	}

	glutPostRedisplay ();
}







///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

void redisplay_all (void)
{
    glutSetWindow(subwinT);
    glutPostRedisplay();
    glutSetWindow(subwinB);
    glutPostRedisplay();
    glutSetWindow(subwinL);
    glutPostRedisplay();
}
///////////////////////////////////////////////////////////////////





















int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition(100, 100); // Set top-left position
    glutInitWindowSize(1280, 720); //Set width and height
    main_window = glutCreateWindow("qwerty");// Create display window

    glutDisplayFunc(mainWindow_display);  // Specify the display callback function
    glutReshapeFunc(myReshapeFunc);
    glutKeyboardFunc(myKeyboardFunc);
    glutSpecialFunc(mySpecialFunc);
    glutMouseFunc(myMouseFunc);
    glutMotionFunc(myMotionFunc);


    //int subwindow1;
    subwinT = glutCreateSubWindow(main_window,250,20,330,330);
    glutDisplayFunc (subwindowT_display);
	glutMouseFunc (subwindowT_mouse);
	glutKeyboardFunc (subwindowT_keyboard);

    subwinB = glutCreateSubWindow(main_window,250,370,330,330);
    glutDisplayFunc (subwindowB_display);
	glutMouseFunc (subwindowB_mouse);
	glutKeyboardFunc (subwindowB_keyboard);

    subwinL = glutCreateSubWindow(main_window,20,20,210,680);
    glutDisplayFunc (subwindowL_display);
	glutMouseFunc (subwindowL_mouse);
	glutKeyboardFunc (subwindowL_keyboard);


    //myInit();

    char buffer[10] = "fuck";
    char *ptr = buffer;
    //GLUI *glui = GLUI_Master.create_glui( ptr, 0, 600, 200 );
    GLUI *gluiT = GLUI_Master.create_glui_subwindow( subwinT, GLUI_SUBWINDOW_TOP );




    //GLUI_Master.set_glutKeyboardFunc( myKeyboardFunc );
    //GLUI_Master.set_glutSpecialFunc( mySpecialFunc );
    //GLUI_Master.set_glutMouseFunc( myMouseFunc );
    //GLUI_Master.set_glutReshapeFunc( myReshapeFunc );
    //GLUI_Master.set_glutIdleFunc( NULL );



    //*//
    //new GLUI_StaticText( glui, "Testing the UI" );
    //new GLUI_Separator( glui );
    //new GLUI_Button( glui, "Toggle Axis", 1,toggleAxis );
    //new GLUI_Button( glui, "Toggle Skin", 1,toggleSkin );
    //new GLUI_Button( glui, "Toggle Wireframe", 1,toggleWireframe );

    //new GLUI_Separator( glui );
    //edittext = new GLUI_EditText( glui, "Model name:", name, 0 );
    //new GLUI_Button( glui, "Load model", 1,loadModel );
    //new GLUI_Separator( glui );

    new GLUI_StaticText (gluiT, "Model 1:");
    GLUI_Rollout *rolloutT = gluiT->add_rollout("Controls", false);
    //GLUI_RadioGroup *rbGroupT = gluiT->add_radiogroup_to_panel(rolloutT);
    //GLUI_RadioButton *rbVertexT = gluiT->add_radiobutton_to_group(rbGroupT, "Show vertices");
    //GLUI_RadioButton *rbFaceT = gluiT->add_radiobutton_to_group(rbGroupT, "Show faces");
    GLUI_Checkbox *checkT = gluiT->add_checkbox_to_panel(rolloutT, "Show faces ");
    gluiT->add_column_to_panel(rolloutT, true);
    GLUI_Button *btnResetViewT = gluiT->add_button_to_panel( rolloutT, "Reset view" );

    //GLUI_Button *quit_btn =  gluiTL->add_button_to_panel( test, "Quit", 0,(GLUI_Update_CB)exit );
    //*//

    GLUI *gluiB = GLUI_Master.create_glui_subwindow( subwinB, GLUI_SUBWINDOW_TOP );
    new GLUI_StaticText (gluiB, "Model 2:");
    GLUI_Rollout *rolloutB = gluiB->add_rollout("Controls", false);
    //GLUI_RadioGroup *rbGroupB = gluiB->add_radiogroup_to_panel(rolloutB);
    //GLUI_RadioButton *rbVertexB = gluiB->add_radiobutton_to_group(rbGroupB, "Show vertices");
    //GLUI_RadioButton *rbFaceB = gluiB->add_radiobutton_to_group(rbGroupB, "Show faces");
    GLUI_Checkbox *checkB = gluiB->add_checkbox_to_panel(rolloutB, "Show faces ");
    gluiB->add_column_to_panel(rolloutB, true);
    GLUI_Button *btnResetViewB = gluiB->add_button_to_panel( rolloutB, "Reset view" );


    GLUI *gluiL = GLUI_Master.create_glui_subwindow( subwinL, GLUI_SUBWINDOW_TOP );
    new GLUI_StaticText(gluiL, "3D Blending");
    gluiL->add_separator();
    GLUI_Listbox *listboxL = gluiL->add_listbox("Model pairs: ");
    listboxL->add_item(0, "Human hand");
    listboxL->add_item(1, "Model 2");
    listboxL->add_item(2, "Model 3");
    gluiL->add_separator();
    new GLUI_StaticText(gluiL, "View controls:");
    new GLUI_StaticText(gluiL, "Left mouse drag to rotate.");
    new GLUI_StaticText(gluiL, "Right mouse drag to zoom.");
    new GLUI_StaticText(gluiL, "Middle mouse drag to pan.");
    gluiL->add_separator();
    new GLUI_StaticText(gluiL, "Model 1:");
    new GLUI_StaticText(gluiL, "### surface vertices");
    new GLUI_StaticText(gluiL, "### faces");
    new GLUI_StaticText(gluiL, "### skeleton vertices");
    new GLUI_StaticText(gluiL, "### skeleton edges");
    gluiL->add_separator();
    new GLUI_StaticText(gluiL, "Model 2:");
    new GLUI_StaticText(gluiL, "### surface vertices");
    new GLUI_StaticText(gluiL, "### faces");
    new GLUI_StaticText(gluiL, "### skeleton vertices");
    new GLUI_StaticText(gluiL, "### skeleton edges");
    gluiL->add_separator();
    new GLUI_StaticText(gluiL, "Blended model:");
    new GLUI_StaticText(gluiL, "### surface vertices");
    new GLUI_StaticText(gluiL, "### faces");
    new GLUI_StaticText(gluiL, "### skeleton vertices");
    new GLUI_StaticText(gluiL, "### skeleton edges");

    //glui_subwindow->set_main_gfx_window( main_window );

    gluiT->set_main_gfx_window( main_window );
    gluiB->set_main_gfx_window( main_window );


    glutMainLoop();
    return 0;
}














