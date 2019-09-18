#pragma once

#include <QtWidgets/QMainWindow>
#include <string>
#include <QMouseEvent>
#include "spline.h"
#include <vector>


class TanHandle {
	TanHandle(Knot knot, bool isRight)
		:_knot(knot), _isRight(isRight){}
public:
	Knot _knot;
	bool _isRight;
};



class KCurve1 : public QMainWindow
{
	Q_OBJECT

public:
	KCurve1(QWidget *parent = Q_NULLPTR);
	~KCurve1();

private:
	void renderGrid(bool visible);

	void paintEvent(QPaintEvent *event);

	void mousePressEvent(QMouseEvent *event);

	void mouseMoveEvent(QMouseEvent *event);

	void renderSpline();

	void renderSel();

	void TestBezier();

private:
	//Ui::KCurve1Class ui;

	bool _isClosed;
	std::string _mode;
	int _grid;
	bool _shiftOnDrag;
	std::vector<Knot> _knots;
	std::vector<Knot> _selection;
	QPoint _initPt;
	QPoint _lastPt;
	Spline* _spline;
	BezPath* _bezpath;

	int _width;
	int _height;
};
