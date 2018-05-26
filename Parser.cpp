/********************
parser.cpp
********************/
#include "circuit.h"
#include <string>

void circuit::parser(char* _filename)
{
////////////////////////////////////////////////////////////////////////////////////// Parsing input config file

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
////////////////////////////////////////////////////////////////////////////////////// Parsing the layout input

	cout << "===========================================================" << endl;

	fstream FileInDes;
	FileInDes.open (design, ios::in);

	if (FileInDes)
		cout << "Reading......" << endl;
	else
		cout << "Input Config file open FAILED." << endl;

	int v = 0;
	int b = 0;

	FileInDes >> v;
	FileInDes >> b;
	_BLboundary = new point(v, b); 

	FileInDes >> v;
	FileInDes >> b;
	_TRboundary = new point(v, b); 

	/* Print Out for debuging*/
	cout << _TRboundary->_x << " ";
	cout << _TRboundary->_y << endl;
	cout << _BLboundary->_x << " ";
	cout << _BLboundary->_y << endl;

	FileInDes.ignore(18);

	while(!FileInDes.eof())
	{
		int p;
		int w, x, y, z, q, r; 

		FileInDes >> w >> x >> y >> z >> q >> r;
		FileInDes.getline(temp, 100, '\n');
		FileInDes >> p;

		w = w - _BLboundary->_x; // Normalization
		x = x - _BLboundary->_y;
		y = y - _BLboundary->_x;
		z = z - _BLboundary->_y;

		block* Tmp = new block(w, x, y, z, q, r);
		_Blocks[r].push_back(Tmp);
		
	}
		
	// cout << "DONE~~~~ Hohohohohohoho~~~" << endl;

	FileInDes.close();

////////////////////////////////////////////////////////////////////////////////////// Parsing Process file

	fstream FileInPro;
	FileInPro.open(processfile, ios::in);

	FileInPro.getline(temp, 100, '\n');
	FileInPro.getline(temp, 100, ' ');
	FileInPro >> _window;
	
	 

	FileInPro.close();

////////////////////////////////////////////////////////////////////////////////////// Parsing rule file

	fstream FileInRule;
	FileInRule.open(rulefile, ios::in);

	while(!FileInRule.eof())
	{
		int Cl, Wmin, Smin, Wmax;
		float Dmin, Dmax;

		FileInRule >> Cl;
		FileInRule.getline(temp, 100, 'R');
		FileInRule >> Wmin >> Smin >> Wmax >> Dmin >> Dmax;
		rule* hi = new rule(Cl, Wmin, Smin, Wmax, Dmin, Dmax);
		_Rules.push_back(hi);
	}

	FileInRule.close();

	return;
}

