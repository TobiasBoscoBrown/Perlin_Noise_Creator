#include <fstream>    // For file operations (ofstream)
#include <cstdlib>    // For general-purpose functions (rand, srand, abs)
#include <ctime>      // For time-related functions (time)
#include <iostream>   // For standard input and output streams (cout, endl)
#include <cmath>      // For mathematical functions (sin, abs, pow)
#include <zlib.h>     // For zlib functions (deflate, crc32)
#include "Graph.h"    // Custom header file for the Graph class

using namespace std;  // Use the standard namespace


typedef unsigned char u8;
typedef unsigned int u32;

// Function to compress raw image data into PNG format
u8* CompressPNG(void *img, int w, int h, int numchans, u32 &len_out) {
    int p = w * numchans;
    z_stream z = {0}; 
    deflateInit(&z, -1);    
    u8 *zbuf = (u8*)malloc(53 + (z.avail_out = deflateBound(&z, (1 + p) * h)) + 1);    
    if (!zbuf) return 0;
    z.next_out = zbuf + 41;    
    for (int y = 0; y < h; ++y) {
        Bytef zero = 0; 
        z.avail_in = 1; 
        z.next_in = &zero; 
        deflate(&z, Z_NO_FLUSH);
        z.avail_in = p; 
        z.next_in = ((Bytef*)img) + y * p; 
        deflate(&z, (y == h - 1) ? Z_FINISH : Z_NO_FLUSH);
    }
    len_out = z.next_out - zbuf - 41;
    u8 pnghdr[41] = {
        0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 
        0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
        0, 0, w >> 8, w, 0, 0, h >> 8, h, 8, "\0\0\04\02\06"[numchans], 0, 0, 0, 0, 0, 0, 0,
        len_out >> 24, len_out >> 16, len_out >> 8, len_out, 0x49, 0x44, 0x41, 0x54
    };
    *(u32*)(pnghdr + 29) = htonl(crc32(0, pnghdr + 12, 17));    
    memcpy(zbuf, pnghdr, 41); 
    memcpy(z.next_out + 4, "\x49\x45\x4e\x44\xae\x42\x60\x82", 8); 
    *(u32*)z.next_out = htonl(crc32(0, zbuf + 41 - 4, len_out + 4));
    deflateEnd(&z); 
    len_out += 53; 
    return zbuf;
}

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

    // Perform the specified number of cuts in the graph
    pic.cutGraph();

    // Define distance vectors for the nodes in the graph
    pic.defineDistVect();

    // Create an image buffer to store the pixel data
    u32 img[1000][1000];

    // Open a file to write the image data in PPM format
    ofstream ppmFile("picture.ppm");
    ppmFile << "P3" << endl;                   // PPM file format identifier
    ppmFile << width - 1 << " " << height - 1 << endl; // Image dimensions (width-1) x (height-1)
    ppmFile << "255" << endl;                  // Maximum color value

    // Output the graph data to the PPM file and fill the image buffer
    for (int y = 0; y < height - 1; ++y) {
        for (int x = 0; x < width - 1; ++x) {
            u32 r, g, b;
            pic.getPixelColor(x, y, r, g, b); // Get the color of the pixel from the graph
            ppmFile << r << " " << g << " " << b << " "; // Write the pixel color to the PPM file
            img[y][x] = (0xff << 24) | (r << 16) | (g << 8) | b; // Store the pixel color in the image buffer
        }
        ppmFile << endl;
    }

    // Close the PPM file
    ppmFile.close();

    // Compress the image buffer to PNG format
    u32 pngLength;
    u8 *pngData = CompressPNG(img, width - 1, height - 1, 4, pngLength);

    // Write the compressed PNG data to a file
    FILE *pngFile = fopen("picture.png", "wb");
    fwrite(pngData, 1, pngLength, pngFile);
    fclose(pngFile);

    // Free the allocated memory for the PNG data
    free(pngData);

    // Program finished successfully
    return 0;
}
