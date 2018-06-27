#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
using namesapce std;

class circuit 
{
public:
	void parser(char* _filename);
	void store_data();
	point* _BLboundary; // Chip boundary
	point* _TRboundary; // Chip boundary
	vector < vector < block* > > _Blocks; // All the layout blocks
	int _window; // Window size
	vector < rule* > _Rules; // Conductor rules from rule file



};

class net
{
public:
	vector<int> _critical; // Critical net ids;
	vector<int> _power; // Power net ids;
	vector<int> _ground; // Ground net ids;
}

class point
{
public:
	int _x;
	int _y;
	int _Type; // 1 for Top, 0 for Bottom, 2 for chip boundary, 3 for block on boundary, 4 for mid
	point(int a, int b, int c):_x(a), _y(b), _Type(c){};
	point():_x(0), _y(0), _Type(-1){};
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
	block(int w, int x, int y, int z, int p, int q)
	{
		_BL = new point(w, x);
		_TR = new point(y, z);
		_BR = new point(y, x);
		_TL = new point(w, z);
		_id = p;
		_layer = q;
	}; // Constructor
};

class interval
{
public:
	int _L;
	int _R;
	// int _matchT;
	// int _matchB;
	// interval(int x, int y, int t, int b):_L(x), _R(y), _matchB(b), _matchT(t){};
	interval(int x, int y):_L(x), _R(y){};
	bool operator< (const interval &s) const
	{
		if (this->_R == s._R)
			return (this->_L < s._L);
		else
			return (this->_R > s._R);
	}
};

class cap
{
public:
	float _f; // first
	float _s; // second
	cap(float x, float y):_f(x), _s(y){}; // Constructor
	cap():_f(0), _s(0){}; // Constructor
};

class unit 
{
public:
	float _rr; // range
	cap* _cap;
};

class corr //Correlation
{
public:
	string _name; // name
	vector<*unit> _vu;
	map<int, *unit> _mu;

};

class capacitance_table
{
public:
	corr _array[][];

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