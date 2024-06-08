#include <iostream>   // For standard input and output streams (cout, endl)
#include <sstream>    // For stringstream operations
#include "Graph.h"    // Custom header file for the Graph class
#include <cmath>      // For mathematical functions (sin, abs, pow)
#include <fstream>    // For file operations (ofstream)
#include <cstdlib>    // For general purpose functions (rand, srand, abs)
#include <time.h>     // For time-related functions (time)
using namespace std;  // Use the standard namespace

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"  // Include the stb_image_write header file

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

    // Add nodes to the graph for each coordinate (x, y)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pic.addNode(x, y); // Add a node at coordinates (x, y)
        }
    }

    cout << "Nodes added." << endl;

    // Perform the specified number of cuts in the graph
    pic.cutGraph();

    cout << "Graph cut." << endl;

    // Define distance vectors for the nodes in the graph
    pic.defineDistVect();

    cout << "Distance vectors defined." << endl;

    // Prepare the image data for PNG output
    vector<unsigned char> imageData;
    pic.exportImageData(imageData);

    cout << "Image data exported." << endl;

    // Get the current time for timestamp
    time_t now = time(0);
    tm *ltm = localtime(&now);

    // Format the timestamp as YYYYMMDD_HHMMSS
    stringstream ss;
    ss << 1900 + ltm->tm_year;
    ss << 1 + ltm->tm_mon;
    ss << ltm->tm_mday << "_";
    ss << ltm->tm_hour;
    ss << ltm->tm_min;
    ss << ltm->tm_sec;

    // Create the filename with the timestamp and version number
    string filename = "example/picture_v1_" + ss.str() + ".png";

    // Ensure the example directory exists (create it if not)
    system("mkdir -p example");

    // Write the image data to a PNG file
    int imgWidth = width - 1;
    int imgHeight = height - 1;
    stbi_write_png(filename.c_str(), imgWidth, imgHeight, 3, imageData.data(), imgWidth * 3);

    cout << "Image saved to " << filename << endl;

    // Program finished successfully
    return 0;
}
