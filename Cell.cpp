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

		int pre = _P[1]._y;
		int tem = _P[1]._y;
		vector < int > Temp;
		vector < vector < int > > PS;
		// cout <<"_p size: "<< _P.size() << endl;
		Temp.push_back(_P[0]._y);
		// cout << _P[0]._y << endl;

		for (auto &y:_P)
		{
			tem = y._y;
			if (pre != tem)
			{
				PS.push_back(Temp);
				Temp.clear();
				Temp.push_back(y._y);
				Temp.push_back(y._x);
			}
			else
				Temp.push_back(y._x);

			pre = y._y;
		}
		PS.push_back(Temp);

		for (auto &y:PS)
		{
			for (auto &x:y)
				cout << x << " ";
				cout << endl;
				cout << "==================" << endl; 
		}

		_P.clear();
	}
}