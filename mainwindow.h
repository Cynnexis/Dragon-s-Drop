#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QAbstractButton>
#include <QPushButton>
#include <QMenu>
#include <QTableWidgetItem>
#include <QDateTime>
#include <QColor>
#include <QPainter>
#include <QTimer>
#include <QDesktopServices>
#include <QMessageBox>
#include <QTemporaryFile>
#include <QTemporaryDir>
#include <QFileInfo>
#include <QColorDialog>
#include <QImageWriter>
#include <QMetaEnum>

#include "edittextclipboard.h"
#include "editimageclipboard.h"
#include "editcolorclipboard.h"
#include "clip.h"
#include "r.h"

using namespace std;


#define PRINT_TABLE_FREQUENTLY 0

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	
	void showEvent(QShowEvent*);
	
private slots:
	/**
	 * Add a row in the history table (the central widget in `MainWindow`)
	 * @param k The first widget item in the row. It is considered as the "key" cell.
	 * @param v The second widget item in the row. It is considered as the "value" cell.
	 */
	void addHistoryRow(QTableWidgetItem* k, QTableWidgetItem* v);
	/**
	 * Add a row in the history table (the central widget in `MainWindow`). The first cell is
	 * automatically added, with the current date and time as text.
	 * @param v The second widget item in the row. It is considered as the "value" cell.
	 */
	void addHistoryRow(QTableWidgetItem* v);
	/**
	 * Add a row in the history table (the central widget in `MainWindow`). The first cell is
	 * automatically added, with the current date and time as text.
	 * @param value The value as a string in the second cell.
	 */
	void addHistoryRow(QString value);
	/**
	 * Add a row in the history table (the central widget in `MainWindow`). The first cell is
	 * automatically added, with the current date and time as text.
	 * @param value The value as an URL in the second cell.
	 */
	void addHistoryRow(QUrl value);
	/**
	 * Add a row in the history table (the central widget in `MainWindow`). The first cell is
	 * automatically added, with the current date and time as text.
	 * @param value The value as a color in the second cell. The background color of the cell
	 * becomes the actual color \p value and the the foreground color is automatically changed.
	 */
	void addHistoryRow(QColor value);
	/**
	 * Add a row in the history table (the central widget in `MainWindow`). The first cell is
	 * automatically added, with the current date and time as text.
	 * @param value The value as an image in the second cell. It is resized to fit in the cell.
	 */
	void addHistoryRow(QImage value);
	
	QString getDateTimeFormat();
	
	void onDataChanged(const QMimeData*);
	void onTextReceived(QString text);
	void onHtmlReceived(QString html);
	void onUrlsReceived(QList<QUrl> urls);
	void onColorReceived(QColor color);
	void onImageReceived(QImage image);
	
	void on_tw_history_cellDoubleClicked(int row, int column);
	void on_tw_history_customContextMenuRequested(const QPoint& pos);
	
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
	void actionPrintHistoryMap_triggered();
#endif
	
	void on_actionClear_triggered();
	
private:
	Ui::MainWindow *ui = nullptr;
	Clip* clip = nullptr;
	//MapModel mapModel;
	
	QMenu* contextMenu_tw_history = nullptr;
	QAction* actionRevert = nullptr;
	
#ifdef QT_DEBUG
	QMenu* mDebug = nullptr;
	QAction* actionCopyText = nullptr;
	QAction* actionCopyHtml = nullptr;
	QAction* actionCopyUrl = nullptr;
	QAction* actionCopyColor = nullptr;
	QAction* actionCopyImage = nullptr;
	QAction* actionPrintHistoryMap = nullptr;
	QMimeData data;
	QTimer* timer = nullptr;
#endif
};

#endif // MAINWINDOW_H
