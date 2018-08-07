//*****************************************************************************
// Density.cpp
//*****************************************************************************

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "Circuit.h"
using namespace std;

//*****************************************************************************
//Function  [ Management ]
//Commentor [ Christine Lai ]
//Synopsis  [ Usage: Manage the whole process. 
//                   Density Check -> Fill -> ......
//            in   : void
//            out  : void 
//          ]
//Date      [ 2018_08_07 ]
//*****************************************************************************
void Circuit::Management()
{
	int pass = 0;
	int fail = 0;
	int c = 0;
	cout << "Check Density: " << endl;
	for (int L = 1; L < 3; L++)
	{
		cout << "Test" << endl;
		for (int x = 0; x < _TRboundary->_x; x+=(_window/2) )
		{
			for (int y = 0; y < _TRboundary->_y; y+=(_window/2) )
			{
				point BL = point(x, y, 2);
				point TR = point((x+_window/2), (y+_window/2), 2);
				c++;
	
				if (Check_Density(BL, TR, L))
				{
					pass++;
					cout << "1" << endl;
				}
				else
				{
					fail++;
					cout << "0" << endl;
				}
			}
		}
		// cout << "t" << endl;
	}	
	cout << "Counter = " << c << endl;
	cout << "Pass = " << pass << endl;
	cout << "Failed = " << fail << endl;
}

//*****************************************************************************
//Function  [ Check_Density ]
//Commentor [ Christine Lai ]
//Synopsis  [ Usage: Check if this window(w/2) pass the density check. If yes   
//                   return ture, else return false.
//            in   : Window TR point, Window BL point, Window layer
//            out  : pass -> 1, failed -> 0
//          ]
//Date      [ 2018_08_07 ]
//*****************************************************************************
bool Circuit::Check_Density( point window_BL, point window_TR, int window_Layer )
{
	int area_temp = 0;
	for (auto &x:_blocks[window_Layer])
	{
		if ((x->_BL->_x > window_BL._x && x->_TR->_x < window_TR._x) && (x->_BL->_y > window_BL._y && x->_TR->_y < window_TR._y))
		{ // Whole block is in the window
			area_temp = area_temp + ((x->_TR->_x - x->_BL->_x)*(x->_TR->_y - x->_BL->_y));
		}
		else if ((x->_BL->_x > window_BL._x && x->_TR->_x < window_TR._x) && (x->_BL->_y > window_BL._y && x->_TR->_y > window_TR._y))
		{ // X in window, upper Y out of window 
			area_temp = area_temp + ((x->_TR->_x - x->_BL->_x)*(window_TR._y - x->_BL->_y));
		}
		else if ((x->_BL->_x > window_BL._x && x->_TR->_x < window_TR._x) && (x->_BL->_y < window_BL._y && x->_TR->_y < window_TR._y))
		{ // X in window, lower Y out of window
			area_temp = area_temp + ((x->_TR->_x - x->_BL->_x)*(x->_TR->_y - window_BL._y));
		}
		else if ((x->_BL->_y > window_BL._y && x->_TR->_y < window_TR._y) && (x->_BL->_x < window_BL._x && x->_TR->_x < window_TR._x))
		{ // Y in window, left X out of window
			area_temp = area_temp + ((x->_TR->_y - x->_BL->_y)*(x->_TR->_x - window_BL._x));
		}
		else if ((x->_BL->_y > window_BL._y && x->_TR->_y < window_TR._y) && (x->_BL->_x > window_BL._x && x->_TR->_x > window_TR._x))
		{ // Y in window, right Y out of window
			area_temp = area_temp + ((x->_TR->_y - x->_BL->_y)*(window_TR._x - x->_BL->_x));
		}
		else if ((x->_TR->_x < window_TR._x && x->_TR->_y < window_TR._y) && (x->_BL->_x < window_BL._x && x->_BL->_y < window_BL._y))
		{ // Right Top in window
			area_temp = area_temp + ((x->_TR->_x - window_BL._x)*(x->_TR->_y - window_BL._y));
		}
		else if ((x->_TR->_x > window_TR._x && x->_TR->_y > window_BL._y) && (x->_BL->_x < window_TR._x && x->_BL->_y < window_BL._y))
		{ // Left Top in window
			area_temp = area_temp + ((x->_TR->_y - window_BL._y)*(window_TR._x - x->_BL->_x));
		}
		else if ((x->_TR->_x > window_BL._x && x->_TR->_y > window_TR._y) && (x->_BL->_x < window_TR._x && x->_BL->_y < window_TR._y))
		{ // Right Bottom in window
			area_temp = area_temp + ((window_TR._y - x->_BL->_y)*(x->_TR->_x - window_BL._x));
		}
		else if ((x->_TR->_y > window_TR._y && x->_TR->_x > window_TR._x) && (x->_BL->_y < window_TR._y && x->_BL->_x < window_TR._x))
		{ // Left Bottom in window
			area_temp = area_temp + ((window_TR._x - x->_BL->_x) && (window_TR._y - x->_BL->_y));
		}
		else 
		{
			continue;
		}
	}

	// cout << _rules[window_Layer-1]->_dmin << endl;
	// cout << area_temp << " ";
	if (area_temp >= (_windowsize * _rules[window_Layer-1]->_dmin))
		return 1; // Pass Density Check
	else 
		return 0; // Did not pass density check
}