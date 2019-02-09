#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow) {
	ui->setupUi(this);
	clip = Clip::getInstance(this);
	data = new QMimeData();
	
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
	
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, [this]() -> void {
		cout << "(" << QDateTime::currentDateTime().toString("HH:mm:ss").toStdString() << ") Table:\n";
		for (int row = 0, rmax = this->ui->tw_history->rowCount(); row < rmax; row++) {
			for (int col = 0, cmax = this->ui->tw_history->columnCount(); col < cmax; col++) {
				cout << "[" << this->ui->tw_history->item(row, col)->text().replace("\n", "").toStdString() << "]" << (col < cmax-1 ? "\t" : "");
			}
			cout << "\n";
		}
		cout << endl << flush;
		cout.flush();
	});
	timer->setInterval(5000);
	timer->start(5000);
#endif
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::addHistoryRow(QTableWidgetItem* k, QTableWidgetItem* v) {
	ui->tw_history->setRowCount(ui->tw_history->rowCount()+1);
	ui->tw_history->setItem(ui->tw_history->rowCount() - 1, 0, k);
	ui->tw_history->setItem(ui->tw_history->rowCount() - 1, 1, v);
}

void MainWindow::addHistoryRow(QTableWidgetItem* v) {
	addHistoryRow(new QTableWidgetItem(QDateTime::currentDateTime().toString("dd/MM/yy HH:mm:ss")), v);
}
void MainWindow::addHistoryRow(QString value) {
	addHistoryRow(new QTableWidgetItem(value));
}

void MainWindow::addHistoryRow(QUrl value) {
	addHistoryRow(new QTableWidgetItem(value.toString()));
}

void MainWindow::addHistoryRow(QColor value) {
	QTableWidgetItem* v = new QTableWidgetItem(value.name());
	QFont consolas = QFont("Consolas");
	consolas.setBold(true);
	v->setFont(consolas);
	v->setBackgroundColor(value);
	if (value.lightness() < 180)
		v->setForeground(QBrush(Qt::white));
	addHistoryRow(v);
}

void MainWindow::addHistoryRow(QImage value) {
	QTableWidgetItem* v = new QTableWidgetItem();
	//https://stackoverflow.com/questions/14365875/qt-cannot-put-an-image-in-a-table
	v->setData(Qt::DecorationRole, QPixmap::fromImage(value).scaled(100, 100, Qt::KeepAspectRatio));
	addHistoryRow(v);
}

void MainWindow::onDataChanged(const QMimeData* data) {
}

void MainWindow::onTextReceived(QString text) {
	addHistoryRow(text);
}

void MainWindow::onHtmlReceived(QString html) {
	addHistoryRow(html);
}

void MainWindow::onUrlsReceived(QList<QUrl> urls) {
	QString str = "";
	for (int i = 0, max = urls.count() ; i < max ; i++) {
		str += urls.at(i).toString();
		if (i < max-1)
			str += "\n";
	}
	addHistoryRow(str);
}

void MainWindow::onColorReceived(QColor color) {
	addHistoryRow(color);
}

void MainWindow::onImageReceived(QImage image) {
	addHistoryRow(image);
}

void MainWindow::on_tw_history_cellDoubleClicked(int row, int column) {
	QString url = ui->tw_history->item(row, column)->text();
    if (Clip::isUrlValid(url))
		QDesktopServices::openUrl(url);
}

#ifdef QT_DEBUG
void MainWindow::actionCopyText_triggered() {
	data->clear();
	clip->setText("This is a text");
	cout << "MainWindow::actionCopyText_triggered> Text saved" << endl;
}

void MainWindow::actionCopyHtml_triggered() {
	QString html = QString("<html>\n"
						   "<body>\n"
						   "<!--StartFragment--><span style=\"color: rgb(84, 84, 84); font-family: arial, sans-serif; font-size: 14px; font-style: normal; font-variant-ligatures: normal; font-variant-caps: normal; font-weight: 400; letter-spacing: normal; orphans: 2; text-align: left; text-indent: 0px; text-transform: none; white-space: normal; widows: 2; word-spacing: 0px; -webkit-text-stroke-width: 0px; background-color: rgb(255, 255, 255); text-decoration-style: initial; text-decoration-color: initial; display: inline !important; float: none;\">Hello world</span><!--EndFragment-->\n"
						   "</body>\n"
						   "</html>");
	data->clear();
	data->setData("text/html", html.toUtf8());
	clip->setMimeType(data);
	cout << "MainWindow::actionCopyHtml_triggered> Html saved" << endl;
}

void MainWindow::actionCopyUrl_triggered() {
	data->clear();
	data->setText("https://www.google.com");
	data->setData("text/uri-list", QString("https://www.google.com").toUtf8());
	clip->setMimeType(data);
	cout << "MainWindow::actionCopyUrl_triggered> Url saved" << endl;
}

void MainWindow::actionCopyColor_triggered() {
	data->clear();
	data->setText("#3e85cf");
	clip->setMimeType(data);
	cout << "MainWindow::actionCopyColor_triggered> Color saved" << endl;
}

void MainWindow::actionCopyImage_triggered() {
	QImage image = QImage(100, 50, QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&image);
	painter.fillRect(image.rect(), Qt::white);
	painter.drawText(image.rect(), Qt::AlignCenter | Qt::AlignVCenter, "Hello world!");
	image.save("image.png");
	data->clear();
	data->setImageData(image);
	clip->setMimeType(data);
	cout << "MainWindow::actionCopyImage_triggered> Image saved" << endl;
}
#endif
