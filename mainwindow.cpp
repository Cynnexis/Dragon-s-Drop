#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow) {
	ui->setupUi(this);
	clip = Clip::getInstance(this);
	
#ifdef QT_DEBUG
	mDebug = new QMenu("Debug", ui->menuBar);
	actionCopyText = new QAction("Copy text", mDebug);
	actionCopyHtml = new QAction("Copy HTML", mDebug);
	actionCopyUrl = new QAction("Copy URL", mDebug);
	actionCopyColor = new QAction("Copy color", mDebug);
	actionCopyImage = new QAction("Copy image", mDebug);
#endif
}

MainWindow::~MainWindow() {
	delete ui;
}

#ifdef QT_DEBUG
void MainWindow::actionCopyText_toggled() {
	clip->setText("This is a text");
}

void MainWindow::actionCopyHtml_toggled() {
	data.setData("text/html", QString("<html><body><h1>This is HTML</h1></body></html>").toUtf8());
	clip->setMimeType(&data);
}

void MainWindow::actionCopyUrl_toggled() {
	data.setData("text/plain", QString("https://www.google.com").toUtf8());
	clip->setMimeType(&data);
}

void MainWindow::actionCopyColor_toggled() {
	data.setData("text/plain", QString("#ffffff").toUtf8());
	clip->setMimeType(&data);
}

void MainWindow::actionCopyImage_toggled() {
	//data.setData("text/plain");
	//clip->setMimeType(&data);
}
#endif
