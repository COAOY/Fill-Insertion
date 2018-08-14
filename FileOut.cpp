//*****************************************************************************
// FileOut.cpp
//*****************************************************************************

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <map>
#include "Circuit.h"
using namespace std;

//*****************************************************************************
//Function  [ FillInsertion ]
//Commentor [ Christine Lai ]
//Synopsis  [ Usage: Paste the pattern of fill onto the input cell. Pattern  
//                   meets the spacing and width criteria.
//            in   : Cell TR point, Cell BL point, Cell layer
//            out  : void 
//          ]
//Date      [ 2018_08_07 ]
//*****************************************************************************
void Circuit::FillInsertion( point* TR, point* BL, int layer)
{
	cout << "Fill Insertion~" << endl;
	vector < block* > Fill_temp;

	int midwidth = (_rules[layer-1]->_wmin + _rules[layer-1]->_wmax)/2;
	int spacing = _rules[layer-1]->_smin;

	cout << "Test: " << endl;
	cout << "Spacing = " << spacing << endl;
	cout << "MidWidth = " << midwidth << endl;

	int y = BL->_y;
	while (y < (TR->_y - midwidth) ) 
	{
		interval interval_temp = interval(BL->_x, TR->_x);
		while (interval_temp._start <= (interval_temp._end-midwidth) )
		{
			block* f_temp = new block(interval_temp._start, y, (interval_temp._start + midwidth), (y + midwidth), 0, layer, _TRboundary->_x, _TRboundary->_y);
			Fill_temp.push_back(f_temp);
			interval_temp._start = interval_temp._start + midwidth;
		}
		y = y + (midwidth*2) + (spacing*2);
	}

	int y2 = BL->_y + midwidth + spacing;
	while(y2 < (TR->_y - midwidth) )
	{
		interval interval_temp = interval((BL->_x + midwidth), TR->_x);
		while (interval_temp._start <= (interval_temp._end - midwidth) )
		{
			block* f_temp = new block(interval_temp._start, y2, (interval_temp._start + midwidth), (y2 + midwidth), 0, layer, _TRboundary->_x, _TRboundary->_y);
			Fill_temp.push_back(f_temp);
			interval_temp._start = interval_temp._start + midwidth;
		}
		y2 = y2 + (midwidth*2) + (spacing*2);
	}
	
}

//********:********************************************************************
//Function  [ FileOut ]
//Commentor [ Christine Lai ]
//Synopsis  [ Usage: Open the output file and write all our output in.   
//                   
//            in   : All the fills in terms of vector < vector < block* > >
//            out  : void 
//          ]
//Date      [ 2018_08_07 ]
//*****************************************************************************
void Circuit::FileOut( vector < vector < block* > > Fill )
{
	fstream fileout;
	fileout.open(_output, ios::out);
	if (fileout.good())
	{
		cout << "=========================================" << endl;
		cout << "Output file open success! " << endl;
	}
	else
	{
		cout << "Error! Output file open failed! " << endl;
		exit(1);
	}

	cout << "Writing output file..." << endl
		<< "Please Wait..." << endl;

	int area=0;
	int counter = 1;
	int EC = 0;
	for (auto &x:Fill)
	{
		for (auto &y:x)
		{
			// fileout << counter << " " << y->_BL->_x+_BLboundary->_x << " " << y->_BL->_y+_BLboundary->_y << " " << y->_TR->_x+_BLboundary->_x 
			// 	<< " " << y->_TR->_y+_BLboundary->_y << " 0 " << y->_layer << " Fill;" << endl;
			// counter++;

			// Area of all the cells - bug: some area appear to be below 0 or 0			
			area = (y->_TR->_x - y->_BL->_x)*(y->_TR->_y - y->_BL->_y);
			fileout << "[" << counter << "] " << area << " L = " << y->_layer << endl;
			if (area <= 0)
			{
				fileout << " " << y->_BL->_x << " " << y->_BL->_y << " "
					<< y->_TR->_x << " " << y->_TR->_y << endl;
					EC++;
			}
			counter++;
		}
	}

	fileout.close();
	cout << "DONE~" << endl;
	cout << "Error count = " << EC << endl;
}