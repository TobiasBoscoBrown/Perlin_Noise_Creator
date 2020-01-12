#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;
class Graph
{
private:
	typedef struct pixel {
		int red, green, blue;
		float distanceVectorTR[2], distanceVectorTL[2], distanceVectorBR[2], distanceVectorBL[2];
		float dotProd1, dotProd2, dotProd3, dotProd4;
		float inter1, inter2, value;
	}*pixelPtr;

	typedef struct node {
		int xPos, yPos;
		int gradientVector[2];
		bool isCorner;
		node* left;
		node* right;
		node* up;
		node* down;
		pixel* toPixel;
	}*nodePtr;

	nodePtr curr = NULL;
	nodePtr temp = NULL;
	nodePtr root = NULL;
	nodePtr tail = NULL;
	nodePtr newLineNode = root;
	nodePtr currAboveNode;
	nodePtr currCorner;
	int width, height, cut;
public:
	void setWidth(int w);
	void setHeight(int h);
	void setCut(int c);

	int getWidth();
	int getCut();
	int getHeight();
	int simpInt(int i);
	
	void addNode(int x, int y);
	void cutGraph();
	void defineDistVect();
	void printNodes();
	void printPixels();
	void printCorners();
	void outputGraph(ofstream&);
};