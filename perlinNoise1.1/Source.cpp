#include <fstream>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include "Graph.h"

using namespace std;


int noise(float data, int range);
int main() {
		
	int width = 1001, height = 1001; // this is the length and width of nodes, there will be length-1 * width -1 pixels
	int numCuts = 100;

	srand(time(0));

	Graph pic;
	pic.setWidth(width);
	pic.setHeight(height);
	pic.setCut(numCuts);

	ofstream img ("picture.ppm");
	img << "P3" << endl;
	img << width - 1 << " " << height - 1<< endl;
	img << "255" << endl;


	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {

			pic.addNode(x, y);
		}
	}
	pic.cutGraph();
	//pic.printNodes();
	cout << endl;
	//pic.printPixels();
	cout << endl;
	//pic.printCorners();
	cout << endl;
	pic.defineDistVect();
	pic.outputGraph(img);

	return 0;
}

int noise(float data, int range) {
	int noiseSeed = time(0) % 100;
	


	/*data = data + noiseSeed;
	data = abs(sin(data));
	data = data * range;*/

	int a = data;
	return a;
}
