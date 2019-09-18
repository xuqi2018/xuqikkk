#include "KCurve1.h"
#include <QPainter>
#include <QPen>
#include "spline.h"
#include <set>

KCurve1::KCurve1(QWidget *parent)
	: QMainWindow(parent)
{
	_isClosed = false;
	_mode = "start";
	_grid = 20;
	_shiftOnDrag = false;
	_knots.clear();
	_selection.clear();
	_spline = new Spline;
	_bezpath = new BezPath;

	_initPt.setX(FloatIlegal);
	_initPt.setY(FloatIlegal);
	_lastPt.setX(FloatIlegal);
	_lastPt.setY(FloatIlegal);

	_width = 640;
	_height = 480;
	this->setMinimumSize(_width, _height);

}

KCurve1::~KCurve1()
{
	delete _spline;
	delete _bezpath;
}

void KCurve1::renderGrid(bool visible)
{
	if (!visible)return;

	QPainter painter(this);
	painter.setPen(QPen(Qt::gray, 1));

	QLine temp;

	for (int i = 0; i < _width; i += _grid)
	{
		temp.setLine(i, 0, i, _height);
		painter.drawLine(temp);
	}

	for (int i = 0; i < _height; i += _grid)
	{
		temp.setLine(0, i, _width, i);
		painter.drawLine(temp);
	}
}

void KCurve1::paintEvent(QPaintEvent *event)
{
	renderGrid(true);
	renderSpline();
	renderSel();

	TestBezier();
}

void KCurve1::mousePressEvent(QMouseEvent *event)
{
	if (!(event->button() == Qt::LeftButton)) return;
	std::vector<ControlPoint> ctrlPts;
	ctrlPts.push_back(ControlPoint(Vec2(event->x(), event->y()), CurveType::smooth, FloatIlegal, FloatIlegal));
	_spline->setCtrlPts(ctrlPts);

	if (_initPt.x() == FloatIlegal && _initPt.y() == FloatIlegal)
	{
		_initPt.setX(event->x());
		_initPt.setY(event->y());
	}

	update();
}

void KCurve1::mouseMoveEvent(QMouseEvent *event)
{

	update();

}

void KCurve1::renderSpline()
{
// 	std::vector<ControlPoint> ctrlPts;
// 	 ctrlPts.push_back(ControlPoint(Vec2(147,155),CurveType::smooth,FloatIlegal,FloatIlegal));
// 	 ctrlPts.push_back(ControlPoint(Vec2(377,192),CurveType::smooth,FloatIlegal,FloatIlegal));
// 	 ctrlPts.push_back(ControlPoint(Vec2(291,338),CurveType::smooth,FloatIlegal,FloatIlegal));
// 	_spline->setCtrlPts(ctrlPts);

	_spline->_isClosed = _isClosed;
	_spline->solve();

	_spline->computeCurvatureBlending();
	_spline->render(_bezpath);

}

void KCurve1::renderSel()
{

}

void KCurve1::TestBezier()
{
	QPainter painter(this);
	painter.setPen(QPen(Qt::blue, 1));
	
	QPoint p1, p2, p3;

	int length = _bezpath->_path[BezMent::curveTo].size();


	int i = 0;
	QPoint temp;
	for (i =0; i <= length-6; i+=6)
	{
		if (_lastPt.x() == FloatIlegal && _lastPt.y() == FloatIlegal)
		{
			_lastPt.setX(_initPt.x());
			_lastPt.setY(_initPt.y());

		}
// 		else {
// 			_lastPt.setX(_bezpath->_path[BezMent::curveTo][i / 6 + 2]);
// 			_lastPt.setY(_bezpath->_path[BezMent::curveTo][i / 6 + 3]);
// 		}
		p1.setX(_bezpath->_path[BezMent::curveTo][i]);
		p1.setY(_bezpath->_path[BezMent::curveTo][i+1]);
		p2.setX(_bezpath->_path[BezMent::curveTo][i+2]);
		p2.setY(_bezpath->_path[BezMent::curveTo][i+3]);
		p3.setX(_bezpath->_path[BezMent::curveTo][i+4]);
		p3.setY(_bezpath->_path[BezMent::curveTo][i+5]);
		QPainterPath path(_lastPt);
		path.cubicTo(p1, p2, p3);
		painter.drawPath(path);
		//qDebug()<<path.currentPosition()<<p3;
		//path.moveTo(p3);
		if (i % 6 == 0)
		{
			_lastPt.setX(_bezpath->_path[BezMent::curveTo][i + 4]);
			_lastPt.setY(_bezpath->_path[BezMent::curveTo][i + 5]);
		}
// 		else {
// 			_lastPt.setX(_bezpath->_path[BezMent::curveTo][i / 6 + 3]);
// 			_lastPt.setY(_bezpath->_path[BezMent::curveTo][i / 6 + 4]);
// 		}
		

	}

	_bezpath->_path.clear();
// 	_lastPt.setX(temp.x());
// 	_lastPt.setY(temp.y());
	_lastPt.setX(FloatIlegal);
	_lastPt.setY(FloatIlegal);
}

