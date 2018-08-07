//*****************************************************************************
// Parser.cpp
//*****************************************************************************

#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include "Circuit.h"
using namespace std;

void Circuit::Parser(char* filename)
{
	char design[30];
	char rulefile[30];
	char processfile[30];

	fstream FileInConfig;
	FileInConfig.open(filename, ios::in);

	if (FileInConfig.good())
	{
		cout << "Input Config File open successfully. " << endl;
		cout << "=========================================" << endl;
	}
	else
	{
		cout << "Input Config File open FAILED. " << endl;
		exit(1);
	}

	int hund = 100;
	char g[hund];
	string temp;

	FileInConfig.getline(g, hund, ' ');
	FileInConfig.getline(design, 30);

	FileInConfig.getline(g, hund, ' ');
	FileInConfig.getline(_output, 30);

	FileInConfig.getline(g, hund, ' ');
	FileInConfig.getline(rulefile, 30);

	FileInConfig.getline(g, hund, ' ');
	FileInConfig.getline(processfile, 30);

	/*For debug*/
	// cout << "Design file: " << design << endl;
	// cout << "Output file: " << _output << endl;
	// cout << "Rule file: " << rulefile << endl;
	// cout << "Process file: " << processfile << endl;

	while(FileInConfig >> temp)
	{
		int a;
		if (temp == "critical_nets:")
		{
			FileInConfig >> temp;
			while (temp != "power_nets:")
			{
				a = atoi(temp.c_str());
				_criticalID.push_back(a);
				FileInConfig >> temp;
			}

			FileInConfig >> temp;
			while (temp != "ground_nets:")
			{
				a = atoi(temp.c_str());
				_powerID.push_back(a);
				FileInConfig >> temp;
			}

			FileInConfig >> temp;
			while (!FileInConfig.eof())
			{
				a = atoi(temp.c_str());
				_gndID.push_back(a);
				FileInConfig >> temp;
			}
		}
	}

	FileInConfig.close();

	Parse_RuleFile(rulefile);
	Parse_Design(design);
	Parse_Pro(processfile);
}

void Circuit::Parse_RuleFile(char* rufile)
{
	fstream FileInRule;
	FileInRule.open(rufile, ios::in);

	if (FileInRule.good())
	{
		cout << "Input Rule file open success." << endl;
		cout << "=========================================" << endl;
	}
	else 
	{
		cout << "Input Rule file open failed! " << endl;
		exit(1);
	}

	int Cl, Wmin, Smin, Wmax;
	float Dmin, Dmax;
	int hund = 100;
	char g[hund];
	string temp;

	FileInRule >> Cl;
	while(!FileInRule.eof())
	{
		FileInRule.getline(g, 100, 'r');
		FileInRule >> Wmin >> Smin >> Wmax >> Dmin >> Dmax;
	
		rule* hi = new rule(Cl, Wmin, Smin, Wmax, Dmin, Dmax);
		_rules.push_back(hi);
		FileInRule >> Cl;
	}
		// delete( *(_rules.end()-1) );
		// _rules.pop_back();
		
	/*For Debugging Print out _rules*/
	// for (auto &x : _rules)
	// {
	// 	cout << "Rule File: " 
	// 		<< x->_Lid << " " << x->_wmin << " " << x->_smin << " " << x->_wmax << " " << x->_dmin << " " << x->_dmax << endl;
	// }

	FileInRule.close();
}

void Circuit::Parse_Design(char* desfile)
{
	// cout << "test" << endl;
	int v = 0;
	int b = 0;
	string hi;

	fstream FileInDes;
	FileInDes.open (desfile, ios::in);

	if (FileInDes)
		cout << "Reading Data......" << endl;
	else
	{
		cout << "Input Design file open FAILED." << endl;
		exit(1);
	}

	FileInDes >> v;
	FileInDes >> b;
	_BLboundary = new point(v, b, 2); 

	FileInDes >> v;
	FileInDes >> b;
	_TRboundary = new point( (v-_BLboundary->_x), (b-_BLboundary->_y), 2); 

	/* Print Out for debuging*/
	cout << "Top Right Bound: ";
	cout << _TRboundary->_x << " ";
	cout << _TRboundary->_y << endl;
	cout << "bottom Left Bound: ";
	cout << _BLboundary->_x << " ";
	cout << _BLboundary->_y << endl;

	FileInDes.ignore(18);

	int p;
	int w, x, y, z, q, r; 
	vector < vector < block* > > _BLOCKS(11);
	char g[20];

	while(!FileInDes.eof())
	{
		
		FileInDes >> w >> x >> y >> z >> q >> r;
		FileInDes.getline(g, 10, '\n');
		FileInDes >> p;

		int mins; //minimum spacing

		for (auto &y:_rules)
		{
			if (y->_Lid == r)
				mins = y->_smin;
		}

		/* For Debug*/
		// cout << "min spacing = " << mins << endl;

		w = w - _BLboundary->_x - mins; // Normalization
		x = x - _BLboundary->_y - mins;
		y = y - _BLboundary->_x + mins;
		z = z - _BLboundary->_y + mins;

		// cout << w << " " << x << " " << y << " " << z << " " << q <<" "<< r << endl;
		block* Tmp = new block(w, x, y, z, q, r, _TRboundary->_x, _TRboundary->_y);
		_BLOCKS[r].push_back(Tmp);
	}

	/* For Debug - Print out the design layout */
	// for (auto &y:_blocks)
	// {
	// 	for (auto &x:y)
	// 	{
	// 		cout << x->_BL->_x << " " << x->_BL->_y << " " << x->_TR->_x << " " << x->_TR->_y << " "
	// 				<< x->_id << " " << x->_layer << endl;
	// 	}
	// }

	FileInDes.close();
	_blocks = _BLOCKS;
}

void Circuit::Parse_Pro(char* pro)
{
	fstream FileInPro;
	FileInPro.open(pro, ios::in);

	if (FileInPro.good())
	{
		cout << "=========================================" << endl;
		cout << "Input Process file open success. " << endl;
		cout << "=========================================" << endl;
	}
	else
	{
		cout << "Input Process file open failed!" << endl;
		exit(1);
	}
	int hund = 100;
	char temp[hund];

	FileInPro.getline(temp, 100, '\n');
	FileInPro.getline(temp, 100, ' ');
	FileInPro >> _window;
	cout << "Window Size: " << _window << endl;
	_windowsize = _window*_window;

	FileInPro.close();
}