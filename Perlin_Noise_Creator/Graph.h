#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include "stb_image_write.h"
using namespace std;

// Define the Graph class
class Graph {
private:
    // Define a struct for pixel data
    typedef struct pixel {
        int red, green, blue;               // RGB color values
        float distanceVectorTR[2],          // Distance vectors for different corners
              distanceVectorTL[2], 
              distanceVectorBR[2], 
              distanceVectorBL[2];
        float dotProd1, dotProd2, dotProd3, dotProd4; // Dot products for interpolation
        float inter1, inter2, value;       // Intermediate values and final value for the pixel
    } *pixelPtr; // Pointer to a pixel struct

    // Define a struct for node data
    typedef struct node {
        int xPos, yPos;                    // Position of the node
        int gradientVector[2];             // Gradient vector for the node
        bool isCorner;                     // Flag to indicate if the node is a corner
        node* left;                        // Pointer to the left neighbor
        node* right;                       // Pointer to the right neighbor
        node* up;                          // Pointer to the upper neighbor
        node* down;                        // Pointer to the lower neighbor
        pixel* toPixel;                    // Pointer to the corresponding pixel
    } *nodePtr; // Pointer to a node struct

    nodePtr curr = NULL;         // Current node pointer
    nodePtr temp = NULL;         // Temporary node pointer
    nodePtr root = NULL;         // Root node pointer
    nodePtr tail = NULL;         // Tail node pointer
    nodePtr newLineNode = root;  // New line node pointer
    nodePtr currAboveNode;       // Current node above the current one
    nodePtr currCorner;          // Current corner node pointer
    int width, height, cut;      // Width, height, and cut properties of the graph

public:
    // Setters
    void setWidth(int w);        // Set the width of the graph
    void setHeight(int h);       // Set the height of the graph
    void setCut(int c);          // Set the cut property of the graph

    // Getters
    int getWidth();              // Get the width of the graph
    int getCut();                // Get the cut property of the graph
    int getHeight();             // Get the height of the graph
    int simpInt(int i);          // Simplify an integer value (implementation not provided)

    // Functions to manipulate the graph
    void addNode(int x, int y);  // Add a node at position (x, y)
    void cutGraph();             // Cut the graph
    void defineDistVect();       // Define the distance vectors for nodes
    void printNodes();           // Print all nodes in the graph
    void printPixels();          // Print all pixels in the graph
    void printCorners();         // Print all corner nodes in the graph
    void outputGraph(ofstream&); // Output the graph to a file
};
