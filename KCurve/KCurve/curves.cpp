#include "curves.h"
#include <math.h>
#include <vector>


void MyCurve::render(float th0, float th1, std::vector<Vec2>& result)
{
	std::vector<float> coords(8);
	GlobalFunc::myCubic(th0, th1, coords);
	Vec2 temp;
	temp._x = coords[2];
	temp._y = coords[3];
	result.push_back(temp);
	temp._x = coords[4];
	temp._y = coords[5];
	result.push_back(temp);
}

void MyCurve::render4Quintic(float th0, float th1, float k0, float k1, std::vector<Vec2>& result)
{
	std::vector<float> coords;
	GlobalFunc::myCubic(th0, th1, coords);
	 CubicBez cb(coords);

	auto deriv_scale = [](float t, float th, float k, CubicBez& cb) {
		if (k == FloatIlegal) return (float)1 / 3;
		float c = cos(th);
		float s = sin(th);

		Vec2 d, d2;
		cb.deriv(t, d);
		cb.deriv2(t, d2);
		float d2cross = d2._y * c - d2._x * s;
		float ddot = d._x * c + d._y * s;
		float oldK = d2cross / (ddot * ddot);
		// fudge to avoid divide-by-zero
		if (abs(oldK) < 1e-6) oldK = 1e-6;
		float ratio = k / oldK;
		// TODO: fine tune this dodgy formula
		//let scale = ratio < 1 ? 1/2 - ratio/6 : 1/(3*ratio);
		float scale = 1 / (2 + ratio);
		return scale;
	};
	float scale0 = deriv_scale(0, th0, k0, cb);
	Vec2 d0;
	 cb.deriv(0, d0);
	 Vec2 d00(d0._x * scale0, d0._y * scale0);
	result.push_back(d00);
	Vec2 d1;
	cb.deriv(1, d1);
	float scale1 = deriv_scale(1, -th1, k1, cb);
	Vec2 d11(1 - d1._x * scale1, -d1._y * scale1);
	result.push_back(d11);

}

void MyCurve::render4(float th0, float th1, float k0, float k1, std::vector<Vec2>& result)
{
	//std::vector<Vec2> renderResult;
	if (k0 == FloatIlegal && k1 == FloatIlegal)
	{
		render(th0, th1, result);
		return;
	}

	render4Quintic(th0, th1, k0, k1, result);
}

void MyCurve::computeCurvature(float th0, float th1, std::map<std::string, float>& c)
{
	std::vector<float> coords(8);
	GlobalFunc::myCubic(th0, th1, coords);
	CubicBez cb(coords);
	auto curv = [](float t, float th, CubicBez& cb) {
		float c = cos(th);
		float s = sin(th);
		Vec2 d2;
		cb.deriv2(t, d2);
		float d2cross = d2._y * c - d2._x * s;
		Vec2 d;
		cb.deriv(t, d);
		float ddot = d._x * c + d._y * s;
		return atan2(d2cross, ddot * abs(ddot));
	};

	//let ak0 = cb.atanCurvature(0);
	//let ak1 = cb.atanCurvature(1);
	float ak0 = curv(0, th0, cb);
	float ak1 = curv(1, -th1, cb);
	c["ak0"] = ak0;
	c["ak1"] = ak1;
}

float MyCurve::endpointTangent(float th)
{
	return 0.5 * sin(2 * th);
}

TwoParamSpline::TwoParamSpline(MyCurve& curve, std::vector<Vec2*> ctrlPts)
{
	_curve = curve;
	_ctrlPts.clear();
	_ctrlPts.assign(ctrlPts.begin(), ctrlPts.end());
	_startTh = FloatIlegal;
	_endTh = FloatIlegal;
	_ths.clear();
}


const std::vector<float>& TwoParamSpline::initialThs()
{
	int length = _ctrlPts.size();
	std::vector<float> ths(length,0);
	for (int i = 1; i < length - 1; i++)
	{
		int dx0 = _ctrlPts[i]->_x - _ctrlPts[i - 1]->_x;
		int dy0 = _ctrlPts[i]->_y - _ctrlPts[i - 1]->_y;
		float l0 = hypot(dx0, dy0);
		int dx1 = _ctrlPts[i + 1]->_x - _ctrlPts[i]->_x;
		int dy1 = _ctrlPts[i + 1]->_y - _ctrlPts[i]->_y;
		float l1 = hypot(dx1, dy1);
		float th0 = atan2(dy0, dx0);
		float th1 = atan2(dy1, dx1);

		float bend = GlobalFunc::mod2pi(th1 - th0);
		float th = GlobalFunc::mod2pi(th0 + bend * l0 / (l0 + l1));
		ths[i] = th;
		if (i == 1){	ths[0] = th0;	}
		if (i == ths.size() - 2){ ths[i + 1] = th1; }
	}
	if (_startTh != FloatIlegal)
	{
		ths[0] = _startTh;
	}
	if (_endTh != FloatIlegal)
	{
		ths[ths.size() - 1] = _endTh;
	}
	_ths.assign(ths.begin(), ths.end());
	return ths;
}

int TwoParamSpline::iterDumb(int iter)
{
	auto computeErr = [](std::map<std::string, float> ths0, 
						 std::map<std::string, float> ak0,
						 std::map<std::string, float> ths1, 
						 std::map<std::string, float> ak1) {
		// rescale tangents by geometric mean of chordlengths
		float ch0 = sqrt(ths0["chord"]);
		float ch1 = sqrt(ths1["chord"]);
		float a0 = atan2(sin(ak0["ak1"]) * ch1, cos(ak0["ak1"]) * ch0);
		float a1 = atan2(sin(ak1["ak0"]) * ch0, cos(ak1["ak0"]) * ch1);
		return a0 - a1;
		/*
		return ths1.chord * Math.sin(ak0.ak1) * Math.cos(ak1.ak0)
			- ths0.chord * Math.sin(ak1.ak0) * Math.cos(ak0.ak1);
		*/
	};

	int n = _ctrlPts.size();
	if (_startTh == FloatIlegal)
	{
		std::map<std::string, float> ths0;
		getThs(0, ths0);
		_ths[0] += _curve.endpointTangent(ths0["th1"]) - ths0["th0"];
	}

	if (_endTh == FloatIlegal)
	{
		std::map<std::string, float> ths0;
		getThs(n - 2, ths0);
		_ths[n - 1] -= _curve.endpointTangent(ths0["th0"]) - ths0["th1"];
	}
	if (n < 3) return 0;

	float absErr = 0;
	std::vector<float> x(n - 2);
	std::map<std::string, float> ths0;
	getThs(0, ths0);
	std::map<std::string, float> ak0;
	_curve.computeCurvature(ths0["th0"], ths0["th1"], ak0);
	for (int i = 0; i < n - 2; i++)
	{
		std::map<std::string, float> ths1;
		getThs(i + 1, ths1);
		std::map<std::string, float> ak1;
		_curve.computeCurvature(ths1["th0"], ths1["th1"], ak1);
		float err = computeErr(ths0, ak0, ths1, ak1);
		absErr += std::abs(err);

		float epsilon = 1e-3;
		std::map<std::string, float> ak0p;
		_curve.computeCurvature(ths0["th0"], ths0["th1"] + epsilon, ak0p);
		std::map<std::string, float> ak1p;
		 _curve.computeCurvature(ths1["th0"]  - epsilon, ths1["th1"], ak1p);
		float errp = computeErr(ths0, ak0p, ths1, ak1p);
		float derr = (errp - err) * (1 / epsilon);
		x[i] = err / derr;

		ths0.clear();
		ths0.insert(ths1.begin(), ths1.end());
		ak0.clear();
		ak0.insert(ak1.begin(), ak1.end());
	}

	for (int i = 0; i < n - 2; i++)
	{
		float scale = tanh(0.25 * (iter + 1));
		_ths[i + 1] += scale * x[i];
	}
	return absErr;
}

void TwoParamSpline::getThs(int i, std::map<std::string, float>& result)
{
	float dx = _ctrlPts[i + 1]->_x - _ctrlPts[i]->_x;
	float dy = _ctrlPts[i + 1]->_y - _ctrlPts[i]->_y;
	float th = atan2(dy, dx);
	float th0 = GlobalFunc::mod2pi(_ths[i] - th);
	float th1 = GlobalFunc::mod2pi(th - _ths[i + 1]);
	float chord = hypot(dx, dy);
	result["th0"] = th0;
	result["th1"] = th1;
	result["chord"] = chord;
}

int Spline::startIx()
{
	if (!_isClosed)return 0;
	for (int i = 0; i < _ctrlPts.size(); i++)
	{
		ControlPoint pt = _ctrlPts[i];
		if (pt._ty == CurveType::corner || pt._lth != FloatIlegal)
		{
			return i;
		}
	}
	return 0;
}

ControlPoint& Spline::pt(int i, int start)
{
	int length = _ctrlPts.size();
	return _ctrlPts[(i + start + length) % length];
}

void Spline::setCtrlPts(std::vector<ControlPoint>& ctrlPts)
{
	for (std::vector<ControlPoint>::iterator it = ctrlPts.begin(); it != ctrlPts.end(); ++it)
	{
		_ctrlPts.push_back(*it);
	}
}

void Spline::solve()
{
	int start = startIx();
	int length = _ctrlPts.size() - (_isClosed ? 0 : 1);
	int i = 0;
	while (i < length) {
		ControlPoint& ptI = pt(i, start);
		ControlPoint& ptI1 = pt(i + 1, start);
		if ((i + 1 == length || ptI1._ty == CurveType::corner) && ptI._rth == FloatIlegal && ptI1._lth == FloatIlegal)
		{
			float dx = ptI1._pt._x - ptI._pt._x;
			float dy = ptI1._pt._y - ptI._pt._y;
			float th = atan2(dy, dx);
			ptI._rTh = th;
			ptI1._lTh = th;
			i += 1;
		}
		else {
			std::vector<Vec2*> innerPts;
			innerPts.push_back(&(ptI._pt));
			int j = i + 1;
			while (j < length + 1)
			{
				ControlPoint& ptJ = pt(j, start);
				innerPts.push_back(&(ptJ._pt));
				j += 1;
				if (ptJ._ty == CurveType::corner || ptJ._lth != FloatIlegal)
				{
					break;
				}
			}
			TwoParamSpline inner(_curve, innerPts);
			inner._startTh = pt(i, start)._rth;
			inner._endTh = pt(j - 1, start)._lth;
			int nIter = 10;
			inner.initialThs();
			for (int k = 0; k < nIter; k++)
			{
				inner.iterDumb(k);
			}
			for (int k = i; k + 1 < j; k++)
			{
				ControlPoint& temp1 = pt(k, start);
				temp1._rTh = inner._ths[k - i];

				ControlPoint& temp2 = pt(k+1, start);
				temp2._lTh = inner._ths[k + 1 - i];

				std::map<std::string, float> ths;
				inner.getThs(k - i, ths);
				std::map<std::string, float> aks;
				 _curve.computeCurvature(ths["th0"], ths["th1"], aks);

				ControlPoint& temp3 = pt(k, start);
				temp3._rAk = aks["ak0"];
				ControlPoint& temp4 = pt(k + 1, start);
				temp4._lAk = aks["ak1"];
			}
			i = j - 1;
		}
	}
}


void Spline::computeCurvatureBlending()
{
	auto myTan = [](float th) {
		if (th > PI / 2) {
			return (float)tan(PI - th);
		}
		else if (th < -PI / 2) {
			return (float)tan(-PI - th);
		}
		else {
			return tan(th);
		}
	};
	for (std::vector<ControlPoint>::iterator pt = _ctrlPts.begin(); pt != _ctrlPts.end(); ++pt) {
		pt->_kBlend = FloatIlegal;
	}
	int length = _ctrlPts.size() - (_isClosed ? 0 : 1);
	for (int i = 0; i < length; i++) {
		ControlPoint& ptResult = pt(i, 0);
		if (ptResult._ty == CurveType::smooth && ptResult._lth != FloatIlegal) {
			float thresh = PI / 2 - 0.000001;
			//if (Math.abs(pt.rAk) > thresh || Math.abs(pt.lAk) > thresh) {
			//	// Don't blend reversals. We might reconsider this, but punt for now.
			//	continue;
			//}
			if (signbit(ptResult._rAk) != signbit(ptResult._lAk)) {
				ptResult._kBlend = 0;
			}
			else {
				float rK = myTan(ptResult._rAk) / chordLen(i - 1);
				float lK = myTan(ptResult._lAk) / chordLen(i);
				ptResult._kBlend = 2 / (1 / rK + 1 / lK);
				//console.log(`point ${i}: kBlend = ${pt.kBlend}`);
			}
		}
	}
}

float Spline::chordLen(int i)
{

	Vec2 ptI = pt(i, 0)._pt;
	Vec2 ptI1 = pt(i + 1, 0)._pt;
	return hypot(ptI1._x - ptI._x, ptI1._y - ptI._y);
}

void Spline::render(BezPath* bezpath)
{
	if (_ctrlPts.size() == 0)
	{
		return;
	}
	ControlPoint pt0 = _ctrlPts[0];
	bezpath->_path[BezMent::moveTo].push_back(pt0._pt._x);
	bezpath->_path[BezMent::moveTo].push_back(pt0._pt._y);

	int length = _ctrlPts.size() - (_isClosed ? 0 : 1);
	
	for (int i = 0; i < length; i++) {
// 		path.mark(i);
		ControlPoint& ptI = pt(i, 0);
		ControlPoint& ptI1 = pt(i + 1, 0);
		float dx = ptI1._pt._x - ptI._pt._x;
		float dy = ptI1._pt._y - ptI._pt._y;
		float chth = atan2(dy, dx);
		float chord = hypot(dy, dx);
		float th0 = GlobalFunc::mod2pi(ptI._rTh - chth);
		float th1 = GlobalFunc::mod2pi(chth - ptI1._lTh);
// 		// Apply curvature blending
		float k0 = ptI._kBlend != FloatIlegal ? ptI._kBlend * chord : FloatIlegal;
		float k1 = ptI1._kBlend != FloatIlegal ? ptI1._kBlend * chord : FloatIlegal;
// 		//console.log(`segment ${i}: ${k0} ${k1}`);
		std::vector<Vec2> render1;
		render1.clear();
		_curve.render4(th0, th1, k0, k1, render1);
		std::vector<float> c;
		c.clear();
		for (int j = 0; j < render1.size(); j++) {
			Vec2 pt = render1[j];
			c.push_back(ptI._pt._x + dx * pt._x - dy * pt._y);
			c.push_back(ptI._pt._y + dy * pt._x + dx * pt._y);
		}
		c.push_back(ptI1._pt._x);
		c.push_back(ptI1._pt._y);
		for (int j = 0; j < c.size() -5; j += 6) {
			//path.curveto(c[j], c[j + 1], c[j + 2], c[j + 3], c[j + 4], c[j + 5]);
			for (int i = 0; i <= 5; i++)
			{
				bezpath->_path[BezMent::curveTo].push_back(c[j + i]);
			}
		}
	}
	if (_isClosed)
	{
		bezpath->_path[BezMent::cloasePath].push_back(FloatIlegal);
	}

}
