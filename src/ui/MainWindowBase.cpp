/*
 * MainWindowBase.cpp
 *
 *  Created on: Mar 21, 2013
 *      Author: gabriel
 */
#include "ui/MainWindowBase.h"

MainWindowBase::MainWindowBase(DisplayFiles* displayFiles, QWidget* parent, Qt::WindowFlags fl) :
	QMainWindow(parent, fl) {
	this->scene = 0;
	this->displayFiles = displayFiles;
	this->wd = new Window("Camera Padrao [Camera]");
	displayFiles->addGeometricShape(wd);

	setupUi(this);
	this->tableWidgetPoligono->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	this->tableWidgetBSpline->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	QListWidgetItem* item = new QListWidgetItem(wd->getName(), this->listWidgetObjects);

	this->listWidgetObjects->setItemSelected(item, true);
	this->objectSelected();

	this->clipping = new CohenSutherlandClipping(wd);

	selectedColor = QColor(Qt::black);
	paintSelectedColor();
	drawAxis();
	clippingSizeFactor = 1;
	this->lastAngle = -1;

	this->repaint();
}

MainWindowBase::~MainWindowBase() {
	delete this->displayFiles;
	delete this->scene;
	delete this->wd;
	delete this->clipping;
}

void MainWindowBase::aplicarEscalonamento() {
	if(!this->listWidgetObjects->selectedItems().isEmpty())
	{
		this->displayFiles->scale(this->listWidgetObjects->selectedItems().at(0)->text(), Vec3D(1.0+this->point_x_scale->value()/100.0, 1.0+this->point_y_scale->value()/100.0, 1.0+this->point_y_scale->value()/100.0));
		this->repaint();
	}
}

void MainWindowBase::objectSelected() {
	if(!this->listWidgetObjects->selectedItems().isEmpty()){
		if(this->displayFiles->getGeometricShapeType(listWidgetObjects->selectedItems().at(0)->text()) == GeometricShape::WINDOW){
			this->pushButtonZoomOut->setEnabled(true);
			this->pushButtonZoomIn->setEnabled(true);
		}else{
			this->pushButtonZoomOut->setEnabled(false);
			this->pushButtonZoomIn->setEnabled(false);
		}
	}
}

void MainWindowBase::paintSelectedColor(){
	this->widgetSelectedColor->setStyleSheet("QWidget { background-color: " + this->selectedColor.name() + "; }");
}
void MainWindowBase::drawAxis(){
	this->displayFiles->addLine("eixo_x", Vec3D(0, 0), Vec3D(500, 0), QColor(Qt::black));
	this->displayFiles->addLine("eixo_y", Vec3D(0, 0), Vec3D(0, 500), QColor(Qt::black));
}

void MainWindowBase::addPoint()
{
	if(!this->lineEditPointName->text().isEmpty())
	{
		QString name = this->lineEditPointName->text() + " [Ponto]";
		if(!this->displayFiles->contains(name))
		{
			Vec3D p1(this->point_x->value(),this->point_y->value(), this->point_z->value());
			this->displayFiles->addPoint(name, p1, this->selectedColor);

			this->repaint();
			new QListWidgetItem(name, this->listWidgetObjects);

			this->lineEditPointName->clear();

		}
		else
		{
			this->statusbar->showMessage("Ja existe um objeto com esse nome, por favor digite outro.");
		}
	}
	else
		this->statusbar->showMessage("Por favor, digite um nome para o objeto.");
}
void MainWindowBase::addLine()
{
	if(!this->lineEditLineName->text().isEmpty())
	{
		QString name = this->lineEditLineName->text() + " [Reta]";
		if(!this->displayFiles->contains(name))
		{
			Vec3D p1(this->line_x1->value(),this->line_y1->value(), this->line_z1->value());
			Vec3D p2(this->line_x2->value(),this->line_y2->value(), this->line_z2->value());
			this->displayFiles->addLine(name, p1, p2, this->selectedColor);

			this->repaint();
			new QListWidgetItem(name, this->listWidgetObjects);
			this->lineEditLineName->clear();
		}
		else
		{
			this->statusbar->showMessage("Ja existe um objeto com esse nome, por favor digite outro.");
		}
	}
	else
		this->statusbar->showMessage("Por favor, digite um nome para o objeto.");
}

void MainWindowBase::addPolygon()
{
	if(this->lineEditWireframeName->text().isEmpty())
	{
		this->statusbar->showMessage("Por favor, digite um nome para o objeto.");
		return;
	}
	if(this->displayFiles->contains(this->lineEditWireframeName->text()))
	{
		this->statusbar->showMessage("Ja existe um objeto com esse nome, por favor digite outro.");
		return;
	}

	QString name = this->lineEditWireframeName->text() + " [Poligono]";
	VectorList vl = VectorList();
	for(int i = 0; i < this->tableWidgetPoligono->rowCount(); i++)
	{
		QTableWidgetItem* item_x = this->tableWidgetPoligono->item(i,0);
		QTableWidgetItem* item_y = this->tableWidgetPoligono->item(i,1);
		QTableWidgetItem* item_z = this->tableWidgetPoligono->item(i,2);

		if(item_x && item_y && item_z)
			if(!item_x->text().isEmpty() && !item_y->text().isEmpty() && !item_z->text().isEmpty())
			{
				axis x = item_x->text().toDouble();
				axis y = item_y->text().toDouble();
				axis z = item_z->text().toDouble();

				Vec3D vector(x,y,z);

				vl.append(vector);
			}
	}

	bool brushed;
	if(this->checkBoxBrushed->isChecked())
		brushed = true;
	else
		brushed = false;


	//poligono deve ter pelo menos duas entradas, formando um triangulo nesse caso
	if(vl.size() <= 2)
	{
		this->statusbar->showMessage("Coordenadas insuficientes para formar um Poligono.");
		return;
	}

	this->displayFiles->addPolygon(name, vl, this->selectedColor, this->opacityOorizontalSlider->value(), brushed);
	new QListWidgetItem(name, this->listWidgetObjects);
	this->lineEditWireframeName->clear();
	this->repaint();
}

void MainWindowBase::addBezierCurve()
{
	if(this->lineEditBezierCurveName->text().isEmpty())
	{
		this->statusbar->showMessage("Por favor, digite um nome para o objeto.");
		return;
	}
	if(this->displayFiles->contains(this->lineEditBezierCurveName->text()))
	{
		this->statusbar->showMessage("Ja existe um objeto com esse nome, por favor digite outro.");
		return;
	}
	QString name = this->lineEditBezierCurveName->text() + " [Bezier]";

	Vec3D p1(this->curve_x1->value(), this->curve_y1->value(), this->curve_z1->value());
	Vec3D p2(this->curve_x2->value(), this->curve_y2->value(), this->curve_z2->value());
	Vec3D p3(this->curve_x3->value(), this->curve_y3->value(), this->curve_z3->value());
	Vec3D p4(this->curve_x4->value(), this->curve_y4->value(), this->curve_z4->value());

	this->displayFiles->addBezierCurve(name, p1, p2, p3, p4, this->selectedColor);

	this->repaint();
	new QListWidgetItem(name, this->listWidgetObjects);
	this->lineEditBezierCurveName->clear();

}



void MainWindowBase::repaint()
{
	if(this->scene)
		delete scene;

	this->scene = new QGraphicsScene(VIEW_PORT_X, VIEW_PORT_Y, VIEW_PORT_WIDTH, VIEW_PORT_HEIGHT);
	this->viewPortDefault->setScene(this->scene);

	this->viewPortDefault->setSceneRect(VIEW_PORT_X, VIEW_PORT_Y, VIEW_PORT_WIDTH, VIEW_PORT_HEIGHT);



	this->clipping->setWindowCoordinates(this->wd, Vec3D(clippingSizeFactor, clippingSizeFactor));


	this->displayFiles->viewProjection(this->wd);

	paintClippingRect();
	for(int i = 0; i < this->displayFiles->getFileList()->values().size(); i++)
	{
		GeometricShape* gs = this->clipping->clip(this->displayFiles->getFileList()->values().at(i));
		if(gs != 0){
			this->geometricShapeToScene(gs, this->scene);
		}
	}
}

void MainWindowBase::geometricShapeToScene(GeometricShape* gs, QGraphicsScene* scene)
{
	switch(gs->getType())
	{
		case GeometricShape::WINDOW:
			//TODO Pintar de outra cor para que outras cameras possam ser vistas
			break;
		case GeometricShape::POINT:
			scene->addEllipse(xToVp(gs->getVectorList().at(0).getX()-2), yToVp(gs->getVectorList().at(0).getY()+2), 4, 4,
						QPen(gs->getRgb()), QBrush(Qt::SolidPattern));
			break;
		case GeometricShape::LINE:
			scene->addLine(xToVp(gs->getVectorList().at(0).getX()),yToVp(gs->getVectorList().at(0).getY()), xToVp(gs->getVectorList().at(1).getX()),yToVp(gs->getVectorList().at(1).getY()),  QPen(gs->getRgb()));
			break;
		case GeometricShape::BEZIER_CURVE_2D:
		{
			VectorList vl_bezier = gs->getVectorList();
			for(int i = 0; i < vl_bezier.size()-1; i++)
				scene->addLine(xToVp(vl_bezier.at(i).getX()),yToVp(vl_bezier.at(i).getY()), xToVp(vl_bezier.at(i+1).getX()),yToVp(vl_bezier.at(i+1).getY()),  QPen(gs->getRgb()));
			break;
		}
		case GeometricShape::B_SPLINE_FORWARD_DIFFERENCES:
		{
			VectorList vl_spline = gs->getVectorList();
			for(int i = 0; i < vl_spline.size()-1; i++)
				scene->addLine(xToVp(vl_spline.at(i).getX()),yToVp(vl_spline.at(i).getY()), xToVp(vl_spline.at(i+1).getX()),yToVp(vl_spline.at(i+1).getY()),  QPen(gs->getRgb()));
			break;
		}
		case GeometricShape::WIREFRAME:
		{
			VectorList vl = gs->getVectorList();

			QVector<QPoint> points;
			for(int i = 0; i < vl.size(); i++)
				points.append(QPoint(xToVp(vl.at(i).getX()), yToVp(vl.at(i).getY())));
			QPolygon polygon = QPolygon(points);
			if(((WireFrame*)gs)->getBrushed())
				scene->addPolygon(polygon, QPen(gs->getRgb()), QBrush(gs->getRgb()));
			else
				scene->addPolygon(polygon, QPen(gs->getRgb()));
			break;
		}
		case GeometricShape::OBJECT3D:
		{
			VectorList vl = gs->getVectorList();
			FaceList fl = gs->getFaceList();

			for(int i = 0; i < fl.size(); i++){
				VectorList temp;
				temp.append(Vec3D(vl.at(fl.at(i).f1).getX(), vl.at(fl.at(i).f1).getY()));
				temp.append(Vec3D(vl.at(fl.at(i).f2).getX(), vl.at(fl.at(i).f2).getY()));
				temp.append(Vec3D(vl.at(fl.at(i).f3).getX(), vl.at(fl.at(i).f3).getY()));
				WireFrame* wf = new WireFrame(temp);

				wf =(WireFrame*) this->clipping->clip((GeometricShape*)wf);

				if(wf){
					VectorList vlWf = wf->getVectorList();
					QVector<QPoint> points;
					for(int j = 0; j < vlWf.size(); j++)
						points.append(QPoint(xToVp(vlWf.at(j).getX()), yToVp(vlWf.at(j).getY())));
					QPolygon polygon = QPolygon(points);
//					if(((Object3D*)gs)->getBrushed())
					QColor col = gs->getRgb();
					col.setAlpha(100);
						scene->addPolygon(polygon, QPen(gs->getRgb()), QBrush(col));
//					else
//						scene->addPolygon(polygon, QPen(gs->getRgb()));

					delete wf;
				}
			}
			break;
		}
	}
}

axis MainWindowBase::xToVp(axis x) const {
	return ((x - wd->getXMin()) / (wd->getXMax() - wd->getXMin()))  * (VIEW_PORT_X_MAX - VIEW_PORT_X);
}

axis MainWindowBase::yToVp(axis y) const {
	return ((axis)1 - ( (y - wd->getYMin()) / (wd->getYMax() - wd->getYMin()) )) * (VIEW_PORT_Y_MAX - VIEW_PORT_Y);
}

void MainWindowBase::removeObject()
{
	if(!this->listWidgetObjects->selectedItems().isEmpty()
			&& this->listWidgetObjects->selectedItems().at(0)->text() != wd->getName())
	{
		this->displayFiles->removeObject(this->listWidgetObjects->selectedItems().at(0)->text());
		delete this->listWidgetObjects->selectedItems().at(0);
		this->repaint();
	}
}

void MainWindowBase::rotate(int angle)
{
	if(!this->listWidgetObjects->selectedItems().isEmpty())
	{
		if(this->radioButtonCenter_Object->isChecked())
		{
			Vec3D center = this->displayFiles->getGeometricShape(this->listWidgetObjects->selectedItems().at(0)->text())->center();


			axis x2 = this->point_x2_rotate->value();
			axis y2 = this->point_y2_rotate->value();
			axis z2 = this->point_z2_rotate->value();

			Vec3D xyz2(x2, y2, z2);
			if(angle > lastAngle || (angle == 1 && lastAngle == 90))
				this->displayFiles->rotate(this->listWidgetObjects->selectedItems().at(0)->text(), 4, center, xyz2);
			else
				this->displayFiles->rotate(this->listWidgetObjects->selectedItems().at(0)->text(), -4, center, xyz2);
		}
		else //Rotacao em um ponto especifico
		{
			axis x1 = this->point_x1_rotate->value();
			axis y1 = this->point_y1_rotate->value();
			axis z1 = this->point_z1_rotate->value();

			axis x2 = this->point_x2_rotate->value();
			axis y2 = this->point_y2_rotate->value();
			axis z2 = this->point_z2_rotate->value();

			Vec3D xyz1(x1, y1, z1);
			Vec3D xyz2(x2, y2, z2);
			if(angle > lastAngle || (angle == 1 && lastAngle == 90))
				this->displayFiles->rotate(this->listWidgetObjects->selectedItems().at(0)->text(), 4, xyz1, xyz2);
			else
				this->displayFiles->rotate(this->listWidgetObjects->selectedItems().at(0)->text(), -4, xyz1, xyz2);
		}

		this->repaint();
		lastAngle = angle;
	}
}


void MainWindowBase::moveLeft()
{
	if(!this->listWidgetObjects->selectedItems().isEmpty())
	{
		this->displayFiles->translate(this->listWidgetObjects->selectedItems().at(0)->text(), Vec3D(-10,0));
		this->repaint();
	}
}

void MainWindowBase::moveRight()
{
	if(!this->listWidgetObjects->selectedItems().isEmpty())
	{
		this->displayFiles->translate(this->listWidgetObjects->selectedItems().at(0)->text(), Vec3D(10,0));
		this->repaint();
	}
}

void MainWindowBase::moveUp()
{
	if(!this->listWidgetObjects->selectedItems().isEmpty())
	{
		this->displayFiles->translate(this->listWidgetObjects->selectedItems().at(0)->text(), Vec3D(0,10));
		this->repaint();
	}
}

void MainWindowBase::moveDown()
{
	if(!this->listWidgetObjects->selectedItems().isEmpty())
	{
		this->displayFiles->translate(this->listWidgetObjects->selectedItems().at(0)->text(), Vec3D(0,-10));
		this->repaint();
	}
}

void MainWindowBase::zoomIn()
{
	if (!this->listWidgetObjects->selectedItems().isEmpty()) {

		if (displayFiles->getGeometricShapeType(this->listWidgetObjects->selectedItems().at(0)->text())
				== GeometricShape::WINDOW)
		{
			this->displayFiles->scale(this->listWidgetObjects->selectedItems().at(0)->text(),Vec3D(0.9, 0.9, 0.9));
			this->repaint();
		}

//		else {
//			this->displayFiles->scale(this->listWidgetObjects->selectedItems().at(0)->text(), Vec3D(1.1, 1.1));
//			this->repaint();
//		}
	}

}

void MainWindowBase::zoomOut()
{
	if (!this->listWidgetObjects->selectedItems().isEmpty()) {

		if (displayFiles->getGeometricShapeType(this->listWidgetObjects->selectedItems().at(0)->text())
				== GeometricShape::WINDOW)
		{
			this->displayFiles->scale(this->listWidgetObjects->selectedItems().at(0)->text(),Vec3D(1.1, 1.1, 1.1));
			this->repaint();
		}
//
//		else {
//			this->displayFiles->scale(this->listWidgetObjects->selectedItems().at(0)->text(), Vec3D(0.9, 0.9));
//			this->repaint();
//		}
	}
}


void MainWindowBase::browseColor()
{
	QColor color = QColorDialog::getColor(this->selectedColor,this);
	if(color.isValid())
		this->selectedColor = color;
	this->paintSelectedColor();
}


void MainWindowBase::increaseClippingSize() {
	this->clippingSizeFactor += 0.02;
	if(this->clippingSizeFactor > 1){
		this->clippingSizeFactor = 1;
	}
	this->repaint();
}

void MainWindowBase::decreaseClippingSize() {
	this->clippingSizeFactor -= 0.02;
	if(this->clippingSizeFactor < 0.4){
		this->clippingSizeFactor = 0.4;
	}
	this->repaint();
}

void MainWindowBase::setDefaultClippingSize() {
	this->clippingSizeFactor = 1;
	this->repaint();
}

void MainWindowBase::paintClippingRect(){

	axis xminClipp = xToVp(clipping->getXMin());
	axis yminClipp = yToVp(clipping->getYMin());

	axis xmaxClipp = xToVp(clipping->getXMax());
	axis ymaxClipp = yToVp(clipping->getYMax());

	this->scene->addRect(xminClipp, yminClipp,
			(xmaxClipp - xminClipp), (ymaxClipp - yminClipp));

}

void MainWindowBase::changeClippingAlgorithm(bool checked)
{
	if(checked)
		this->clipping = new CohenSutherlandClipping(wd);
	else
		this->clipping = new LiangBarskyClipping(wd);
}


void MainWindowBase::addVector()
{
	QTableWidgetItem* item_x = new QTableWidgetItem("0");
	QTableWidgetItem* item_y = new QTableWidgetItem("0");
	QTableWidgetItem* item_z = new QTableWidgetItem("0");
	this->tableWidgetPoligono->insertRow(this->tableWidgetPoligono->rowCount());
	this->tableWidgetPoligono->setItem(this->tableWidgetPoligono->rowCount()-1, 0, item_x);
	this->tableWidgetPoligono->setItem(this->tableWidgetPoligono->rowCount()-1, 1, item_y);
	this->tableWidgetPoligono->setItem(this->tableWidgetPoligono->rowCount()-1, 2, item_z);
}

void MainWindowBase::removeVector()
{
	this->tableWidgetPoligono->removeRow(this->tableWidgetPoligono->rowCount() - 1);
}

void MainWindowBase::addBSplineCurve() {
	if(this->lineEditBSplineName->text().isEmpty())
	{
		this->statusbar->showMessage("Por favor, digite um nome para o objeto.");
		return;
	}
	if(this->displayFiles->contains(this->lineEditBSplineName->text()))
	{
		this->statusbar->showMessage("Ja existe um objeto com esse nome, por favor digite outro.");
		return;
	}

	QString name = this->lineEditBSplineName->text() + " [B-Spline]";
	VectorList vl = VectorList();
	for(int i = 0; i < this->tableWidgetBSpline->rowCount(); i++)
	{
		QTableWidgetItem* item_x = this->tableWidgetBSpline->item(i,0);
		QTableWidgetItem* item_y = this->tableWidgetBSpline->item(i,1);
		QTableWidgetItem* item_z = this->tableWidgetBSpline->item(i,2);

		if(item_x && item_y && item_z)
			if(!item_x->text().isEmpty() && !item_y->text().isEmpty() && !item_z->text().isEmpty())
			{
				axis x = item_x->text().toDouble();
				axis y = item_y->text().toDouble();
				axis z = item_z->text().toDouble();

				Vec3D vector(x,y,z);

				vl.append(vector);
			}
	}


	if(vl.size() < 4)
	{
		this->statusbar->showMessage("Coordenadas insuficientes para formar uma B-Spline.");
		return;
	}

	this->displayFiles->addBSpline(name, vl, this->selectedColor);
	new QListWidgetItem(name, this->listWidgetObjects);
	this->lineEditBSplineName->clear();
	this->repaint();

}

void MainWindowBase::addBSplineVector() {
	QTableWidgetItem* item_x = new QTableWidgetItem("0");
	QTableWidgetItem* item_y = new QTableWidgetItem("0");
	this->tableWidgetBSpline->insertRow(this->tableWidgetBSpline->rowCount());
	this->tableWidgetBSpline->setItem(this->tableWidgetBSpline->rowCount()-1, 0, item_x);
	this->tableWidgetBSpline->setItem(this->tableWidgetBSpline->rowCount()-1, 1, item_y);
}

void MainWindowBase::removeBSplineVector() {

	if(this->tableWidgetBSpline->rowCount()>4){
		this->tableWidgetBSpline->removeRow(this->tableWidgetBSpline->rowCount() - 1);
	}
}

void MainWindowBase::importObject()
{
	QFileDialog fileLoc(0, Qt::Dialog);
	fileLoc.setFileMode(QFileDialog::ExistingFile);
	fileLoc.setViewMode(QFileDialog::Detail);
	fileLoc.resize(700, 400);

	QString file = QString::null;
	if(fileLoc.exec() == QDialog::Accepted)
	{
		QStringList files = fileLoc.selectedFiles();
		if(!files.isEmpty())
			 file = files.at(0);
	}

	if(file.isNull())
		return;
	QFileInfo fileInfo(file);
	this->displayFiles->addObject3D(file, fileInfo.fileName()+ " [Objeto 3D]" ,selectedColor);
	new QListWidgetItem(fileInfo.fileName()+ " [Objeto 3D]", this->listWidgetObjects);

	repaint();
}
















