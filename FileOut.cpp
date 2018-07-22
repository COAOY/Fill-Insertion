/***********************
FileOut.cpp
***********************/

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <map>
#include "Circuit.h"
using namespace std;

void Circuit::FileOut( vector < vector < block* > > Fill)
{
	fstream fileout;
	fileout.open(_output, ios::out);
	if (fileout.good())
	{
		cout << "=========================================" << endl;
		cout << "output file open success! " << endl;
	}
	else
	{
		cout << "Error! Output file open failed! " << endl;
		exit(1);
	}

	cout << "Writing output file..." << endl
		<< "Please Wait..." << endl;

	int area;
	int counter = 1;
	for (auto &x:Fill)
	{
		for (auto &y:x)
		{
			fileout << counter << " " << y->_BL->_x+_BLboundary->_x << " " << y->_BL->_y+_BLboundary->_y << " " << y->_TR->_x+_BLboundary->_x 
				<< " " << y->_TR->_y+_BLboundary->_y << " 0 " << y->_layer << " Fill;" << endl;
			counter++;

			// Area of all the cells - bug: some area appear to be below 0 or 0			
			// area = (y->_TR->_x - y->_BL->_x)*(y->_TR->_y - y->_BL->_y);
			// fileout << "[" << counter << "] " << area << " L = " << y->_layer << endl;
			// counter++;
		}
	}

	fileout.close();
	cout << "DONE~" << endl;
}