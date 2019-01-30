#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow) {
	ui->setupUi(this);
	clip = Clip::getInstance(this);
	
	connect(clip, SIGNAL(dataChanged(const QMimeData*)), this, SLOT(onDataChanged(const QMimeData*)));
	connect(clip, SIGNAL(textReceived(QString)), this, SLOT(onTextReceived(QString)));
	connect(clip, SIGNAL(htmlReceived(QString)), this, SLOT(onHtmlReceived(QString)));
	connect(clip, SIGNAL(urlsReceived(QList<QUrl>)), this, SLOT(onUrlsReceived(QList<QUrl>)));
	connect(clip, SIGNAL(colorReceived(QColor)), this, SLOT(onColorReceived(QColor)));
	connect(clip, SIGNAL(imageReceived(QImage)), this, SLOT(onImageReceived(QImage)));
	
#ifdef QT_DEBUG
	mDebug = new QMenu("Debug", ui->menuBar);
	actionCopyText = new QAction("Copy text", mDebug);
	actionCopyHtml = new QAction("Copy HTML", mDebug);
	actionCopyUrl = new QAction("Copy URL", mDebug);
	actionCopyColor = new QAction("Copy color", mDebug);
	actionCopyImage = new QAction("Copy image", mDebug);
	
	mDebug->addAction(actionCopyText);
	mDebug->addAction(actionCopyHtml);
	mDebug->addAction(actionCopyUrl);
	mDebug->addAction(actionCopyColor);
	mDebug->addAction(actionCopyImage);
	
	connect(actionCopyText, SIGNAL(triggered()), this, SLOT(actionCopyText_triggered()));
	connect(actionCopyHtml, SIGNAL(triggered()), this, SLOT(actionCopyHtml_triggered()));
	connect(actionCopyUrl, SIGNAL(triggered()), this, SLOT(actionCopyUrl_triggered()));
	connect(actionCopyColor, SIGNAL(triggered()), this, SLOT(actionCopyColor_triggered()));
	
	ui->menuBar->addMenu(mDebug);
#endif
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::addHistoryRow(QString key, QString value) {
	QTableWidgetItem* k = new QTableWidgetItem(key);
	QTableWidgetItem* v = new QTableWidgetItem(value);
	ui->tw_history->setRowCount(ui->tw_history->rowCount()+1);
	ui->tw_history->setItem(ui->tw_history->rowCount() - 1, 0, k);
	ui->tw_history->setItem(ui->tw_history->rowCount() - 1, 1, v);
}

void MainWindow::addHistoryRow(QString value) {
	addHistoryRow(QDateTime().toString(Qt::SystemLocaleShortDate), value);
}

void MainWindow::onDataChanged(const QMimeData* data) {
}

void MainWindow::onTextReceived(QString text) {
	addHistoryRow(text);
#ifdef QT_DEBUG
	cout << "MainWindow> Text received" << endl;
#endif
}

void MainWindow::onHtmlReceived(QString html) {
	addHistoryRow(html);
#ifdef QT_DEBUG
	cout << "MainWindow> Html received" << endl;
#endif
}

void MainWindow::onUrlsReceived(QList<QUrl> urls) {
	QString str = "";
	for (int i = 0, max = urls.count() ; i < max ; i++) {
		str += urls.at(i).toString();
		if (i < max-1)
			str += "\n";
	}
	addHistoryRow(str);
#ifdef QT_DEBUG
	cout << "MainWindow> Urls received" << endl;
#endif
}

void MainWindow::onColorReceived(QColor color) {
	addHistoryRow(color.name());
#ifdef QT_DEBUG
	cout << "MainWindow> Color received" << endl;
#endif
}

void MainWindow::onImageReceived(QImage image) {
	addHistoryRow("(" + QString::number(image.size().width()) + "x" + QString::number(image.size().height()) + ")");
#ifdef QT_DEBUG
	cout << "MainWindow> Image received" << endl;
#endif
}

#ifdef QT_DEBUG
void MainWindow::actionCopyText_triggered() {
	clip->setText("This is a text");
}

void MainWindow::actionCopyHtml_triggered() {
	data.setData("text/html", QString("<html><body><h1>This is HTML</h1></body></html>").toUtf8());
	clip->setMimeType(&data);
}

void MainWindow::actionCopyUrl_triggered() {
	data.setData("text/uri-list", QString("https://www.google.com").toUtf8());
	clip->setMimeType(&data);
}

void MainWindow::actionCopyColor_triggered() {
	data.setData("text/plain", QString("#ffffff").toUtf8());
	clip->setMimeType(&data);
}

void MainWindow::actionCopyImage_triggered() {
	data.setData("image/png", QString("").toUtf8());
	clip->setMimeType(&data);
}
#endif
