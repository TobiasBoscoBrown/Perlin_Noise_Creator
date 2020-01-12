#include "Graph.h"
#include <string>
#include <iostream>

using namespace std;


void Graph::addNode(int x, int y) {
	nodePtr n = new node;

	n->right = NULL;
	n->down = NULL;
	n->xPos = x;
	n->yPos = y;
	if (root != NULL) { 		

		if (n->xPos == 0) {
			newLineNode->down = n;
			n->up = newLineNode;
			currAboveNode = newLineNode;
			newLineNode = newLineNode->down;
			tail = n;
		}
		else if (n->xPos < width) {
			tail->right = n;
			n->left = tail;
			if (n->yPos > 0 && n->yPos != height) {
				pixelPtr p = new pixel;
				currAboveNode->toPixel = p;
				p->red = 10;
				p->green = 10;
				p->blue = 201;
				currAboveNode = currAboveNode->right;
				n->up = currAboveNode;
				currAboveNode->down = n;
			}
			tail = n;
		}
	}
	
	else if (root == NULL) {
		n->up = NULL;
		n->left = NULL;
		n->xPos = 0;
		n->yPos = 0;
		root = n;
		tail = root;
		newLineNode = root;
	}
}

void Graph::cutGraph()
{
	srand(time(0));
	int transform = width / cut;
	curr = root;
	root->isCorner = true;
	root->gradientVector[0] = simpInt(rand() % 10);
	root->gradientVector[1] = simpInt(rand() % 10);
	
	while (curr->yPos != height - 1) {
		for (int i = 0; i < transform; i++) {
			curr = curr->down;
		}
		curr->isCorner = true;
		curr->gradientVector[0] = simpInt(rand() % 10);
		curr->gradientVector[1] = simpInt(rand() % 10);

	}
	curr = root;
	while (curr->xPos != width - 1) {
		for (int i = 0; i < transform; i++) {
			curr = curr->right;
		}
		curr->isCorner = true;
		curr->gradientVector[0] = simpInt(rand() % 10);
		curr->gradientVector[1] = simpInt(rand() % 10);
		temp = curr;
		while (curr->yPos != height - 1) {
			for (int i = 0; i < transform; i++) {
				curr = curr->down;
			}
			curr->isCorner = true;
			curr->gradientVector[0] = simpInt(rand() % 10);
			curr->gradientVector[1] = simpInt(rand() % 10);
		}
		curr = temp;
	}
}

void Graph::defineDistVect()
{
	int currX = 0;
	int currY = 0;
	int transform = width / cut;
	float pxDiff = 1 / ((float)transform - 1.0);
	curr = root;
	currCorner = root;
	newLineNode = root;
	root->toPixel->distanceVectorTL[0] = 0;
	root->toPixel->distanceVectorTL[1] = 0;
	root->toPixel->distanceVectorTR[0] = -((float)transform  - 1) * pxDiff;
	root->toPixel->distanceVectorTR[1] = 0;
	root->toPixel->distanceVectorBL[0] = 0;
	root->toPixel->distanceVectorBL[1] = -((float)transform - 1) * pxDiff;
	root->toPixel->distanceVectorBR[0] = -((float)transform - 1) * pxDiff;
	root->toPixel->distanceVectorBR[1] = -((float)transform - 1) * pxDiff;

	srand(time(0));


	while ((curr != tail)) {
		if (curr->right != NULL && currX < width - 1) {
			curr = curr->right;
			if (curr->xPos % transform == 0) {

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

			float tempDist = curr->toPixel->distanceVectorTL[0];
			tempDist = (6 * (pow(tempDist, 5)) - (15 * (pow(tempDist, 4))) + (10 * (pow(tempDist, 3))));
			curr->toPixel->distanceVectorTL[0] = tempDist;

			tempDist = curr->toPixel->distanceVectorTL[1];
			tempDist = (6 * (pow(tempDist, 5)) - (15 * (pow(tempDist, 4))) + (10 * (pow(tempDist, 3))));
			curr->toPixel->distanceVectorTL[1] = tempDist;

			curr->toPixel->inter1 = curr->toPixel->dotProd1 + (curr->toPixel->distanceVectorTL[0] * (curr->toPixel->dotProd2 - curr->toPixel->dotProd1));
			curr->toPixel->inter2 = curr->toPixel->dotProd3 + (curr->toPixel->distanceVectorTL[0] * (curr->toPixel->dotProd4 - curr->toPixel->dotProd3));
			curr->toPixel->value = curr->toPixel->inter1 + (curr->toPixel->distanceVectorTL[1] * (curr->toPixel->inter2 - curr->toPixel->inter1));
			curr->toPixel->blue = abs(curr->toPixel->value) * 255;
			curr->toPixel->green = abs(curr->toPixel->value + .02) * 255;
			if (curr->toPixel->green > 255) {
				curr->toPixel->green = 255;
			}
			//curr->toPixel->red = abs(curr->toPixel->value - .02) * 255;
			//if (curr->toPixel->red < 0) {
			//	curr->toPixel->red = 0;
			//}

		}
	}

}

void Graph::printNodes()
{
	int currX = 0;
	int currY = 0;
	curr = root;
	newLineNode = root;
	cout << "(" << root->xPos << ", " << root->yPos << ")" << " ";

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

void Graph::printPixels()
{
	int currX = 0;
	int currY = 0;
	curr = root;
	newLineNode = root;
	cout << "(" << root->toPixel->red << ", " << root->toPixel->green << ", " << root->toPixel->blue << ")" << " ";

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

void Graph::printCorners()
{
	int currX = 0;
	int currY = 0;
	curr = root;
	newLineNode = root;
	cout << "(" << root->xPos << ", " << root->yPos << ")" << " ";
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

void Graph::outputGraph(ofstream& img)
{
	int currX = 0;
	int currY = 0;
	curr = root;
	newLineNode = root;
	img << root->toPixel->red << " " << root->toPixel->green << " " << root->toPixel->blue << endl;

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

void Graph::setWidth(int w)
{
	width = w;
}

void Graph::setHeight(int h)
{
	height = h;
}

void Graph::setCut(int c)
{
	cut = c;
}

int Graph::getWidth()
{
	return width;
}

int Graph::getCut()
{
	return cut;
}

int Graph::getHeight()
{
	return height;
}

int Graph::simpInt(int i)
{
	if (i <= 4) {
		return -1;
	}
	else
	{
		return 1;
	}

}
