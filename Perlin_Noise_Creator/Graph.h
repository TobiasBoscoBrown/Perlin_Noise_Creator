#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

class Graph
{
private:
    typedef struct Pixel {
        int red, green, blue;
        float distanceVectorTR[2], distanceVectorTL[2], distanceVectorBR[2], distanceVectorBL[2];
        float dotProd1, dotProd2, dotProd3, dotProd4;
        float inter1, inter2, value;
    }*PixelPtr;

    typedef struct Node {
        int xPos, yPos;
        int gradientVector[2];
        bool isCorner;
        Node* left;
        Node* right;
        Node* up;
        Node* down;
        Pixel* toPixel;
    }*NodePtr;

    NodePtr curr = NULL;
    NodePtr temp = NULL;
    NodePtr root = NULL;
    NodePtr tail = NULL;
    NodePtr newLineNode = root;
    NodePtr currAboveNode;
    NodePtr currCorner;
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
    void exportImageData(vector<unsigned char>& imageData);
};
