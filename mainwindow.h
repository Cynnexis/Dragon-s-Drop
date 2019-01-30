#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>

#include "clip.h"
//#include "mapmodel.h"

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
#ifdef QT_DEBUG
	void actionCopyText_toggled();
	void actionCopyHtml_toggled();
	void actionCopyUrl_toggled();
	void actionCopyColor_toggled();
	void actionCopyImage_toggled();
#endif
	
private:
	Ui::MainWindow *ui;
	Clip* clip = nullptr;
	//MapModel<uint, QMimeData> mapModel;
	
#ifdef QT_DEBUG
	QMenu* mDebug;
	QAction* actionCopyText;
	QAction* actionCopyHtml;
	QAction* actionCopyUrl;
	QAction* actionCopyColor;
	QAction* actionCopyImage;
	QMimeData data;
#endif
};

#endif // MAINWINDOW_H
