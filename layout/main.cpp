#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<stdlib.h>
#define size 10

using namespace std;

class point
{
public:
	int _x;
	int _y;
	point(int a, int b):_x(a), _y(b){};
	point():_x(0), _y(0){};

};

class block
{
public:
	point* _BL; // Bottom Left corner
	point* _TR; // Top Right corner
	int _id;
	int _layer;
	block(int w, int x, int y, int z, int p, int q)
	{
		_BL = new point(w, x);
		_TR = new point(y, z);
		_id = p;
		_layer = q;
	}; // Constructor
};

class Layer
{
public:
	vector<block*> _L; // Layers
	int _Lid;
};

int main(int argc, char* argv[])
{
//////////////////////////////////////////////////////////////////////parsing Config file
	string design;
	string output;
	string rulefile;
	string processfile;

	fstream FileInCong;
	FileInCong.open (argv[1], ios::in);

	if (FileInCong)
		cout << "Input Config file open success." << endl;
	else
		cout << "Input Config file open FAILED." << endl;

	int k = 100;
	char temp[k];

	FileInCong.getline(temp, k, ' ');
	FileInCong >> design;

	FileInCong.getline(temp, k, ' ');
	FileInCong >> output;

	FileInCong.getline(temp, k, ' ');
	FileInCong >> rulefile;

	FileInCong.getline(temp, k, ' ');
	FileInCong >> processfile;
	
	string t;
	int a;
	vector<int> _critical;
	vector<int> _power;
	vector<int> _ground;

	while(FileInCong >> t)
	{
		if (t == "critical_nets:")
		{
			FileInCong >> t;
			while (t != "power_nets:")
			{
				a = atoi(t.c_str());
				_critical.push_back(a);
				FileInCong >> t;
			}

			FileInCong >> t;
			while (t != "ground_nets:")
			{
				a = atoi(t.c_str());
				_power.push_back(a);
				FileInCong >> t;
			}

			FileInCong >> t;
			while (!FileInCong.eof())
			{
				a = atoi(t.c_str());
				_ground.push_back(a);
				FileInCong >> t;
			}
		}
	}

	FileInCong.close();
//////////////////////////////////////////////////////////////////////parsing input file
	fstream Layout[size];
	
	for (int i = 1; i < size; i++)
	{
		string m;
		m = to_string(i);
		Layout[i].open("layout"+m+".svg", ios::out);
	}


	int v = 0;
	int b = 0;
	string hi;

	fstream FileInDes;
	FileInDes.open (design, ios::in);

	if (FileInDes)
		cout << "Reading......" << endl;
	else
		cout << "Input Config file open FAILED." << endl;

	FileInDes >> v;
	FileInDes >> b;
	point* _BLboundary = new point(v, b); 

	FileInDes >> v;
	FileInDes >> b;
	point* _TRboundary = new point((v-(_BLboundary->_x)), b-(_BLboundary->_y)); 

	FileInDes.ignore(18);

	for (int j = 1;j < size; j++)
	{
		Layout[j] << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << _TRboundary->_x
			<< "\" height=\"" << _TRboundary->_y << "\">";


		Layout[j] << "<polygon fill=\"#fcfcfc\" points=\"0,0," << _TRboundary->_x << ",0," 
			<< _TRboundary->_x << ","  << _TRboundary->_y << ",0," << _TRboundary->_y << "\"/>" << endl;
	}

	int num = _TRboundary->_y/10;
	int num2 = _TRboundary->_x/10;

	// Draw line
	for(int j = 1; j < size; j++)
	{
		for (int i = 0; i < num+1; i++)
		{
			Layout[j] << endl;
			Layout[j] << "<line x1=\"0\" y1=\"" << (10*i)
				<< "\" x2=\"10000\" y2=\"" << (10*i) 
				<< "\" style=\"stroke:black;stroke-width:3;stroke-opacity:0.5\"/>" << endl;

		}
		for (int i = 0; i < num2+1; i++)
		{
			Layout[j] << "<line x1=\"" << (10*i)
				<< "\" y1=\"0\" x2=\"" << (10*i)
				<< "\" y2=\"10000\" style=\"stroke:black;stroke-width:3;stroke-opacity:0.5\"/>";
		}
		Layout[j] << endl;
	}

	
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int p;
	int w, x, y, z, q, r; 
	vector<block*> _BLOCKS;
	char g[20];

	while(!FileInDes.eof())
	{
		int p;
		int w, x, y, z, q, r; 

		FileInDes >> w >> x >> y >> z >> q >> r;
		FileInDes.getline(g, 100, '\n');
		FileInDes >> p;

		w = w - _BLboundary->_x; // Normalization
		x = x - _BLboundary->_y;
		y = y - _BLboundary->_x;
		z = z - _BLboundary->_y;

		cout << w << " " << x << " " << y << " " << z << " " << q << " " << r << endl;

		///////////////////////////////////////////////////////////////////////
		int counter = 0;

		for (auto &c:_critical)
		{
			if (q == c)
				Layout[r] << "<polygon fill=\"#b15bff\" points=\"" << w << "," << abs(x-_TRboundary->_y) 
					<< "," << y << "," << abs(x-_TRboundary->_y) 
					<< "," << y << "," << abs(z-_TRboundary->_y) 
					<< "," << w << "," << abs(z-_TRboundary->_y) << "\"/>" << endl;
			else
				counter++;
		}
		for (auto &c:_power)
		{
			if (q == c)
				Layout[r] << "<polygon fill=\"#84c1ff\" points=\"" << w << "," << abs(x-_TRboundary->_y) 
					<< "," << y << "," << abs(x-_TRboundary->_y) 
					<< "," << y << "," << abs(z-_TRboundary->_y) 
					<< "," << w << "," << abs(z-_TRboundary->_y) << "\"/>" << endl;
			else
				counter++;
		}
		for (auto &c:_ground)
		{
			if (q == c)
				Layout[r] << "<polygon fill=\"#a23400\" points=\"" << w << "," << abs(x-_TRboundary->_y) 
					<< "," << y << "," << abs(x-_TRboundary->_y) 
					<< "," << y << "," << abs(z-_TRboundary->_y) 
					<< "," << w << "," << abs(z-_TRboundary->_y) << "\"/>" << endl;
			else
				counter++;
		}
		if (counter == 3)
		{
			Layout[r] << "<polygon fill=\"#adadad\" points=\"" << w << "," << abs(x-_TRboundary->_y) 
				<< "," << y << "," << abs(x-_TRboundary->_y)
				<< "," << y << "," << abs(z-_TRboundary->_y) 
				<< "," << w << "," << abs(z-_TRboundary->_y) << "\"/>" << endl;
		}
		
	}
		
	cout << "==================================================" << endl;

	FileInDes.close();
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fstream FileInOut;
	FileInOut.open(output, ios::in);
	if (!FileInOut.good())
		cout << "Output file open failed!";
	else
		cout << "HI";

	while (!FileInOut.eof())
	{
		int p1;
		int w1, x1, y1, z1, q1, r1; 
		char T[10];

		FileInOut.getline(T, 10, ' ');
		FileInOut >> w1 >> x1 >> y1 >> z1 >> q1 >> r1;
		FileInOut.getline(T, 10, '\n');
		w1 = w1 - _BLboundary->_x; // Normalization
		x1 = x1 - _BLboundary->_y;
		y1 = y1 - _BLboundary->_x;
		z1 = z1 - _BLboundary->_y;
		cout << w1 << " " << x1 << " " << y1 << " " << z1 << " " << q1 << " " << r1 << endl;
		
		Layout[r1] << "<polygon fill=\"#28ff28\" points=\"" << w1 << "," << abs(x1-_TRboundary->_y) << "," << y1 << "," << abs(x1-_TRboundary->_y)
				<< "," << y1 << "," << abs(z1-_TRboundary->_y) << "," << w1 << "," << abs(z1-_TRboundary->_y) << "\"/>" << endl;

	}

	FileInOut.close();

	for(int j = 1; j < size; j++)
	{
		Layout[j] << "</svg>";
		Layout[j].close();

	}
	return 0;
}

