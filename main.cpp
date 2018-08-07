//*****************************************************************************
// main.cpp
//*****************************************************************************
#include "Circuit.h"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
	// cout << "Hello world!" << endl;
	Circuit cir;
	// cout << argv[1] << endl;

	cir.Parser(argv[1]);
	cir.Blank_Point();
	// cir.FillInsertion();
	// cir.FileOut();
	cir.Management();

	return 0;
}