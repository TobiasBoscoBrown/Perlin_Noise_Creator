#include "Graph.h"     // Include the header file for the Graph class
#include <string>      // For standard string operations
#include <iostream>    // For standard input and output streams
#include <cmath>       // For mathematical functions like pow

using namespace std;   // Use the standard namespace

// Function to add a node to the graph at position (x, y)
void Graph::addNode(int x, int y) {
    nodePtr n = new node; // Create a new node

    n->right = NULL;     // Initialize right pointer to NULL
    n->down = NULL;      // Initialize down pointer to NULL
    n->xPos = x;         // Set the x position of the node
    n->yPos = y;         // Set the y position of the node

    if (root != NULL) { // If the root is not NULL (graph is not empty)
        if (n->xPos == 0) { // If the node is at the beginning of a new line
            newLineNode->down = n; // Link the node to the node above it
            n->up = newLineNode;   // Link the node above to this node
            currAboveNode = newLineNode; // Set the current above node
            newLineNode = newLineNode->down; // Move to the next line
            tail = n;              // Update the tail pointer
        }
        else if (n->xPos < width) { // If the node is within the width of the graph
            tail->right = n;       // Link the node to the node on the left
            n->left = tail;        // Link the node on the left to this node
            if (n->yPos > 0 && n->yPos != height) { // If the node is within the height of the graph
                pixelPtr p = new pixel; // Create a new pixel
                currAboveNode->toPixel = p; // Link the pixel to the node above
                p->red = 10;         // Set initial red value
                p->green = 10;       // Set initial green value
                p->blue = 201;       // Set initial blue value
                currAboveNode = currAboveNode->right; // Move to the next node above
                n->up = currAboveNode; // Link the node above to this node
                currAboveNode->down = n; // Link this node to the node above
            }
            tail = n;              // Update the tail pointer
        }
    }
    else if (root == NULL) { // If the root is NULL (graph is empty)
        n->up = NULL;       // Initialize up pointer to NULL
        n->left = NULL;     // Initialize left pointer to NULL
        n->xPos = 0;        // Set x position to 0
        n->yPos = 0;        // Set y position to 0
        root = n;           // Set the root to this node
        tail = root;        // Set the tail to the root
        newLineNode = root; // Initialize new line node to the root
    }
}

// Function to cut the graph into sections
void Graph::cutGraph() {
    srand(time(0)); // Seed the random number generator with the current time
    int transform = width / cut; // Calculate the transform based on width and cuts
    curr = root; // Set the current node to the root
    root->isCorner = true; // Mark the root as a corner
    root->gradientVector[0] = simpInt(rand() % 10); // Set the gradient vector for the root
    root->gradientVector[1] = simpInt(rand() % 10);

    // Cut vertically
    while (curr->yPos != height - 1) {
        for (int i = 0; i < transform; i++) {
            curr = curr->down;
        }
        curr->isCorner = true; // Mark the node as a corner
        curr->gradientVector[0] = simpInt(rand() % 10); // Set the gradient vector
        curr->gradientVector[1] = simpInt(rand() % 10);
    }

    curr = root; // Reset current node to the root

    // Cut horizontally and vertically within each section
    while (curr->xPos != width - 1) {
        for (int i = 0; i < transform; i++) {
            curr = curr->right;
        }
        curr->isCorner = true; // Mark the node as a corner
        curr->gradientVector[0] = simpInt(rand() % 10); // Set the gradient vector
        curr->gradientVector[1] = simpInt(rand() % 10);
        temp = curr; // Save the current node

        while (curr->yPos != height - 1) {
            for (int i = 0; i < transform; i++) {
                curr = curr->down;
            }
            curr->isCorner = true; // Mark the node as a corner
            curr->gradientVector[0] = simpInt(rand() % 10); // Set the gradient vector
            curr->gradientVector[1] = simpInt(rand() % 10);
        }
        curr = temp; // Restore the current node
    }
}

// Function to define the distance vectors for each node
void Graph::defineDistVect() {
    int currX = 0; // Current x position
    int currY = 0; // Current y position
    int transform = width / cut; // Calculate the transform based on width and cuts
    float pxDiff = 1 / ((float)transform - 1.0); // Calculate pixel difference
    curr = root; // Set the current node to the root
    currCorner = root; // Set the current corner to the root
    newLineNode = root; // Initialize new line node to the root

    // Initialize distance vectors for the root pixel
    root->toPixel->distanceVectorTL[0] = 0;
    root->toPixel->distanceVectorTL[1] = 0;
    root->toPixel->distanceVectorTR[0] = -((float)transform - 1) * pxDiff;
    root->toPixel->distanceVectorTR[1] = 0;
    root->toPixel->distanceVectorBL[0] = 0;
    root->toPixel->distanceVectorBL[1] = -((float)transform - 1) * pxDiff;
    root->toPixel->distanceVectorBR[0] = -((float)transform - 1) * pxDiff;
    root->toPixel->distanceVectorBR[1] = -((float)transform - 1) * pxDiff;

    srand(time(0)); // Seed the random number generator with the current time

    // Traverse the graph to define distance vectors for each pixel
    while ((curr != tail)) {
        if (curr->right != NULL && currX < width - 1) {
            curr = curr->right;
            if (curr->xPos % transform == 0) {
                // Placeholder for any operations at the transform boundary
            }
            currX++;
        }
        else if (currX == width - 1) {
            if (newLineNode->down != NULL) {
                curr = newLineNode->down;
                newLineNode = newLineNode->down;
            }
            currX = 0;
            currY++;
        }
        if (currX != width - 1 && currY != height - 1) {
            // Define distance vectors for the current pixel
            curr->toPixel->distanceVectorTL[0] = (currX % transform) * pxDiff;
            curr->toPixel->distanceVectorTL[1] = (currY % transform) * pxDiff;
            curr->toPixel->distanceVectorTR[0] = -(1 - ((currX % transform) * pxDiff));
            curr->toPixel->distanceVectorTR[1] = (currY % transform) * pxDiff;
            curr->toPixel->distanceVectorBL[0] = (currX % transform) * pxDiff;
            curr->toPixel->distanceVectorBL[1] = -(1 - ((currY % transform) * pxDiff));
            curr->toPixel->distanceVectorBR[0] = -(1 - ((currX % transform) * pxDiff));
            curr->toPixel->distanceVectorBR[1] = -(1 - ((currY % transform) * pxDiff));

            temp = curr;
            while (temp->yPos % transform != 0) {
                temp = temp->up;
            }
            while (temp->xPos % transform != 0) {
                temp = temp->left;
            }
            curr->toPixel->dotProd1 = (temp->gradientVector[0] * curr->toPixel->distanceVectorTL[0]) + (temp->gradientVector[1] * curr->toPixel->distanceVectorTL[1]);
            temp = curr;

            while (temp->yPos % transform != 0) {
                temp = temp->up;
            }
            while (temp->xPos % transform != 0) {
                temp = temp->right;
            }
            curr->toPixel->dotProd2 = (temp->gradientVector[0] * curr->toPixel->distanceVectorTR[0]) + (temp->gradientVector[1] * curr->toPixel->distanceVectorTR[1]);
            temp = curr;            
            while (temp->yPos % transform != 0) {
                temp = temp->down;
            }
            while (temp->xPos % transform != 0) {
                temp = temp->left;
            }
            curr->toPixel->dotProd3 = (temp->gradientVector[0] * curr->toPixel->distanceVectorBL[0]) + (temp->gradientVector[1] * curr->toPixel->distanceVectorBL[1]);
            temp = curr;
            while (temp->yPos % transform != 0) {
                temp = temp->down;
            }
            while (temp->xPos % transform != 0) {
                temp = temp->right;
            }
            curr->toPixel->dotProd4 = (temp->gradientVector[0] * curr->toPixel->distanceVectorBR[0]) + (temp->gradientVector[1] * curr->toPixel->distanceVectorBR[1]);
            temp = curr;

            // Calculate the smoothed distance vector values
            float tempDist = curr->toPixel->distanceVectorTL[0];
            tempDist = (6 * (pow(tempDist, 5)) - (15 * (pow(tempDist, 4))) + (10 * (pow(tempDist, 3))));
            curr->toPixel->distanceVectorTL[0] = tempDist;

            tempDist = curr->toPixel->distanceVectorTL[1];
            tempDist = (6 * (std::pow(tempDist, 5)) - (15 * (std::pow(tempDist, 4))) + (10 * (std::pow(tempDist, 3))));
            curr->toPixel->distanceVectorTL[1] = tempDist;

            // Interpolate the pixel values
            curr->toPixel->inter1 = curr->toPixel->dotProd1 + (curr->toPixel->distanceVectorTL[0] * (curr->toPixel->dotProd2 - curr->toPixel->dotProd1));
            curr->toPixel->inter2 = curr->toPixel->dotProd3 + (curr->toPixel->distanceVectorTL[0] * (curr->toPixel->dotProd4 - curr->toPixel->dotProd3));
            curr->toPixel->value = curr->toPixel->inter1 + (curr->toPixel->distanceVectorTL[1] * (curr->toPixel->inter2 - curr->toPixel->inter1));

            // Set the color values for the pixel
            curr->toPixel->blue = abs(curr->toPixel->value) * 255;
            curr->toPixel->green = abs(curr->toPixel->value + .02) * 255;
            if (curr->toPixel->green > 255) {
                curr->toPixel->green = 255;
            }
            // Uncomment the following lines if you want to use the red channel
            // curr->toPixel->red = abs(curr->toPixel->value - .02) * 255;
            // if (curr->toPixel->red < 0) {
            //     curr->toPixel->red = 0;
            // }
        }
    }
}

// Function to print the positions of all nodes
void Graph::printNodes() {
    int currX = 0; // Current x position
    int currY = 0; // Current y position
    curr = root;   // Set the current node to the root
    newLineNode = root; // Initialize new line node to the root
    cout << "(" << root->xPos << ", " << root->yPos << ")" << " "; // Print the root node

    while ((curr != tail)) {
        if (curr->right != NULL && currX < width-1) {
            curr = curr->right;
            currX++;
        }
        else if (currX == width-1) {
            cout << endl;
            if (newLineNode->down != NULL) {
                curr = newLineNode->down;
                newLineNode = newLineNode->down;
            }
            currX = 0;
            currY++;
        }
        cout << "(" << curr->xPos << ", " << curr->yPos << ")" << " ";
    }
    cout << endl;
}

// Function to print the color values of all pixels
void Graph::printPixels() {
    int currX = 0; // Current x position
    int currY = 0; // Current y position
    curr = root;   // Set the current node to the root
    newLineNode = root; // Initialize new line node to the root
    cout << "(" << root->toPixel->red << ", " << root->toPixel->green << ", " << root->toPixel->blue << ")" << " "; // Print the root pixel

    while ((curr != tail)) {
        if (curr->right != NULL && currX < width - 1) {
            curr = curr->right;
            currX++;
        }
        else if (currX == width - 1) {
            cout << endl;
            if (newLineNode->down != NULL) {
                curr = newLineNode->down;
                newLineNode = newLineNode->down;
            }
            currX = 0;
            currY++;
        }
        if (currX != width - 1 && currY != height - 1) {
            cout << "(" << curr->toPixel->red << ", " << curr->toPixel->green << ", " << curr->toPixel->blue << ")" << " ";
        }
    }
}

// Function to print the positions and gradient vectors of corner nodes
void Graph::printCorners() {
    int currX = 0; // Current x position
    int currY = 0; // Current y position
    curr = root;   // Set the current node to the root
    newLineNode = root; // Initialize new line node to the root
    cout << "(" << root->xPos << ", " << root->yPos << ")" << " "; // Print the root node
    cout << "Gradient Vector = [" << root->gradientVector[0] << ", " << root->gradientVector[1] << "]" << endl;

    while ((curr != tail)) {
        if (curr->right != NULL && currX < width - 1) {
            curr = curr->right;
            currX++;
        }
        else if (currX == width - 1) {
            if (newLineNode->down != NULL) {
                curr = newLineNode->down;
                newLineNode = newLineNode->down;
            }
            currX = 0;
            currY++;
        }
        if (curr->isCorner == true) {
            cout << "(" << curr->xPos << ", " << curr->yPos << ")" << " ";
            cout << "Gradient Vector = [" << curr->gradientVector[0] << ", " << curr->gradientVector[1] << "]" << endl;
        }
    }
}

// Function to output the graph to a PPM file
void Graph::outputGraph(ofstream& img) {
    int currX = 0; // Current x position
    int currY = 0; // Current y position
    curr = root;   // Set the current node to the root
    newLineNode = root; // Initialize new line node to the root
    img << root->toPixel->red << " " << root->toPixel->green << " " << root->toPixel->blue << endl; // Write the root pixel

    while ((curr != tail)) {
        if (curr->right != NULL && currX < width - 1) {
            curr = curr->right;
            currX++;
        }
        else if (currX == width - 1) {
            if (newLineNode->down != NULL) {
                curr = newLineNode->down;
                newLineNode = newLineNode->down;
            }
            currX = 0;
            currY++;
        }
        if (currX != width - 1 && currY != height - 1) {
            img << curr->toPixel->red << " " << curr->toPixel->green << " " << curr->toPixel->blue << endl;
        }
    }
}

// Function to set the width of the graph
void Graph::setWidth(int w) {
    width = w;
}

// Function to set the height of the graph
void Graph::setHeight(int h) {
    height = h;
}

// Function to set the number of cuts in the graph
void Graph::setCut(int c) {
    cut = c;
}

// Function to get the width of the graph
int Graph::getWidth() {
    return width;
}

// Function to get the number of cuts in the graph
int Graph::getCut() {
    return cut;
}

// Function to get the height of the graph
int Graph::getHeight() {
    return height;
}

// Function to simplify an integer value
int Graph::simpInt(int i) {
    if (i <= 4) {
        return -1;
    }
    else {
        return 1;
    }
}
