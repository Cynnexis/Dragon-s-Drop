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
#include <QTemporaryDir>
#include <QFileInfo>
#include <QColorDialog>

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
	
	void showEvent(QShowEvent*);
	
	/**
	 * Edit the clipboard content by creating a temporary file containing the clipboard data,
	 * open the default application associated with the given file extension, and then load
	 * again the file and apply the data to the clipboard.
	 * 
	 * This is an example of the method call using lambda functions:
	 * @code{.cpp}
	 * editClipboard(".txt",
	 * 	[this](QFile* f) {
	 * 		// Write
	 * 		f->write(clip->getText().toUtf8());
	 * 		return true;
	 * 	},
	 * 	[this](QFile* f) {
	 * 		// Read
	 * 		clip->setText(f->readAll());
	 * 		return true;
	 * 	}
	 * );
	 * @endcode
	 * @param fileExtension The file extension that can open the corresponding application.
	 * @param write Function that write the clipboard content in the given `QFile`. This file object is already instanciated and open. Please do not close the file inside this function. Returns `true` by default, `false` if there is an exception and the method `editClipboard()` must stop right after the execution of `write()`.
	 * @param read Function that read the given `QFile` content to the clipboard. This file object is already instanciated and open. Please do not close the file inside this function. Returns `true` by default, `false` if there is an exception and the method `editClipboard()` must stop right after the execution of `write()`.
	 */
	void editClipboard(QString fileExtension, std::function<bool(QFile*)> write, std::function<bool(QFile*)> read);
	
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
	
	void onDataChanged(const QMimeData*);
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
