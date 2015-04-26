#ifndef PREMEASUREDCOLOR
#define PREMEASUREDCOLOR

enum PreMeasuredColor
{
	BROWN,	   //	0
	RED,	   //	1
	GREEN,	   //	2
	BLUE,	   //	3
	WHITE,	   //	4
	BLACK,	   //	5
	UNKNOWN,   //	6
	sizeOfPreMeasuredColorArray
};

// Use readRGBC() function to calibrate the following values 
// according to the colors used in your project.
// You can add or remove colors from the enum and list.

const short int preMeasuredColors[sizeOfPreMeasuredColorArray][4] = 
{
// Pre-measured RGBC values
		{186, 189, 90, 465}, 		// BROWN
		{180, 125, 70, 374}, 		// RED
		{162, 391, 138, 710}, 		// GREEN
		{70, 156, 162, 399}, 		// BLUE
		{335, 500, 343, 1200}, 		// WHITE
		{53, 94, 61, 210}, 	 		// BLACK
		{276, 342, 153, 800}
};

#endif