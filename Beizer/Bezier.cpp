#include "Bezier.h"
#include <QMouseEvent>
#include <QPainter>

Bezier::Bezier(QWidget *parent)
	: QMainWindow(parent)
{
	pointsAmount = 0;
	m_vecCurvePt.clear();
	m_vecTempCurvePt.clear();
	isSure = 0;

	m_vecCtrlPt.clear();
	m_vecTempCtrlPt.clear();

	ui.setupUi(this);
	ui.centralWidget->setMouseTracking(true);
	setMouseTracking(true);
}

void Bezier::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	QPen pen;
	pen.setColor(Qt::red);
	pen.setWidth(1);
	pen.setStyle(Qt::SolidLine);
	painter.setPen(pen);

	if (m_vecTempCtrlPt.size() == 2) {//画直线
		QLine tempLine(m_vecTempCtrlPt[0], m_vecTempCtrlPt[1]);
		painter.drawLine(tempLine);
	}
	else if (m_vecTempCtrlPt.size() == 3)//画二次贝塞尔曲线
	{
		CreateBezier2();
		for (std::vector<QPoint>::iterator it = m_vecTempCurvePt.begin(); it != m_vecTempCurvePt.end(); ++it) {
			painter.drawPoint(*it);
		}

	}
	else if (m_vecTempCtrlPt.size() == 4)//画三次的贝塞尔曲线
	{
		CreateBezier3();
		for (std::vector<QPoint>::iterator it = m_vecTempCurvePt.begin(); it != m_vecTempCurvePt.end(); ++it) {
			painter.drawPoint(*it);
		}
	}
	pen.setColor(Qt::blue);
	pen.setWidth(5);
	pen.setStyle(Qt::SolidLine);
	painter.setPen(pen);
	for (std::vector<QPoint>::iterator it = m_vecTempCtrlPt.begin(); it != m_vecTempCtrlPt.end(); ++it)
	{
		painter.drawPoint(*it);
	}
	
	pen.setColor(Qt::green);
	pen.setWidth(1);
	pen.setStyle(Qt::SolidLine);
	painter.setPen(pen);
	for (std::vector<QPoint>::iterator it = m_vecClearPt.begin(); it != m_vecClearPt.end(); ++it) {
		painter.drawPoint(*it);
	}
}

void Bezier::mousePressEvent(QMouseEvent *event)
{

}

void Bezier::mouseMoveEvent(QMouseEvent *event)
{
	if (m_vecTempCtrlPt.size() == 1)
	{
		m_vecTempCtrlPt.push_back(event->pos());
		update();
	}
	else if (isSure == 0 && m_vecTempCtrlPt.size() > 1) {
		m_vecTempCtrlPt.pop_back();
		m_vecTempCtrlPt.push_back(event->pos());
		update();
	}
	else if (isSure == 1 && m_vecTempCtrlPt.size() > 1)
	{
		m_vecTempCtrlPt.push_back(event->pos());
		isSure = 0;
		update();
	}
}

void Bezier::mouseReleaseEvent(QMouseEvent *event)
{
	//首次画线
	if (m_vecTempCtrlPt.size() == 0)
	{
		QPoint p0 = event->pos();
		m_vecTempCtrlPt.push_back(p0);
		pointsAmount += 1;
	}
	else if (m_vecTempCtrlPt.size() == 2) {
		QPoint p1 = event->pos();
		m_vecTempCtrlPt.pop_back();
		m_vecTempCtrlPt.push_back(p1);
		pointsAmount += 1;
		isSure = 1;
		m_vecClearPt.clear();
		//做插值运算
		lineInterpolation(m_vecTempCtrlPt[0], m_vecTempCtrlPt[1]);
	}
	else if (m_vecTempCtrlPt.size() == 3) {
		QPoint p1 = event->pos();
		m_vecTempCtrlPt.pop_back();
		m_vecTempCtrlPt.push_back(p1);
		pointsAmount += 1;
		isSure = 1;
		m_vecClearPt.clear();
		m_vecClearPt.insert(m_vecClearPt.end(), m_vecTempCurvePt.begin(), m_vecTempCurvePt.end());

	}
	else if (m_vecTempCtrlPt.size() == 4)
	{
		QPoint p1 = event->pos();
		m_vecTempCtrlPt.pop_back();
		m_vecTempCtrlPt.push_back(p1);
		pointsAmount += 1;
		isSure = 1;
		m_vecClearPt.clear();
		m_vecClearPt.insert(m_vecClearPt.end(), m_vecTempCurvePt.begin(), m_vecTempCurvePt.end());

		int index = m_vecTempCtrlPt.size() - 4;
		m_vecCtrlPt.push_back(m_vecTempCtrlPt[index]);
		std::vector<QPoint>::iterator it = m_vecTempCtrlPt.begin() + index;
		std::vector<QPoint>::iterator itNext = m_vecTempCtrlPt.begin() + index + 1;
		m_vecTempCtrlPt.erase(it, itNext);//起始位置，终点位置
	}

}

void Bezier::lineInterpolation(QPoint p1, QPoint p2)
{
	float interX = std::abs(p2.x() - p1.x());
	float interY = std::abs(p2.y() - p1.y());

	int x = p1.x();
	int y = p1.y();

	int which = interX > interY ? 0 : 1;
	if (which == 0)//X > Y
	{
		int stepX = (interX / interY == 0) ? interX / interY+0 : interX / interY+1;
		int step_x = p1.x() > p2.x() ? -1 : 1;
		int tempX = step_x;
		int step_y = p1.y() > p2.y() ? -1 : 1;
		int tempY = step_y;

		while ( (x != p2.x()) && (y != p2.y()) )
		{
			step_x = 0;
			while ((std::abs(step_x) != stepX) && (y != p2.y()))
			{
				m_vecClearPt.push_back(QPoint(x, y));
				x += tempX;
				step_x += tempX;
			}
			y += tempY;
			step_y += tempY;
		}
		while (y != p2.y())
		{
			y = p2.y();
			m_vecClearPt.push_back(QPoint(x, y));
		}
	} 
	else//Y > X
	{
		int stepY = (interY / interX == 0) ? interY / interX + 0 : interY / interX + 1;
		int step_x = p1.x() > p2.x() ? -1 : 1;
		int tempX = step_x;
		int step_y = p1.y() > p2.y() ? -1 : 1;
		int tempY = step_y;

		while ((y != p2.y()) && (x != p2.x()))
		{
			step_y = 0;
			while ((std::abs(step_y) != stepY) && (x != p2.x()))
			{
				m_vecClearPt.push_back(QPoint(x, y));
				y += tempY;
				step_y += tempY;
			}
			x += tempX;
			step_x += tempX;
		}
		while (x != p2.x())
		{
			x = p2.x();
			m_vecClearPt.push_back(QPoint(x, y));
		}
	}
}

void Bezier::CreateBezier2()
{
	m_vecTempCurvePt.clear();
	float step = 0.002;
	for (float t = 0.0; t <= 1.0; t += step)
	{
		QPoint stPt;
		int p0 = m_vecTempCtrlPt.size() - 3;
		int p1 = m_vecTempCtrlPt.size() - 2;
		int p2 = m_vecTempCtrlPt.size() - 1;
		float x = m_vecTempCtrlPt[p0].x() * pow(1 - t, 2) +
				  m_vecTempCtrlPt[p1].x() * t * (1 - t) * 2 +
				  m_vecTempCtrlPt[p2].x() * pow(t, 2);

		float y = m_vecTempCtrlPt[p0].y() * pow(1 - t, 2) +
				  m_vecTempCtrlPt[p1].y() * t * (1 - t) * 2 +
				  m_vecTempCtrlPt[p2].y() * pow(t, 2);
		stPt.setX(x);
		stPt.setY(y);
		m_vecTempCurvePt.push_back(stPt);
	}
}

void Bezier::CreateBezier3()
{
	m_vecTempCurvePt.clear();
	float step = 0.002;
	for (float t = 0.0; t <= 1.0; t += step)
	{
		QPoint stPt;
		int p0 = m_vecTempCtrlPt.size() - 4;
		int p1 = m_vecTempCtrlPt.size() - 3;
		int p2 = m_vecTempCtrlPt.size() - 2;
		int p3 = m_vecTempCtrlPt.size() - 1;
		float x = m_vecTempCtrlPt[p0].x() * pow(1-t, 3) +
				  m_vecTempCtrlPt[p1].x() * pow(1 - t, 2) * t * 3 +
				  m_vecTempCtrlPt[p2].x() * pow(t, 2) * (1 - t) * 3 +
				  m_vecTempCtrlPt[p3].x() * pow(t, 3);

		float y = m_vecTempCtrlPt[p0].y() * pow(1 - t, 3) +
				  m_vecTempCtrlPt[p1].y() * pow(1 - t, 2) * t * 3 +
				  m_vecTempCtrlPt[p2].y() * pow(t, 2) * (1 - t) * 3 +
				  m_vecTempCtrlPt[p3].y() * pow(t, 3);
		stPt.setX(x);
		stPt.setY(y);
		m_vecTempCurvePt.push_back(stPt);
	}
}
