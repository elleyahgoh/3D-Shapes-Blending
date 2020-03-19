#include<iostream>
#include<fstream>
using namespace std;

//reminder to self
//change filename and number of vertex!!!
//modify .ply input by removing header and faces
//readd header and faces in the output .ply

string infile = "LongHand_vert.ply";
string outfile = "LongHand_temp.ply";
const int lineNum = 7242 * 3;

int main()
{
	ifstream instream(infile.c_str(), ios::in);
	char line[256];
	
	float vert[lineNum];
	
	float a,b,c,d;
	
	for(int i = 0; i < lineNum; i++)
	{
		instream >> a;
		instream >> b;
		instream >> c;
		instream >> d;
		
		//cout <<a<<":"<<b<<":"<<c<<":"<<d<<endl;
		vert[i] = a;
		vert[++i] = b;
		vert[++i] = c;
	}
	instream.close();
	
	
	//*
	ofstream outstream(outfile.c_str(), ios::out);
	for(int i = 0; i < lineNum; i+=3)
	{
		/*//
		cout << vert[i] << ":"
			 << vert[i+1] << ":"
			 << vert[i+2] << ":"
			 << endl;
			 //*///
		outstream << vert[i] << " "
				  << vert[i+1] << " "
				  << vert[i+2] << endl;
	}
	outstream.close();
	//*///
}








