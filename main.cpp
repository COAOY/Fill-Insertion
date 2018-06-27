#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

class point
{
public:
	int _x;
	int _y;
	int _Type; 
	// 1 for Top, 0 for Bottom, 2 for chip boundary, 3 for block on right top
	// 4 for mid, 5 for block on left top
	// 6 for on boundary left bottom, 7 for on right bottom
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
	block(int w, int x, int y, int z, int p, int q, int k, int m)
	{
		if (x < 0)
			x = 0;
		if (w < 0)
			w = 0;

		if (y > k)
			y = k;
		if (z > m)
			z = m;

		if (w == 0)
			_BL = new point(w, x, 6);
		else 
			_BL = new point(w, x, 0);
		
		if (y == k)
			_BR = new point(y, x, 7);
		else 
			_BR = new point(y, x, 0);


		if (w == 0)
			_TL = new point(w, z, 5);
		else
			_TL = new point(w, z, 1);


		if (y == k)
			_TR = new point(y, z, 3);
		else 
			_TR = new point(y, z, 1);

		_id = p;
		_layer = q;
	}; // Constructor
};

class interval
{
public:
	int _L;
	int _R;
	interval(int x, int y):_L(x), _R(y){};
	bool operator< (const interval &s) const
	{
		if (this->_R == s._R)
			return (this->_L < s._L);
		else
			return (this->_R > s._R);
	}
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

bool compare1(point d, point e){return d < e;};
vector< vector< block* > > FillInsertion( vector < vector < block* > > Cell );
void FileOut( vector < vector < block* > > Fill );
void RMStr(string in, int& out1, int& out2);
void ShowLayout( vector < vector < block* > > B); // For debugging
vector < vector < block* > > Cell( vector < vector < block* > > _BLOCK );
void showlayershorted( vector < point > _P );

//////////////////////////////////////////////////////////////////////// Show Layout (for Debuggung)
void ShowLayout( vector < vector < block* > > B)
{
	for (auto &y:B)
	{
		for (auto &x:y)
		{
			cout << x->_BL->_x << " " << x->_BL->_y << " " 
				<< x->_BR->_x << " " << x->_BR->_y << " " 
				<< x->_id << " " << x-> _layer << endl;
		}
	}

	cout << "DONE." << endl;

}

//////////////////////////////////////////////////////////////////////// Global variable
point* _TRboundary;
point* _BLboundary;

string output;


vector < rule* > _Rules; // Conductor rules from rule file

//////////////////////////////////////////////////////////////////////// Main
int main(int argc, char* argv[])
{
	string design;
	// string output;
	string rulefile;
	string processfile;

	fstream FileInCong;
	FileInCong.open (argv[1], ios::in);

	if (FileInCong)
	{
		cout << "Input Config file open success." << endl;
		cout << "=========================================" << endl;
	}
	else
	{
		cout << "Input Config file open FAILED." << endl;
		exit(1);
	}

	int k = 100;
	char temp[k];
	FileInCong.getline(temp, k, ' ');
	FileInCong >> design;
	// cout <<"Design file: " << design << endl;

	FileInCong.getline(temp, k, ' ');
	FileInCong >> output;
	// cout <<"Output file: " << output << endl;

	FileInCong.getline(temp, k, ' ');
	FileInCong >> rulefile;
	// cout <<"Rule file: " << rulefile << endl;

	FileInCong.getline(temp, k, ' ');
	FileInCong >> processfile;
	// cout <<"Process file: " << processfile << endl;
	
	vector<int> _critical;
	vector<int> _power;
	vector<int> _ground;
	
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

	// cout << "critical_nets: ";
	// for (auto &x:_critical)
	// {
	// 	cout << x << " ";
	// }
	// cout << endl << "power_nets: ";

	// for (auto &x:_power)
	// {
	// 	cout << x << " ";
	// }
	// cout << endl << "ground_nets: ";

	// for (auto &x:_ground)
	// {
	// 	cout << x << " ";
	// }
	// cout << endl;

	FileInCong.close();


////////////////////////////////////////////////////////////////////////////

	fstream FileInRule;
	FileInRule.open(rulefile, ios::in);

	if (FileInRule.good())
	{
		cout << "Input Rule file open success!" << endl;
		cout << "=========================================" << endl; 
	}
	else
	{
		cout << "Input Rule file open failed!" << endl;
		exit(1);
	}

	int Cl, Wmin, Smin, Wmax;
	float Dmin, Dmax;

	while(!FileInRule.eof())
	{
		FileInRule >> Cl;
		FileInRule.getline(temp, 100, 'R');
		FileInRule >> Wmin >> Smin >> Wmax >> Dmin >> Dmax;
	
		rule* hi = new rule(Cl, Wmin, Smin, Wmax, Dmin, Dmax);
		_Rules.push_back(hi);
	}
		delete( *(_Rules.end()-1) );
		_Rules.pop_back();
		
		// for(auto &x:_Rules)
		// 	cout << "Rule File:  " 
		// 		<< x->_Lid << " " << x->_wmin << " " << x->_smin << " " << x->_wmax << " " << x->_dmin << " " << x->_dmax << endl;

	FileInRule.close();
	// cout << "===========================================================" << endl;


////////////////////////////////////////////////////////////////////////////

	int v = 0;
	int b = 0;
	string hi;
	// cout << "===========================================================" << endl;


	fstream FileInDes;
	FileInDes.open (design, ios::in);

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
	_TRboundary = new point(v, b, 2); 

	/* Print Out for debuging*/
	// cout << "Top Right Bound: ";
	// cout << _TRboundary->_x << " ";
	// cout << _TRboundary->_y << endl;
	// cout << "bottom Left Bound: ";
	// cout << _BLboundary->_x << " ";
	// cout << _BLboundary->_y << endl;

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

		int mins;

		for (auto &y:_Rules)
		{
			if (y->_Lid == r)
				mins = y->_smin;
		}
		// cout << "min spacing = " << mins << endl;
		w = w - _BLboundary->_x - mins; // Normalization
		x = x - _BLboundary->_y - mins;
		y = y - _BLboundary->_x + mins;
		z = z - _BLboundary->_y + mins;

		// cout << w << " " << x << " " << y << " " << z << " " << q <<" "<< r << endl;
		block* Tmp = new block(w, x, y, z, q, r, _TRboundary->_x, _TRboundary->_y);
		_BLOCKS[r].push_back(Tmp);
		
	}

	// ShowLayout(_BLOCKS); // Show for debuggung

	// cout << "===========================================================" << endl;

	FileInDes.close();

////////////////////////////////////////////////////////////////////////////

	fstream FileInPro;
	FileInPro.open(processfile, ios::in);

	if (FileInPro.good())
	{
		cout << "=========================================" << endl;
		cout << "Input Process file open sucecsee. " << endl;
		cout << "=========================================" << endl;
	}
	else
	{
		cout << "Input Process file open failed!" << endl;
		exit(1);
	}

	int _window = 0;

	FileInPro.getline(temp, 100, '\n');
	FileInPro.getline(temp, 100, ' ');
	FileInPro >> _window;
	// cout << "Window Size: " << _window << endl;

	FileInPro.close();
		// cout << "===========================================================" << endl;

////////////////////////////////////////////////////////////////////////////

	FileOut( FillInsertion( Cell( _BLOCKS ) ) );

	return 0;
}

bool compare(point d, point e){return d < e;};

void showlayershorted( vector < point > _P )
{
	for (auto &x:_P)
	{
		cout << "x = " << x._x << "; y = " << x._y << "; T = " << x._Type << endl;
	}
}


////////////////////////////////////////////////////////////////////////////
vector < vector < block* > > Cell( vector < vector < block* > > _BLOCK )
{
	cout << "Processing..." << endl;
	cout << "Please wait..." << endl;
	vector < vector < block* > > _cell;
	for (int he = 1; he < _BLOCK.size(); he++)
	{
		bool S = _BLOCK[he].size();

		if (S == 0)
			continue;

		vector < point > _P;
		map < int, int > Y1;
		Y1[0] = 1;
		Y1[_TRboundary->_y] = 1;
		// int num=0;
		// cout << "x size:" << x.size() << endl;
		for (auto &y:_BLOCK[he])
		{
			_P.push_back( point(y->_TR->_x, y->_TR->_y, y->_TR->_Type) );
			_P.push_back( point(y->_TL->_x, y->_TL->_y, y->_TL->_Type) );
			_P.push_back( point(y->_BR->_x, y->_BR->_y, y->_BR->_Type) );
			_P.push_back( point(y->_BL->_x, y->_BL->_y, y->_BL->_Type) );
			Y1[y->_TR->_y] = 1;
			Y1[y->_BL->_y] = 1;
			// showlayershorted(_P);
		}

		for (auto &x:Y1)
		{
			int helloR = -1;
			int helloL = -1;

			for (auto &y:_BLOCK[he])
			{
				if (y->_TR->_y > x.first && y->_BL->_y < x.first && y->_BL->_x != 0 && y->_TR->_x != _TRboundary->_x)
				{
					_P.push_back( point(y->_TL->_x, x.first, 4));
					_P.push_back( point(y->_TR->_x, x.first, 4));
				}
				else if (y->_TR->_y >= x.first && y->_BL->_y <= x.first && y->_TR->_x == _TRboundary->_x)
				{
					helloR = 1;
				}
				if (y->_TR->_y >= x.first && y->_BL->_y <= x.first && y->_TL->_x == 0)
				{
					helloL = 1;
				}
			}

			if (helloR != 1)
				_P.push_back( point(_TRboundary->_x, x.first, 2));

			if (helloL != 1)
				_P.push_back( point(0, x.first, 2));
		}

		sort(_P.begin(), _P.end(), compare);
		// showlayershorted(_P);

		int pre = _P[1]._y;
		int tem = _P[1]._y;
		vector < int > Y;
		map < int, int > Temp;
		vector < map < int, int > > PS;
		Y.push_back(pre);

		for (auto &y:_P)
		{
			tem = y._y;
			if (pre != tem)
			{
				PS.push_back(Temp);
				Temp.clear();
				Y.push_back(tem);
				Temp[y._x] = y._Type;
			}
			else
				Temp[y._x] = y._Type;

			pre = y._y;
		}
		PS.push_back(Temp);


		// for (auto &y:PS)
		// {
		// 	for (auto &x:y)
		// 		cout << x.first << " ";
		// 		cout << endl;
		// 		cout << "==================" << endl; 
		// }

		// cout << "Y = ";
		// for (auto &x:Y)
		// {
		// 	cout << x << " ";
		// }
		// cout << endl << "==================" << endl; 

		_P.clear();

		///////////////////////////////////////////////////////////////////
		vector < vector < int > > XS;
		vector < int > Xtemp;
		int prev = -1;
		int La = 0;
		int Tpre = -1;
		for (auto &y:PS)
		{
			for (int i = 0; i < 2; i++)
			{
				for (auto &x:y)
				{
					if (i == 0)
					{
						if (x.second == 2)
						{
							Xtemp.push_back(x.first);
							prev = 2;
						}
						else if (x.second == 0)
						{
							Xtemp.push_back(x.first);
							prev = 0;
						}
						else if (x.second == 1)
						{
							prev = 1;
						}
						else if (x.second == 3)
						{
							Xtemp.push_back(x.first);
							prev = 3;
						}
						else if (x.second == 4)
						{
							prev = 4;
							Xtemp.push_back(x.first);
						}
						else if (x.second == 5)
						{
							Xtemp.push_back(x.first);
							prev = 5;
						}
						else if (x.second == 6)
						{
							prev = 6;
						}
						else
						{
							prev = 7;
						}
					}
					else 
					{
						if (x.second == 2)
						{
							Xtemp.push_back(x.first);
							prev = 2;
						}
						else if (x.second == 1)
						{
							Xtemp.push_back(x.first);
							prev = 1;
						}
						else if (x.second == 0)
						{
							prev = 0;
						}
						else if (x.second == 3)
						{
							prev = 3;
						}
						else if (x.second == 4)
						{
							prev = 4;
							Xtemp.push_back(x.first);
						}
						else if (x.second == 5)
						{
							prev = 5;
						}
						else if (x.second == 6)
						{
							Xtemp.push_back(x.first);
							prev = 6;
						}
						else 
						{
							Xtemp.push_back(x.first);
							prev = 7;
						}
					}
				}
				XS.push_back(Xtemp);
				Xtemp.clear();
			}
		}
		int k = XS.size();
		XS.erase(XS.begin());
		XS.erase(XS.begin()+k-1);

		// for (auto &y:XS)
		// {
		// 	for (auto &x:y)
		// 	{
		// 		cout << x << " ";
		// 	}
		// 	cout << endl;
		// }
		// cout << "==================================" << endl;
		k = XS.size();

		vector < block* > Cell;
		string q;
		string m;

		map < string, interval* > T;


		for (int j = 0; j < k; j++)
		{
			for (int r = 0; r < XS[j].size(); r+=2)
			{
				q = to_string(XS[j][r]);
				m = to_string(XS[j][r+1]);
				q = q+',';
				q = q+m;

				if (T.count(q))
				{
					int lend = -1;
					for (int w = 0; w < XS[j+1].size(); w+=2)
					{
						if (XS[j+1][w] == XS[j][r] && XS[j+1][w+1] == XS[j][r+1])
						{
							lend = 1;
							T[q]->_R = Y[(j+1)/2];
							break;
						}
					}
					if (lend != 1)
					{
						block* KKKKK = new block(XS[j][r], Y[(j+1)/2], XS[j][r+1], T[q]->_L, 0, he, _TRboundary->_x, _TRboundary->_y);
						Cell.push_back(KKKKK);
						if (T.count(q))
						{
							map< string, interval* >::iterator it = T.find(q);
							T.erase(it);
						}
					}
				}
				else
				{
					interval* BBBB = new interval(Y[(j+1)/2], -1);
					T[q] = BBBB;
				}
			}
		}

		// for (auto &x:T)
		// {
		// 	int a1;
		// 	int a2;
		// 	RMStr(x.first, a1, a2);
		// 	block* LL = new block(a1, x.second->_R, a2, x.second->_L, 0, he, _TRboundary->_x, _TRboundary->_y);
		// 	Cell.push_back(LL);
		// }

		// cout << "Layer " << he << endl;
		// cout << "Cell: " << endl;
		// for (auto &y:Cell)
		// {
		// 	cout << endl;
		// 	cout << "x = " << y->_BL->_x << " y = " << y->_BL->_y << endl;
		// 	cout << "x = " << y->_TR->_x << " y = " << y->_TR->_y << endl;
		// }
		// cout << "!!!!!!!!!!!!!!!!!!!!!!!!DONE!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

		_cell.push_back(Cell);
		Cell.clear();
	}
	
	return _cell;
}

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

// void Grid_Div(int window, point TR, point BL)
// {

// }

// void DensityCal()
// {
	
// }

void FileOut(vector < vector < block* > > Fill)
{
	fstream fileout;
	fileout.open(output, ios::out);
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

	int counter = 1;
	for (auto &x:Fill)
	{
		for (auto &y:x)
		{
			fileout << counter << " " << y->_BL->_x+_BLboundary->_x << " " << y->_BL->_y+_BLboundary->_y << " " << y->_TR->_x+_BLboundary->_x 
				<< " " << y->_TR->_y+_BLboundary->_y << " 0 " << y->_layer << " Fill;" << endl;
			counter++;			
		}
	}

	fileout.close();
	cout << "DONE~" << endl;
}

vector< vector< block* > > FillInsertion( vector < vector < block* > > Cell )
{
	vector < block* > tttttt;
	vector < vector < block* > > ans;
	for (int i = 0; i < Cell.size(); i++)
	{
		int LAYER = i+1;
		int midwidth = (_Rules[i]->_wmax + _Rules[i]->_wmin)/2;
		for (auto &y:Cell[i])
		{
			if (y->_TR->_y - y->_BL->_y >= y->_TR->_x - y->_BL->_x)
			{
				interval filltemp = interval(y->_BL->_y, y->_TR->_y);
				while (filltemp._L < filltemp._R - _Rules[i]->_smin - _Rules[i]->_wmax)
				{
					block* ctemp = new block(y->_BL->_x, filltemp._L, y->_TR->_x, (filltemp._L+ midwidth), 0, LAYER, _TRboundary->_x, _TRboundary->_y);
					tttttt.push_back(ctemp);
					filltemp._L = filltemp._L + midwidth + _Rules[i]->_smin;
				}
			}
			else
			{
				interval filltemp = interval(y->_BL->_x, y->_TR->_x);
				while (filltemp._L < filltemp._R - _Rules[i]->_smin - _Rules[i]->_wmax)
				{
					block* ctemp = new block(filltemp._L, y->_BL->_y, (filltemp._L+ midwidth), y->_TR->_x, 0, LAYER, _TRboundary->_x, _TRboundary->_y);
					tttttt.push_back(ctemp);
					filltemp._L = filltemp._L + midwidth + _Rules[i]->_smin;
				}
			}
			
		}
		ans.push_back(tttttt);
		tttttt.clear();

	}
	return ans;
}