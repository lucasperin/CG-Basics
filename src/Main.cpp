//============================================================================
// Name        : trabalhoCg.cpp
// Author      : Perin&Gaspar
// Version     :
// Copyright   : Your copyright notice
//============================================================================

#include <iostream>
using namespace std;
#include "ui/MainWindowBase.h"
#include <QtGui/QApplication>
#include "geometricObject/WireFrame.h"

int main(int argc, char** argv) {
	QApplication app(argc, argv);

	DisplayFiles* displayFiles = new DisplayFiles();
	MainWindowBase mainWindow(displayFiles);
	mainWindow.show();
	app.exec();
	return 0;
}
