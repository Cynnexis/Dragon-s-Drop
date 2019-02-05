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
	connect(actionCopyImage, SIGNAL(triggered()), this, SLOT(actionCopyImage_triggered()));
	
	ui->menuBar->addMenu(mDebug);
#endif
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::addHistoryRow(QString key, std::function<void(QTableWidgetItem*)> applyValue) {
	QTableWidgetItem* k = new QTableWidgetItem(key);
	QTableWidgetItem* v = new QTableWidgetItem();
	applyValue(v);
	cout << "MainWindow::addHistoryRow(QString, std::function)> Applying QTableWidgetItems" << endl;
	ui->tw_history->setRowCount(ui->tw_history->rowCount()+1);
	ui->tw_history->setItem(ui->tw_history->rowCount() - 1, 0, k);
	ui->tw_history->setItem(ui->tw_history->rowCount() - 1, 1, v);
}

void MainWindow::addHistoryRow(QString key, QString value) {
	addHistoryRow(key, [value](QTableWidgetItem* v) -> void {
		v->setText(value);
		cout << "MainWindow::addHistoryRow(QString, QString)> Applying QString" << endl;
	});
}
void MainWindow::addHistoryRow(QString value) {
	addHistoryRow(QDateTime().toString(Qt::SystemLocaleShortDate), value);
}

void MainWindow::addHistoryRow(QString key, QColor value) {
	addHistoryRow(key, [value](QTableWidgetItem* v) -> void {
		/*QFont consolas = QFont("Consolas");
		consolas.setBold(true);
		v->setFont(consolas);*/
		v->setBackgroundColor(value);
		if (value.lightness() < 180)
			v->setForeground(QBrush(Qt::white));
		cout << "MainWindow::addHistoryRow(QString, QColor)> Applying QColor" << endl;
	});
}

void MainWindow::addHistoryRow(QColor value) {
	addHistoryRow(QDateTime().toString(Qt::SystemLocaleShortDate), value);
}

void MainWindow::addHistoryRow(QString key, QImage value) {
	addHistoryRow(key, [value](QTableWidgetItem* v) -> void {
		//https://stackoverflow.com/questions/14365875/qt-cannot-put-an-image-in-a-table
		v->setData(Qt::DecorationRole, QPixmap::fromImage(value));
		cout << "MainWindow::addHistoryRow(QString, QImage)> Applying QImage" << endl;
	});
}

void MainWindow::addHistoryRow(QImage value) {
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
	addHistoryRow(color);
#ifdef QT_DEBUG
	cout << "MainWindow> Color received" << endl;
#endif
}

void MainWindow::onImageReceived(QImage image) {
	//addHistoryRow("(" + QString::number(image.rect().width()) + "x" + QString::number(image.rect().height()) + ")");
	addHistoryRow(image);
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
	data.setText("#3e85cf");
	clip->setMimeType(&data);
}

void MainWindow::actionCopyImage_triggered() {
	QImage image = QImage(100, 50, QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&image);
	painter.fillRect(image.rect(), Qt::white);
	painter.drawText(image.rect(), Qt::AlignCenter | Qt::AlignVCenter, "Hello world!");
	image.save("image.png");
	data.setImageData(image);
	clip->setMimeType(&data);
	cout << "MainWindow::actionCopyImage_triggered> Image saved" << endl;
}
#endif
