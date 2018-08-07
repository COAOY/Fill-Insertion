//*****************************************************************************
// Blank.cpp
//*****************************************************************************
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "Circuit.h"
using namespace std;

bool Point_Comparison(point d, point e){return d < e;};

void RMStr(string in, int& out1, int& out2)
{
	string a;
	string b;
	int hihi = 0;
	for (auto &x:in)
	{
		if(x != ',' && hihi != 1)
		{
			a+=x;
		}
		else if (x == ',')
		{
			hihi = 1;
		}
		else
			b+=x;
	}

	out1 = stoi(a);
	out2 = stoi(b);
}

void Circuit::Blank_Point()
{
	cout << "Processing..." << endl;
	cout << "Please Wait..." << endl;

	for (int Lay = 1; Lay < _blocks.size(); Lay++) // For all layers
	{
		bool S = _blocks[Lay].size();
		if (S == 0) continue; // Eliminate the empty layer

		vector < point > _P;
		map < int, int > Y; //Mark out all the Y that exist any block point

		Y[0] = 1;
		Y[_TRboundary->_y] = 1;

		for (auto &y:_blocks[Lay])
		{
			_P.push_back( point(y->_TR->_x, y->_TR->_y, y->_TR->_Type) );
			_P.push_back( point(y->_TL->_x, y->_TL->_y, y->_TL->_Type) );
			_P.push_back( point(y->_BR->_x, y->_BR->_y, y->_BR->_Type) );
			_P.push_back( point(y->_BL->_x, y->_BL->_y, y->_BL->_Type) );
			Y[y->_TR->_y] = 1;
			Y[y->_BL->_y] = 1;
		}

		for (auto &x:Y)
		{
			bool Right = 0;
			bool Left = 0;

			for (auto &y:_blocks[Lay])
			{
				if (y->_TR->_y > x.first && y->_BL->_y < x.first && y->_BL->_x != 0 && y->_TR->_x != _TRboundary->_x)
				{
					// For all those Y cut through: mark out the point on the block
					_P.push_back( point(y->_TL->_x, x.first, 3) );
					_P.push_back( point(y->_TR->_x, x.first, 3) );
				}
				else if (y->_TR->_y > x.first && y->_BR->_y < x.first && y->_TR->_x == _TRboundary->_x)
				{
					Right = 1;
					_P.push_back( point(y->_TL->_x, x.first, 3) );
				}
				else if (y->_TL->_y > x.first && y->_BL->_y < x.first && y->_TL->_x == 0)
				{
					Left = 1;
					_P.push_back( point(y->_TR->_x, x.first, 3) );
				}
				if ((y->_TR->_y == x.first || y->_BR->_y == x.first) && y->_TR->_x == _TRboundary->_x)
				{
					Right = 1;
				}
				if ((y->_TL->_y == x.first || y->_BL->_y == x.first) && y->_BL->_x == 0)
				{
					Left = 1;
				}
			}

			/* Mark the Point on the chip boundary(_Type 2) */
			if (Right != 1)
				_P.push_back( point(_TRboundary->_x, x.first, 2) );

			if (Left != 1)
				_P.push_back( point(0, x.first, 2) );
		}

		sort(_P.begin(), _P.end(), Point_Comparison); //Sort all the points

		// for (auto &x:_P)
		// {
		// 	cout << "x = " << x._x << "; y = " << x._y << "; Type = " << x._Type << endl;
		// }

		Blank_Array(_P, Lay);
	}
	
}

void Circuit::Blank_Array( vector < point > &SortedPoint , int Lay )
{
	int pre = SortedPoint[1]._y; // Previous _y value
	int cur = SortedPoint[1]._y; // Current _y value
	vector < int > YY; // all the _y value that matters
	map < int, int > Temp; 
	vector < map < int, int > > Point_Array; // vector map < _x, _Type >
	YY.push_back(pre);

	for (auto &y:SortedPoint)
	{
		cur = y._y;
		if (pre != cur) // If new _y
		{
			Point_Array.push_back(Temp);
			Temp.clear();
			YY.push_back(cur);
			Temp[y._x] = y._Type;
		}
		else
			Temp[y._x] = y._Type;

		pre = y._y;
	}
	Point_Array.push_back(Temp);
	SortedPoint.clear();

	/* Print out for Debug */
	// for (auto &y:Point_Array) // Print out the 2D array of x
	// {
	// 	for (auto &x:y)
	// 		cout << x.first << " ";
		
	// 	cout << endl;
	// 	cout << "====================" << endl;
	// }

	// cout << "Y = "; // Print our all the Y 
	// for (auto &x:YY)
	// {
	// 	cout << x << " ";
	// }
	// cout << endl << "====================" << endl;

	vector < vector < int > > X_Array; // the x value of the whole chip
	vector < int > X_temp;
	int pre_x = -1;

	for (auto &y:Point_Array)
	{
		for(int i = 0; i < 2; i++)
		{
			for (auto &x:y)
			{
				if (i == 0)
				{
					if (x.second == 2)
					{
						X_temp.push_back(x.first);
					}
					else if (x.second == 0)
					{
						X_temp.push_back(x.first);
					}
					else if (x.second == 1)
					{
						continue;
					}
					else if (x.second == 3)
					{
						X_temp.push_back(x.first);
					}
					else if (x.second == 4)
					{
						X_temp.push_back(x.first);
					}
					else if (x.second == 5)
					{
						X_temp.push_back(x.first);
					}
					else if (x.second == 6)
					{
						continue;
					}
					else if (x.second == 7)
					{
						continue;
					}
				}
				else
				{
					if (x.second == 0)
					{
						continue;
					}
					else if (x.second == 1)
					{
						X_temp.push_back(x.first);
					}
					else if (x.second == 2)
					{
						X_temp.push_back(x.first);
					}
					else if (x.second == 3)
					{
						X_temp.push_back(x.first);
					}
					else if (x.second == 4)
					{
						continue;
					}
					else if (x.second == 5)
					{
						continue;
					}
					else if (x.second == 6)
					{
						X_temp.push_back(x.first);
					}
					else if (x.second == 7)
					{
						X_temp.push_back(x.first);
					}
				}
			}
			X_Array.push_back(X_temp);
			X_temp.clear();
		}
	}
	int s = X_Array.size();
	X_Array.erase(X_Array.begin());
	X_Array.erase(X_Array.begin()+s-1);
	s = X_Array.size();

	/* Print out for debug */
	// for (auto &y:X_Array)
	// {
	// 	for (auto &x:y)
	// 	{
	// 		cout << x << " ";
	// 	}
	// 	cout << endl;
	// }
	// cout << "====================" << endl;

	vector < block* > Blank_temp;
	string q;
	string w;
	map < string, interval* > T;

	for (int j = 0; j < s; j++)
	{
		for (int r = 0; r < X_Array[j].size(); r+=2)
		{
			q = to_string(X_Array[j][r]);
			w = to_string(X_Array[j][r+1]);
			q = q + ',';
			q = q + w;

			if (T.count(q))
			{
				int lend = -1;
				for (int i = 0; i < X_Array[j+1].size(); i+=2)
				{
					if (X_Array[j+1][i] == X_Array[j][r] && X_Array[j+1][i+1] == X_Array[j][r+1])
					{
						lend = 1;
						T[q]->_end = YY[(j+1)/2];
						break;
					}
				}
				if (lend != 1)
				{
					block* KKKKK = new block(X_Array[j][r], YY[(j+1)/2], X_Array[j][r+1], T[q]->_start, 0, Lay, _TRboundary->_x, _TRboundary->_y);
					Blank_temp.push_back(KKKKK);
					if (T.count(q))
					{
						map< string, interval* >::iterator it = T.find(q);
						T.erase(it);
					}
				}
			}
			else
			{
				interval* TT = new interval(YY[(j+1)/2], -1);
				T[q] = TT;
			}
		}
	}

	for (auto &x:T)
	{
		int a1;
		int a2;
		RMStr(x.first, a1, a2);
		block* LL = new block(a1, x.second->_end, a2, x.second->_start, 0, Lay, _TRboundary->_x, _TRboundary->_y);
		Blank_temp.push_back(LL);
	}
	_blanks.push_back(Blank_temp);

	/* Print out for debug */
	// cout << "Layer " << Lay << endl;
	// cout << "Blank: " << endl;
	// for (auto &y:Blank_temp)
	// {
	// 	cout << "x = " << y->_BL->_x << " y = " << y->_BL->_y << endl;
	// 	cout << "x = " << y->_TR->_x << " y = " << y->_TR->_y << endl;

	// 	// cout << "Area = " << ((y->_TR->_y - y->_BL->_y)*(y->_TR->_x - y->_BL->_x)) << endl;
	// }
	// cout << "!!!!!!!!!!!!!!!!!!!!!!!!DONE!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
}
