#include <fstream>    // For file operations (ofstream)
#include <cstdlib>    // For general purpose functions (rand, srand, abs)
#include <time.h>     // For time-related functions (time)
#include <iostream>   // For standard input and output streams (cout, endl)
#include "Graph.h"    // Custom header file for the Graph class
#include <cmath>      // For mathematical functions (sin, abs, pow)
using namespace std;  // Use the standard namespace

int main() {
    // Initialize the width and height of the graph (grid) to 1001 x 1001 nodes
    int width = 1001, height = 1001; // This results in (width-1) * (height-1) pixels
    int numCuts = 100; // Number of cuts to be made in the graph

    // Seed the random number generator with the current time
    srand(time(0));

    // Create an instance of the Graph class
    Graph pic;
    pic.setWidth(width);   // Set the width of the graph
    pic.setHeight(height); // Set the height of the graph
    pic.setCut(numCuts);   // Set the number of cuts in the graph

    // Open a file to write the image data in PPM format
    ofstream img("picture.ppm");
    img << "P3" << endl;                   // PPM file format identifier
    img << width - 1 << " " << height - 1 << endl; // Image dimensions (width-1) x (height-1)
    img << "255" << endl;                  // Maximum color value

    // Add nodes to the graph for each coordinate (x, y)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pic.addNode(x, y); // Add a node at coordinates (x, y)
        }
    }
	pic.printCorners();

    // Perform the specified number of cuts in the graph
    pic.cutGraph();

    // Define distance vectors for the nodes in the graph
    pic.defineDistVect();

    // Output the graph data to the PPM file
    pic.outputGraph(img);

    // Close the PPM file
    img.close();

    // Program finished successfully
    return 0;
}
