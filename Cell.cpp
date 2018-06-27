/********************
Cell
********************/
void Cell( vector < vector < block* > > _BLOCKS )
{
	for (auto &x:_BLOCKS)
	{
		bool S = x.size();
		if (S == 0)
			continue;
		vector < point > _P;
		
		for (auto &y:x)
		{
			_P.push_back( point(y->_TR->_x, y->_TR->_y) );
			_P.push_back( point(y->_TL->_x, y->_TL->_y) );
			_P.push_back( point(y->_BR->_x, y->_BR->_y) );
			_P.push_back( point(y->_BL->_x, y->_BL->_y) );
		}
		sort(_P.begin(), _P.end(), compare);
		showlayershorted(_P);

		int pre = _P[0]._y;
		int tem = _P[0]._y;
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
		//////////////////////////////////////////////////////Here Now

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
							if (prev == 3)
							{
								Xtemp.push_back(Tpre);
								Tpre = -1;
							}
							prev = 1;
						}
						else if (x.second == 3)
						{
							if (prev == 1)
							{
								Xtemp.push_back(x.first);
							}
							else if (prev == 2)
							{
								Tpre = x.first;
							}
							else if (prev == 0)
							{

							}
							else
							{
								Tpre = x.first;
							}
							prev = 3;
						}
						else
						{
							prev = 4;
							Xtemp.push_back(x.first);
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
							if (prev == 3)
							{
								Xtemp.push_back(Tpre);
								Tpre = -1;
							}
							prev = 0;
						}
						else if (x.second == 3)
						{
							if (prev == 0)
							{
								Xtemp.push_back(x.first);
							}
							else if (prev == 2)
							{
								Tpre = x.first;
							}
							else if (prev == 0)
							{

							}
							else
							{
								Tpre = x.first;
							}
							prev = 3;
						}
						else
						{
							prev = 4;
							Xtemp.push_back(x.first);
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
			for (int r = 0; r < XS[j].size(); r++)
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
						block* KKKKK = new block(XS[j][r], Y[(j+1)/2], XS[j][r+1], T[q]->_L, -1, -1);
						Cell.push_back(KKKKK);
						if (T.count(q))
						{
							map< string, interval*>::iterator it = T.find(q);
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

		for (auto &x:T)
		{
			int a1;
			int a2;
			RMStr(x.first, a1, a2);
			block* LL = new block(a1, x.second->_R, a2, x.second->_L, -1, -1);
			Cell.push_back(LL);
		}

		// for (auto &y:Cell)
		// {
		// 	cout << endl;
		// 	cout << "x = " << y->_BL->_x << " y = " << y->_BL->_y << endl;
		// 	cout << "x = " << y->_TR->_x << " y = " << y->_TR->_y << endl;
		// }
		// cout << "DONE!" << endl;

	}
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