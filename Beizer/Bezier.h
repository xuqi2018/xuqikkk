#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Bezier.h"

class Bezier : public QMainWindow
{
	Q_OBJECT

public:
	Bezier(QWidget *parent = Q_NULLPTR);

	void paintEvent(QPaintEvent *event);

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

private:
	void lineInterpolation(QPoint, QPoint);

private:
	Ui::BezierClass ui;

	//已经确定的点
	int pointsAmount;

	//不变的曲线点和锚点
	std::vector<QPoint> m_vecCurvePt;
	std::vector<QPoint> m_vecCtrlPt;

	//画完之后需要清除掉的点
	std::vector<QPoint> m_vecClearPt;

	//还会变化的曲线点和锚点
	std::vector<QPoint> m_vecTempCurvePt;
	std::vector<QPoint> m_vecTempCtrlPt;

	//画二次的贝塞尔曲线
	void CreateBezier2();

	//画三次的贝塞尔曲线
	void CreateBezier3();

	int isSure;
};
