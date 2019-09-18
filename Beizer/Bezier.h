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

	//�Ѿ�ȷ���ĵ�
	int pointsAmount;

	//��������ߵ��ê��
	std::vector<QPoint> m_vecCurvePt;
	std::vector<QPoint> m_vecCtrlPt;

	//����֮����Ҫ������ĵ�
	std::vector<QPoint> m_vecClearPt;

	//����仯�����ߵ��ê��
	std::vector<QPoint> m_vecTempCurvePt;
	std::vector<QPoint> m_vecTempCtrlPt;

	//�����εı���������
	void CreateBezier2();

	//�����εı���������
	void CreateBezier3();

	int isSure;
};
