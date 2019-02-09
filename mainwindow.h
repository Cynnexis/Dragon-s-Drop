#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QMenu>
#include <QTableWidgetItem>
#include <QDateTime>
#include <QColor>
#include <QPainter>
#include <QTimer>

#include "clip.h"
//#include "mapmodel.h"

using namespace std;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	
private slots:
	void addHistoryRow(QTableWidgetItem* k, QTableWidgetItem* v);
	void addHistoryRow(QTableWidgetItem* v);
	void addHistoryRow(QString value);
	void addHistoryRow(QColor value);
	void addHistoryRow(QImage value);
	
	void onDataChanged(const QMimeData*data);
	void onTextReceived(QString text);
	void onHtmlReceived(QString html);
	void onUrlsReceived(QList<QUrl> urls);
	void onColorReceived(QColor color);
	void onImageReceived(QImage image);
	
#ifdef QT_DEBUG
	void actionCopyText_triggered();
	void actionCopyHtml_triggered();
	void actionCopyUrl_triggered();
	void actionCopyColor_triggered();
	void actionCopyImage_triggered();
#endif
	
private:
	Ui::MainWindow *ui;
	Clip* clip = nullptr;
	//MapModel mapModel;
	
#ifdef QT_DEBUG
	QMenu* mDebug;
	QAction* actionCopyText;
	QAction* actionCopyHtml;
	QAction* actionCopyUrl;
	QAction* actionCopyColor;
	QAction* actionCopyImage;
	QMimeData* data;
	QTimer* timer;
#endif
};

#endif // MAINWINDOW_H
