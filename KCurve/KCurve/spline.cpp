#include "spline.h"
#include <QDebug>

Knot::Knot( int x, int y, CurveType ty)
{
	//_se = se;
	_x = x;
	_y = y;
	_ty = ty;
	_lth = FloatIlegal;
	_rth = FloatIlegal;
	_selected = false;

	logKnot();
}

void Knot::logKnot()
{
	qDebug() << "Knot Point:(" << _x << "," << _y << ")" << endl;
}

void Knot::setTan(float th, bool isRight)
{
	if (_ty == CurveType::smooth || !isRight)
	{
		_lth = th;
	}
	if (_ty == CurveType::smooth || !isRight)
	{
		_rth = th;
	}
}

void Knot::updateSelDecoration(bool selected, bool mode)
{
	bool lComputed = (_lth == FloatIlegal);
	bool rComputed = (_rth == FloatIlegal);
	bool drawCirc = selected && (mode != MoveType::creating && mode != MoveType::dragging);
	bool drawLTan = !lComputed || drawCirc;
	bool drawRTan = !rComputed || drawCirc;
	float lth = FloatIlegal;
	if (drawLTan && _lth != FloatIlegal)
	{
		lth = _lth;
	}else if (drawLTan && _computedLTh != FloatIlegal)
	{
		lth = _computedLTh;
	}
	float rth = FloatIlegal;
	if (drawRTan && _rth != FloatIlegal)
	{
		rth = _rth;
	}
	else if (drawRTan && _computedRTh != FloatIlegal) {
		rth = _computedRTh;
	}
	if (!drawCirc)
	{
		_lth = FloatIlegal;
		_rth = FloatIlegal;
	}
	_selected = selected;
}

void Knot::updatePos(float x, float y)
{
	_x = x;
	_y = y;
	qDebug() << "translate(" << _x << "," << _y << ")";
}




