#include<iostream>
#include<fstream>
using namespace std;

//because it's harder to do so by hand, also not too easy to fully automate
//change input file and number of vertices
//modify .ply by removing header and faces

string infile = "LongHand_vert.ply";
string outfile = "LongHand.seg";
const int lineNum = 7242;

int seg[lineNum];

int main()
{
	ifstream instream(infile.c_str(), ios::in);
	char line[256];
	//instream.getline(line, 256);
	
	float a,b,c,d;
	
	for(int i = 0; i < lineNum; i++)
	{
		instream >> a;
		instream >> b;
		instream >> c;
		instream >> d;
		
		//cout <<a<<":"<<b<<":"<<c<<":"<<d<<endl;
		seg[i] = d;
	}
	instream.close();
	
	ofstream outstream(outfile.c_str(), ios::out);
	for(int i = 0; i < lineNum; i++)
	{
		//cout << seg[i] << endl;
		outstream<<seg[i]<<endl;
	}
	outstream.close();
	
}