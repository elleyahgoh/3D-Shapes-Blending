#include "vertex.h"
#include <stdio.h>
#include <string.h>
#include <GL/glu.h>
#include <iostream>
#include <math.h>
using namespace std;
vertex::vertex()
{
    noOfVertices = 0;
    noOfFaces = 0;
}
//----------------------------------------------------------------------
bool vertex::read3DModel(const char *filename)
{
	bool succ = false;

	if (strstr(filename, ".ply")!=NULL)
		succ = readPLY(filename);
	return succ;
}
//----------------------------------------------------------------------
bool vertex::readPLY(const char *filename)
{
	ifstream readIn(filename,ios::in);
	char line[256];
	readIn.getline(line, 256);

	//Read the header
	while (strcmp(line, "end_header")!=0)
	{
		stringstream ss;
		char elm[256];

		ss << line;
		ss >> elm;

		if (strcmp(elm,"element")==0)
		{
			char next[10];
			ss >> next;
			if (strcmp(next, "vertex")==0)
			{
				ss >> noOfVertices;
			}
			else if (strcmp(next, "face")==0)
			{
				ss >> noOfFaces;
			}
			else
			{
				exit(0);
			}
		}
		readIn.getline(line, 999);
	}

    //cout<<"No-Of-Vertices: "<<noOfVertices<<" \nNo-Of-Faces: "<<noOfFaces<<endl;
	sr_verts = new SR_Vertex[noOfVertices];
	//SurfaceReconstruction, array of struct that has an array of 3[x,y,z]

	static double minX=999, maxX=-999, minY=999, maxY=-999, minZ=999, maxZ=-999;
	//Read the data
	int i;
	for(i=0; i<noOfVertices; i++)
	{
		readIn >> sr_verts[i].v[0];
		readIn >> sr_verts[i].v[1];
		readIn >> sr_verts[i].v[2];

		//float dummy;
		//readIn >> dummy;

		if(minX>sr_verts[i].v[0])
            minX = sr_verts[i].v[0];
        if(maxX<sr_verts[i].v[0])
            maxX = sr_verts[i].v[0];
        if(minY>sr_verts[i].v[1])
            minY = sr_verts[i].v[1];
        if(maxY<sr_verts[i].v[1])
            maxY = sr_verts[i].v[1];
        if(minZ>sr_verts[i].v[2])
            minZ = sr_verts[i].v[2];
        if(maxZ<sr_verts[i].v[2])
            maxZ = sr_verts[i].v[2];
	}
	int dummy;
	tri_triags = new SR_Triangle[noOfFaces];
	//array of struct with array of 3 index and c++vector of normals?

    float x1,y1,z1;
    float x2,y2,z2;
    float x3,y3,z3;
	for(i=0; i<noOfFaces;i++)
	{
	    readIn >> dummy;
	    readIn >> tri_triags[i].tv[0];
		readIn >> tri_triags[i].tv[1];
		readIn >> tri_triags[i].tv[2];

		// Calculate normals
        x1 = sr_verts[tri_triags[i].tv[0]].v[0];
        y1 = sr_verts[tri_triags[i].tv[0]].v[1];
        z1 = sr_verts[tri_triags[i].tv[0]].v[2];

        x2 = sr_verts[tri_triags[i].tv[1]].v[0];
        y2 = sr_verts[tri_triags[i].tv[1]].v[1];
        z2 = sr_verts[tri_triags[i].tv[1]].v[2];

        x3 = sr_verts[tri_triags[i].tv[2]].v[0];
        y3 = sr_verts[tri_triags[i].tv[2]].v[1];
        z3 = sr_verts[tri_triags[i].tv[2]].v[2];

        float coord1[3] = {x1, y1, z1};
        float coord2[3] = {x2, y2, z2};
        float coord3[3] = {x3, y3, z3};

        tri_triags[i].normals = this->calculateNormal(coord1, coord2, coord3);
	}
//    cout<<"Min (X, Y, Z): ( "<<minX<<", "<<minY<<", "<<minZ<<" )"
//        <<"\nMax (X, Y, Z): ( "<<maxX<<", "<<maxY<<", "<<maxZ<<" )"<<endl;

    min[0] = minX, min[1] = minY, min[2] = minZ;
    max[0] = maxX, max[1] = maxY, max[2] = maxZ;
    readIn.close();
	return true;
}
//------------------------------------------------------------------------
bool vertex::read3DSkeleton(const char *filename)
{
	bool succ = false;

	if (strstr(filename, ".skel")!=NULL)
		succ = readSKEL(filename);
	return succ;
}
//-----------------------------------------------------------------------
bool vertex::readSKEL(const char *filename)
{
	int i;
	ifstream readIn(filename);
    string type;
    float sx, sy, sz, e1, e2;
    readIn>>type;
	countSV=0, countSE=0;
	vector<float> dummySV;
	vector<int> dummySE;
	vector<float>::iterator itr;
	vector<int>::iterator itrEdge, itrNeighbor;

	while(type == "sv")
	{
        readIn >>sx>>sy>>sz;
        dummySV.push_back(sx);
        dummySV.push_back(sy);
        dummySV.push_back(sz);
		countSV++;
		readIn>>type;
	}
	//cout<<"count Skeleton Vertices: "<<countSV<<endl;
	while(!readIn.eof() && type == "se")
	{
        readIn >>e1>>e2;
        dummySE.push_back(e1);
        dummySE.push_back(e2);
		countSE++;
		readIn>>type;
	}
	//cout<<"count Skeleton Edges: "<<countSE<<endl;
	sv_vertSkel = new SV_VertexSkel[countSV]; //sv_vertSkel = pointer to struct
	i=0;
	for(itr=dummySV.begin(); itr<dummySV.end(); itr=itr+3)//Take note: Skeleton starts at 1, not 0
	{
		sv_vertSkel[i].sv[0]=*itr;
		sv_vertSkel[i].sv[1]=*(itr+1);
		sv_vertSkel[i++].sv[2]=*(itr+2);
	}
    for(i=0; i<countSV; i++)//Take note: Skeleton starts at 1, not 0
	{
	    neighbor.push_back(i);
	    skelTwoDNeighbor.push_back(neighbor);
	    neighbor.clear();
	}
	cout<<endl;
	se_edgeSkel = new SE_EdgeSkel[countSE]; //why originally countSV?
	i=0;
	for(itrEdge=dummySE.begin(); itrEdge<dummySE.end(); itrEdge=itrEdge+2)
	{
		se_edgeSkel[i].se[0]=*itrEdge;
		se_edgeSkel[i].se[1]=*(itrEdge+1);
        i++;
	}
	for(i=0; i<countSE; i++)//Take note: Skeleton starts at 1, not 0
	{
	    //cout<<i<<"  "<<se_edgeSkel[i].se[0]<<"  "<<se_edgeSkel[i].se[1]<<"  "<<skelTwoDNeighbor.size()<<endl;

		skelTwoDNeighbor[se_edgeSkel[i].se[0]].push_back(se_edgeSkel[i].se[1]);
        skelTwoDNeighbor[se_edgeSkel[i].se[1]].push_back(se_edgeSkel[i].se[0]);
        //getchar();
	}
    readIn.close();
	return true;
}
//------------------------------------------------------------------------
bool vertex::readSegment(const char *filename)
{
	bool succ = false;

	if (strstr(filename, ".seg")!=NULL)
		succ = readSEG(filename);
	return succ;
}

bool vertex::readSEG(const char *filename)
{
    ifstream instream(filename, ios::in);
    int segID;
    segMin = 999;
    segMax = -999;

    for(int i = 0; i < noOfVertices; i++)
    {
        instream >> segID;
        segmentID.push_back(segID);
        if(segID < segMin)
            segMin = segID;
        if(segID > segMax)
            segMax = segID;
    }

    for(int i = 0; i < noOfVertices; i++)
    {
        segmentID[i] -= segMin; //normalise from 0 to segMax - 1;
    }

    instream.close();
    return true;
}

void vertex::setSegmentColor(int segID)
{
    //temporarily hardcode color, due avoid random recreating same colours
    switch(segID)
    {
        case 0:
            glColor3f(0.0,0.0,1.0);
            break;
        case 1:
            glColor3f(0.0,1.0,0.0);
            break;
        case 2:
            glColor3f(0.0,1.0,1.0);
            break;
        case 3:
            glColor3f(1.0,0.0,0.0);
            break;
        case 4:
            glColor3f(1.0,0.0,1.0);
            break;
        case 5:
            glColor3f(1.0,1.0,0.0);
            break;
        case 6:
            glColor3f(0.0,0.5,1.0);
            break;
        case 7:
            glColor3f(1.0,0.5,0.0);
            break;
        case 8:
            glColor3f(1.0,0.0,0.5);
            break;
        case 9:
            glColor3f(0.5,0.0,1.0);
            break;
        default:
            glColor3f(0.5,0.5,0.5);
            break;
    }
}

//------------------------------------------------------------------------
void vertex::DrawAxis()
{
    //glTranslatef(center[0], center[1], center[2]);
    glBegin(GL_LINES);
        //x-axis
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(10.0, 0.0, 0.0);
        //x-axis arrow
        glVertex3f(10.0, 0.0,  0.0);
        glVertex3f(9.0, 1.0, 0.0);
        glVertex3f(10.0, 0.0,  0.0);
        glVertex3f(9.0, -1.0, 0.0);

        //y-axis
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0.0,  0.0, 0.0);
        glVertex3f(0.0, 10.0, 0.0);
        //y-axis arrow
        glVertex3f(0.0, 10.0, 0.0);
        glVertex3f(1.0, 9.0, 0.0);
        glVertex3f(0.0, 10.0, 0.0);
        glVertex3f(-1.0, 9.0, 0.0);

        //z-axis
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0.0, 0.0,  0.0);
        glVertex3f(0.0, 0.0, 10.0);
        //z-axis arrow
        glVertex3f(0.0, 0.0, 10.0);
        glVertex3f(0.0, 1.0, 9.0);
        glVertex3f(0.0, 0.0, 10.0);
        glVertex3f(0.0, -1.0, 9.0);
     glEnd();
}
//------------------------------------------------------------------------
void vertex::DisplayVertices()
{
    //int i;
    //glColor3f(0.5,0.5,0.5);

    //glColor3f(1.0,1.0,1.0);
    glPointSize(1.0);
    glBegin(GL_POINTS);
    for(int i=0; i<noOfVertices; i++)
    {
        segmentID.size() > 0 ? setSegmentColor(segmentID[i]):setSegmentColor();
        glVertex3d(sr_verts[i].v[0],sr_verts[i].v[1],sr_verts[i].v[2]);
    }
    glEnd();
}
//---------------------------------------------------------------------

void vertex::DisplaySkel()
{
    glPointSize(5.0);
	glColor3f(0,0,0);
	glBegin(GL_POINTS);
	for(int i=0; i<countSV; i++)
    {
        glVertex3f(sv_vertSkel[i].sv[0], sv_vertSkel[i].sv[1], sv_vertSkel[i].sv[2]);

    }
    glEnd();
    glColor3f(0,0,1);
    glLineWidth(1.0);
	glBegin(GL_LINES);
	for(int i=0; i<countSE; i++)
    {
        glVertex3f(sv_vertSkel[se_edgeSkel[i].se[0]].sv[0], sv_vertSkel[se_edgeSkel[i].se[0]].sv[1], sv_vertSkel[se_edgeSkel[i].se[0]].sv[2]);
        glVertex3f(sv_vertSkel[se_edgeSkel[i].se[1]].sv[0], sv_vertSkel[se_edgeSkel[i].se[1]].sv[1], sv_vertSkel[se_edgeSkel[i].se[1]].sv[2]);
    }
    glEnd();
}

//--------------------------------------------------------------------
void vertex::DisplayFaces(float transparent)
{
    float x1,y1,z1;
    float x2,y2,z2;
    float x3,y3,z3;

    float nx, ny, nz;


    glBegin(GL_TRIANGLES);
    for(int i=0;i<noOfFaces;i++){
        x1 = sr_verts[tri_triags[i].tv[0]].v[0];
        y1 = sr_verts[tri_triags[i].tv[0]].v[1];
        z1 = sr_verts[tri_triags[i].tv[0]].v[2];

        x2 = sr_verts[tri_triags[i].tv[1]].v[0];
        y2 = sr_verts[tri_triags[i].tv[1]].v[1];
        z2 = sr_verts[tri_triags[i].tv[1]].v[2];

        x3 = sr_verts[tri_triags[i].tv[2]].v[0];
        y3 = sr_verts[tri_triags[i].tv[2]].v[1];
        z3 = sr_verts[tri_triags[i].tv[2]].v[2];

        nx = tri_triags[i].normals[0];
        ny = tri_triags[i].normals[1];
        nz = tri_triags[i].normals[2];

        glNormal3f(nx,ny,nz);
        //glColor3f(nx,ny,nz);
        //glColor3f(0.5,0.5,0.5); //gray color
        segmentID.size() > 0 ? setSegmentColor(segmentID[tri_triags[i].tv[0]]):setSegmentColor();
        //setSegmentColor(segmentID[tri_triags[i].tv[0]]);
        //glColor4f(0.5,0.5,0.5,transparent);
        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y2, z2);
        glVertex3f(x3, y3, z3);
    }
    glEnd();
}
//-------------------------------------------------------------------------
double vertex::getX(int index)
{
    return sr_verts[index].v[0];
}
//-------------------------------------------------------------------------
double vertex::getY(int index)
{
    return sr_verts[index].v[1];
}
//-------------------------------------------------------------------------
double vertex::getZ(int index)
{
    return sr_verts[index].v[2];
}
//--------------------------------------------------------------------------
vector<float> vertex::calculateNormal( float *coord1, float *coord2, float *coord3 )
{
   /* calculate Vector1 and Vector2 */
   float va[3], vb[3], vr[3], val;
   va[0] = coord1[0] - coord2[0];
   va[1] = coord1[1] - coord2[1];
   va[2] = coord1[2] - coord2[2];

   vb[0] = coord1[0] - coord3[0];
   vb[1] = coord1[1] - coord3[1];
   vb[2] = coord1[2] - coord3[2];

   /* cross product */
   vr[0] = va[1] * vb[2] - vb[1] * va[2];
   vr[1] = vb[0] * va[2] - va[0] * vb[2];
   vr[2] = va[0] * vb[1] - vb[0] * va[1];

   /* normalization factor */
   val = sqrt( vr[0]*vr[0] + vr[1]*vr[1] + vr[2]*vr[2] );

	vector<float> norm;
	norm.push_back(vr[0]/val);
	norm.push_back(vr[1]/val);
	norm.push_back(vr[2]/val);


	return norm;
}
