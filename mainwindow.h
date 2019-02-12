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
#include <QDesktopServices>
#include <QMessageBox>
#include <QTemporaryFile>

#include "clip.h"
#include "r.h"

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
	void addHistoryRow(QUrl value);
	void addHistoryRow(QColor value);
	void addHistoryRow(QImage value);
	
	void onDataChanged(const QMimeData*data);
	void onTextReceived(QString text);
	void onHtmlReceived(QString html);
	void onUrlsReceived(QList<QUrl> urls);
	void onColorReceived(QColor color);
	void onImageReceived(QImage image);
	
	void on_tw_history_cellDoubleClicked(int row, int column);
	
	void on_actionExit_triggered();
	void on_actionEdit_clipboard_as_text_triggered();
	void on_actionEdit_clipboard_as_color_triggered();
	void on_actionEdit_clipboard_as_image_triggered();
	void on_actionAbout_Dragon_s_Drop_triggered();
	void on_actionAbout_Qt_triggered();
	
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
