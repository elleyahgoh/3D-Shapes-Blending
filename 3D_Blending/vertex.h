#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <vector>
using namespace std;
typedef struct SR_Vertex {   //SR = Surface Reconstruction//
	double v[3];             //v[3] => coordinate of a vertex/point
}
SR_Vertex;

typedef struct SR_Triangle {
    int tv[3];                 //tv[3]  => 3 vertices that form a triangle
    vector<float> normals;
}
SR_Triangle;

typedef struct SV_VertexSkel {   //SV = Skeleton vertices
    double sv[3];                //sv[3] => coordinate of a skeleton vertex/point
}
SV_VertexSkel;

typedef struct SE_EdgeSkel {   //SE = Skeleton edge
    int se[2];              //se[2] => two indices that form an edge
}
SE_EdgeSkel;

/*//
typedef struct SEG_Color
{
    float sc[3];
}
SEG_Color;
//*///

//*//
typedef struct Point3f
{
    float c[3];
}
Point3f;
//*///
class vertex
{
    public:
    vertex();
    bool read3DModel(const char *filename);
    bool read3DSkeleton(const char *filename);
    bool readPLY(const char *filename);
    bool readSKEL(const char *filename);
    float CheckDist(int, int);
    void showPoint(float, float, float, float, bool, bool, bool);

    int noOfVertices;
    int noOfFaces;
    int countSV; //count skeleton vertices
    int countSE; //count skeleton edges

	double min[3], max[3]; //min and max for model

    SR_Vertex *sr_verts, *sr_selected_verts;
	SR_Triangle *tri_triags, *tri_selected_triags;
	SV_VertexSkel *sv_vertSkel;
	SE_EdgeSkel *se_edgeSkel;

	void DrawAxis();
    void DisplayVertices();
    void DisplayFaces(float);
    void DisplaySkelEdge();
    void DisplaySkel();
    void DisplaySegmentFaces();
    void DisplaySpecificSegmentFaces(int WhichSegment);

    double getX(int index);
	double getY(int index);
    double getZ(int index);

    vector<float> calculateNormal( float *coord1, float *coord2, float *coord3 );
    vector<float> SkelVertices;
    vector<int> SkelEdges;

    vector<int> neighbor;
    vector<vector<int> > skelTwoDNeighbor;



    vector<int> segmentID;
    int segMin, segMax, segNum;

    bool readSegment(const char *filename);
    bool readSEG(const char *filename);

    //SEG_Color *seg_colors;
    void setSegmentColor(int segID = -1);
    int checkEdgeSegment(int skelID1,int skelID2);

    vector<int> segment;
    vector<vector<int> > vectorOfSegments;
    bool readSkelSegment(const char* filename);

    int highlightedSegment;

    //*//


    void debug();
    //*///

   // mouse click related
   int selectedPart;
   int selectedFace;





    vector<int> linkBaseVertices;
    vector<vector<int> > vectorOfLinkBaseVertices;
    void populateLinkBaseVector();


    const int ENUM_X = 0, ENUM_Y = 1, ENUM_Z = 2;

    vector<int> vectorOfIdxOfBlendedSegment;
    vector<vector<Point3f> > vectorOfBlendedSegments;
    bool readBlendedSegment(const char*);
    void deleteOldSegments();
    void drawNewSegments();


    bool searchSegmentExisting(int);

    void prepareBlended();

    void DisplayPartialBlendedSkel();

    void DisplaySelectedFeature(int WhichSegment);
    void DisplayUnselectedFeature(int WhichSegment);
    void MeasureFeatureModel(float arrayXYZ[], int ArraySize, int WhichSegment, int &counterModel);
    void Blend(vertex model1, vertex model2, float model_1_ArrayXYZ[], int WhichSegment, float model_2_ArrayXYZ[], int WhichSegmentRemove);
    void DisplayRemaining(int WhichSegment);

};
#endif // VERTEX_H_INCLUDED
