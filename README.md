//Thank you for checking out this program! If you have any questions feel free to ask!

This is a prorgram that is designed to build images using the Perlin Noise algorithm built from scratch in C++.
It works by building a linked list of node objects that each have a pointer to a pixel object.
It goes through the nodes and alters data based upon relativity to a "corner" node, that is then sent to the pixel objects. 
The data determines the color of a pixel which is subjected to how far away it is from corner nodes that have randomized values.
The result is a noise map that is smoothly transitioning between values as apposed to true random noise. 
