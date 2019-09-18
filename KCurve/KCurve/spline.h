#pragma once
#include "curves.h"

class Knot {
public:
	Knot(){}
	Knot(int x = 0, int y = 0, CurveType ty = CurveType::smooth);
	void logKnot();
	void setTan(float th, bool isRight);

	void updateSelDecoration(bool selected, bool mode);

	void updatePos(float x, float y);

	

	bool operator < (const Knot& t) const {
		if (_x > t._x)
		{
			return true;
		}
		return false;
	}

public:
	//SplineEdit _se;
	float _x;
	float _y;
	CurveType _ty;
	float _lth;
	float _rth;
	bool _selected;
	Vec2 _lastPt;


	float _computedLTh;
	float _computedRTh;
};

