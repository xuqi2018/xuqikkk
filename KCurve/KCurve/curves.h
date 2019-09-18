#pragma once
#include <iostream>
#include <QDebug>
#include <map>
#include <string.h>
#include <math.h>
#define FloatIlegal -10000
#define PI 3.141592653

enum CurveType {
	corner = 0,
	smooth = 1
};

enum MoveType {
	dragging = 0,
	creating = 1
};

enum BezMent {
	moveTo = 0,
	lineTo = 1,
	curveTo = 2,
	cloasePath = 3,
	mark = 4
};

class Vec2
{
public:
	Vec2(float x =0, float y =0)
		:_x(x), _y(y){}

	Vec2& operator= (const Vec2& v) {
		_x = v._x;
		_y = v._y;
		return *this;
	}

public:
	float _x;
	float _y;
};

class GlobalFunc {
public:
	static float mod2pi(float th) {
		float twopi = 2 * PI;
		float frac = th * (1 / twopi);
		return twopi * (frac - round(frac));
	}

	static void myCubic(float th0, float th1, std::vector<float>& coords) {
		auto myCubicLen = [](float th0, float th1) {
			float offset = 0.3 * sin(th1 * 2 - 0.4 * sin(th1 * 2));
			bool newShape = true;
			if (newShape) {
				float scale = 1.0 / (3 * 0.8);
				float len = scale * (std::cos(th0 - offset) - 0.2 * std::cos((3 * (th0 - offset))));
				return len;
			}
			else {
				float drive = 2.0;
				float scale = 1.0 / (3 * tanh(drive));
				float len = scale * tanh(drive * cos(th0 - offset));
				return len;
			}
		};

		//std::vector<float> coords(8);
		float len0 = myCubicLen(th0, th1);
		coords[2] = cos(th0) * len0;
		coords[3] = sin(th0) * len0;

		float len1 = myCubicLen(th1, th0);
		coords[4] = 1 - cos(th1) * len1;
		coords[5] = sin(th1) * len1;
		coords[6] = 1;
		//return coords;
	}
};

class CubicBez {
public:
	CubicBez(std::vector<float>& coords){
		_c.clear();
		for (auto it = coords.begin(); it != coords.end(); ++it)
		{
			_c.push_back(*it);
		}
	}

	void weightsum(float c0, float c1, float c2, float c3, Vec2& result) {
		float x = c0 * _c[0] + c1 * _c[2] + c2 * _c[4] + c3 * _c[6];
		float y = c0 * _c[1] + c1 * _c[3] + c2 * _c[5] + c3 * _c[7];
		result._x = x;
		result._y = y;
	}

	void deriv(float t, Vec2& result) {
		float mt = 1 - t;
		float c0 = -3 * mt * mt;
		float c3 = 3 * t * t;
		float c1 = -6 * t * mt - c0;
		float c2 = 6 * t * mt - c3;
		weightsum(c0, c1, c2, c3, result);
	}

	void deriv2(float t, Vec2& result) {
		float mt = 1 - t;
		float c0 = 6 * mt;
		float c3 = 6 * t;
		float c1 = 6 - 18 * mt;
		float c2 = 6 - 18 * t;
		weightsum(c0, c1, c2, c3, result);
	}
public:
	std::vector<float> _c;
};

class MyCurve {
public:
	void render(float th0, float th1, std::vector<Vec2>& result);
	void render4Quintic(float th0, float th1, float k0, float k1, std::vector<Vec2>& result);
	void render4(float th0, float th1, float k0, float k1, std::vector<Vec2>& result);
	void computeCurvature(float th0, float th1, std::map<std::string, float>& c);
	float endpointTangent(float th);
};

class ControlPoint {
public:
	Vec2 _pt;
	CurveType _ty;
	float _lth;
	float _rth;
	float _rAk;
	float _lAk;
	float _kBlend;
	float _rTh;
	float _lTh;
public:
	ControlPoint(){}
	ControlPoint(Vec2 pt, CurveType ty, float lth, float rth) {
		_pt = pt;
		_ty = ty;
		_lth = lth;
		_rth = rth;
		qDebug() << "(" << pt._x << "," << pt._y << ")";
	}

	ControlPoint& operator= (const ControlPoint& t) {
		this->_pt = t._pt;
		this->_ty = t._ty;
		this->_lth = t._lth;
		this->_rth = t._rth;
		return *this;
	}
};


class TwoParamSpline {
public:
	TwoParamSpline(MyCurve& curve, std::vector<Vec2*> innerPts);
	const std::vector<float>& initialThs();
	int iterDumb(int iter);
	void getThs(int i, std::map<std::string, float>&);

public:
	MyCurve _curve;
	std::vector<Vec2*> _ctrlPts;
	float _startTh;
	float _endTh;
	std::vector<float> _ths;
};

class BezPath {
public:
	std::map<BezMent, std::vector<float> > _path;

};

class Spline {
public:
	Spline(){
	}
	//Spline(const std::vector<ControlPoint>& ctrlPts, bool isClosed);

	int startIx();
	ControlPoint& pt(int i, int start);

	void setCtrlPts(std::vector<ControlPoint>& ctrlPts);
	void solve();

	void computeCurvatureBlending();

	float chordLen(int i);

	void render(BezPath* bezpath);

public:
	std::vector<ControlPoint> _ctrlPts;
	bool _isClosed;
	MyCurve _curve;
};

