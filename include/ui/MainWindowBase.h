/*
 * MainWindowBase.h
 *
 *  Created on: Mar 21, 2013
 *      Author: gabriel
 */

#ifndef MAINWINDOWBASE_H_
#define MAINWINDOWBASE_H_

#include "ui/base/ui_MainWindow.h"
#include "clipping/Clipping.h"
#include "clipping/CohenSutherlandClipping.h"
#include "clipping/LiangBarskyClipping.h"
#include <QGraphicsScene>
#include "DisplayFiles.h"
#include "Window.h"
#include <iostream>
#include <QColorDialog>
#include <QFileDialog>

const axis VIEW_PORT_X = 0;
const axis VIEW_PORT_Y = 0;
const axis VIEW_PORT_WIDTH = 500;
const axis VIEW_PORT_HEIGHT = 500;
const axis VIEW_PORT_X_MAX = VIEW_PORT_X + VIEW_PORT_WIDTH;
const axis VIEW_PORT_Y_MAX = VIEW_PORT_Y + VIEW_PORT_HEIGHT;
const Vec3D VIEW_PORT_CENTER = Vec3D(VIEW_PORT_X + (VIEW_PORT_WIDTH/2.0), VIEW_PORT_Y + (VIEW_PORT_HEIGHT/2.0));

class MainWindowBase : public QMainWindow, Ui::SistemaGrafico {
	Q_OBJECT
public:
	MainWindowBase(DisplayFiles* displayFiles, QWidget* parent = 0,  Qt::WindowFlags fl = 0);
	virtual ~MainWindowBase();
	void repaint();
	void geometricShapeToScene(GeometricShape* gs, QGraphicsScene* scene);
	void drawAxis();

public slots:
	void addPoint();
	void addLine();
	void addPolygon();
	void addBezierCurve();
	void addBSplineCurve();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void zoomIn();
	void zoomOut();

	void removeObject();
	void objectSelected();
	void addVector();
	void removeVector();
	void addBSplineVector();
	void removeBSplineVector();


	void rotate(int angle);

	void browseColor();

	void aplicarEscalonamento();

	void increaseClippingSize();
	void decreaseClippingSize();
	void setDefaultClippingSize();

	void changeClippingAlgorithm(bool checked);

	void importObject();

private:
	axis xToVp(axis x) const;
	axis yToVp(axis y) const;
	axis clippingSizeFactor;
	QGraphicsScene* scene;
	DisplayFiles* displayFiles;
	Window* wd;
	int lastAngle;
	QColor selectedColor;
	Clipping* clipping;

	void paintSelectedColor();
	void paintClippingRect();

};

#endif /* MAINWINDOWBASE_H_ */
