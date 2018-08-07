//*****************************************************************************
// Circuit.h
//*****************************************************************************

#ifndef CIRCUIT_H
#define CIRCUIT_H
#include <vector> 
#include <map>
#include <iostream>
#include <string>
using namespace std;

class point;
class block;
class rule;
class interval;

class Circuit
{
public:
	int _window; // Window size;
	int _windowsize; // Window size (area)
	point* _TRboundary; // Top right boundary
	point* _BLboundary; // Bottom left boundary
	vector < int > _criticalID; // ID for critical nets
	vector < int > _powerID; // ID for power nets
	vector < int > _gndID; // ID for ground nets
	vector < vector < block* > > _blocks; // input blocks(critical, gnd, non-critical)
	vector < rule* > _rules; // Rule file
	char _output[30];
	vector < vector < block* > > _blanks; // Empty blank space
	vector < vector < block* > > _fills; // Dummy Fills

	void Parser(char* filename);
	void Parse_Design(char* desfile);
	void Parse_RuleFile(char* rufile);
	void Parse_Pro(char* pro);

	// bool Point_Comparison(point d, point e);

	void Blank_Point();
	void Blank_Array( vector < point > &SortedPoint, int Lay );
	// void RMstr(string in, int& out1, int& out2);

	bool Check_Density( point window_BL, point window_TR, int window_Layer );
	void Management();

	void FillInsertion( point* TR, point* BL, int layer );
	void FileOut(vector < vector < block* > > Fill);

};

class interval
{
public:
	int _start;
	int _end;

	interval(int a, int b): _start(a), _end(b){};
	interval(): _start(-1), _end(-1){};
};

class point
{
public:
	int _x;
	int _y;
	int _Type;
	/*
		0 for block bottom, 1 for block top, 2 for chip boundary, 
		3 for mid, 4 for block RT on bound , 5 for block LT on bound,
		6 for block LB on bound, 7 for block RB on bound 
	*/

	point(int a, int b, int c): _x(a), _y(b), _Type(c){};
	point(): _x(-1), _y(-1), _Type(-1){};

	bool operator< (const point &s) const
	{
		if (this->_y == s._y)
			return (this->_x < s._x);
		else
			return (this->_y > s._y);
	}
};


class block
{
public:
	point* _BL; // Bottom Left corner
	point* _TR; // Top Right corner
	point* _BR; // Bottom Right corner
	point* _TL; // Top Left corner
	int _id;
	int _layer;

	block(int a, int b, int c, int d, int e, int f, int g, int h)
	{
		// Out of boundary ---> fill to the bound.
		if (a < 0) a = 0;
		if (b < 0) b = 0;

		if (c > g) c = g;
		if (d > h) d = h;

		if (a == 0)
		{
			_BL = new point(a, b, 6);
			_TL = new point(a, d, 5);
		}
		else 
		{
			_BL = new point(a, b, 0);
			_TL = new point(a, d, 1);
		}

		if (c == g)
		{
			_BR = new point(c, b, 7);
			_TR = new point(c, d, 4);
		}
		else 
		{
			_BR = new point(c, b, 0);
			_TR = new point(c, d, 1);
		}

		_id = e;
		_layer = f;
	}; //Constructor
};

class rule
{
public:
	int _Lid; // LayerID
	int _wmin; // Minimum width
	int _wmax; // Maximum width
	int _smin; // Minimum space
	float _dmin; // Minimum density
	float _dmax; // Maximum dentity

	rule(int a, int b, int c, int d, float e, float f)
	{
		_Lid = a;
		_wmin = b;
		_smin = c;
		_wmax = d;
		_dmin = e;
		_dmax = f;
	}; // Constructor
};

#endif