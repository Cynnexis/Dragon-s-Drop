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
	
	ui->menuFile->setTitle(QApplication::applicationName());
	
	ui->actionSettings->setIcon(R::getSettings());
	ui->actionExit->setIcon(R::getPower());
	
	// Context menu for tw_history
	contextMenu_tw_history = new QMenu(this);
	actionRevert = new QAction("Revert", this);
	
	contextMenu_tw_history->addAction(actionRevert);
	
#ifdef QT_DEBUG
	mDebug = new QMenu("Debug", ui->menuBar);
	actionCopyText = new QAction("Copy text", mDebug);
	actionCopyHtml = new QAction("Copy HTML", mDebug);
	actionCopyUrl = new QAction("Copy URL", mDebug);
	actionCopyColor = new QAction("Copy color", mDebug);
	actionCopyImage = new QAction("Copy image", mDebug);
	actionPrintHistoryMap = new QAction("Print history map", mDebug);
	
	mDebug->addAction(actionCopyText);
	mDebug->addAction(actionCopyHtml);
	mDebug->addAction(actionCopyUrl);
	mDebug->addAction(actionCopyColor);
	mDebug->addAction(actionCopyImage);
	mDebug->addSeparator();
	mDebug->addAction(actionPrintHistoryMap);
	
	connect(actionCopyText, SIGNAL(triggered()), this, SLOT(actionCopyText_triggered()));
	connect(actionCopyHtml, SIGNAL(triggered()), this, SLOT(actionCopyHtml_triggered()));
	connect(actionCopyUrl, SIGNAL(triggered()), this, SLOT(actionCopyUrl_triggered()));
	connect(actionCopyColor, SIGNAL(triggered()), this, SLOT(actionCopyColor_triggered()));
	connect(actionCopyImage, SIGNAL(triggered()), this, SLOT(actionCopyImage_triggered()));
	connect(actionPrintHistoryMap, SIGNAL(triggered()), this, SLOT(actionPrintHistoryMap_triggered()));
	
	ui->menuBar->addMenu(mDebug);
	
#if PRINT_TABLE_FREQUENTLY != 0
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
#endif
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::showEvent(QShowEvent*) {
	clip->reprocessClipboardContent();
}

void MainWindow::addHistoryRow(QTableWidgetItem* k, QTableWidgetItem* v) {
	ui->tw_history->setRowCount(ui->tw_history->rowCount()+1);
	ui->tw_history->setItem(ui->tw_history->rowCount() - 1, 0, k);
	ui->tw_history->setItem(ui->tw_history->rowCount() - 1, 1, v);
}

void MainWindow::addHistoryRow(QTableWidgetItem* v) {
	// Get last key in the history
	qint64 lastKey = clip->getHistory()->lastKey();
	addHistoryRow(new QTableWidgetItem(QDateTime::fromMSecsSinceEpoch(lastKey).toString(getDateTimeFormat())), v);
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
	if (value.lightness() < 170)
		v->setForeground(QBrush(Qt::white));
	addHistoryRow(v);
}

void MainWindow::addHistoryRow(QImage value) {
	QTableWidgetItem* v = new QTableWidgetItem();
	//https://stackoverflow.com/questions/14365875/qt-cannot-put-an-image-in-a-table
	v->setData(Qt::DecorationRole, QPixmap::fromImage(value).scaled(100, 100, Qt::KeepAspectRatio));
	addHistoryRow(v);
}

QString MainWindow::getDateTimeFormat() {
	return "dd/MM/yyyy HH:mm:ss";
}

void MainWindow::onDataChanged(const QMimeData*) {
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

void MainWindow::on_tw_history_customContextMenuRequested(const QPoint &pos) {
    QModelIndex model = ui->tw_history->indexAt(pos);
	
	// Get index in Clip::history
	QDateTime date = QDateTime::fromString(ui->tw_history->item(model.row(), 0)->text(), getDateTimeFormat());
	uint index = date.toTime_t();
	
#ifdef QT_DEBUG
	cout << "on_tw_history_customContextMenuRequested> Index = \n\traw = " << ui->tw_history->item(model.row(), 0)->text().toStdString() << "\n\t" <<
			"parsed (epoch) = " << index << "\n\t" <<
			"parsed (date) = " << date.toString(getDateTimeFormat()).toStdString() << endl;
#endif
	
	// Check the index exists in the map
	if (clip->getHistory()->contains(index)) {
		QVariant variant = clip->getHistory()->value(index, QVariant(QVariant::Type::Invalid));
		
		if (variant.isValid() && variant.type() != QVariant::Type::Invalid) {
#ifdef QT_DEBUG
			if (variant.type() == QVariant::Type::String)
				cout << "on_tw_history_customContextMenuRequested> The variant is a string and its value is \"" << variant.toString().toStdString() << "\"." << endl;
#endif
			
			actionRevert->disconnect();
			connect(actionRevert, &QAction::triggered, this, [this, variant]() -> void {
				QMimeData* mime = Clip::variantToMimeData(variant);
				
				if (mime != nullptr)
					clip->setMimeType(Clip::variantToMimeData(variant));
#ifdef QT_DEBUG
				else
					cout << "on_tw_history_customContextMenuRequested> Cannot convert the QVariant instance to QMimeData*. QVariant type: " << variant.type() << endl;
#endif
			});
			
			contextMenu_tw_history->exec(mapToGlobal(pos), nullptr);
		}
#ifdef QT_DEBUG
		else
			cout << "on_tw_history_customContextMenuRequested> QVariant is invalid." << endl;
#endif
	}
#ifdef QT_DEBUG
	else
		cout << "on_tw_history_customContextMenuRequested> The history map does not contain an index of '" << index << "' (" << date.toString(getDateTimeFormat()).toStdString() << ")" << endl;
#endif
}

void MainWindow::on_actionExit_triggered() {
    qApp->exit();
}

void MainWindow::on_actionEdit_clipboard_as_text_triggered() {
	EditTextClipboard editText(this);
	editText.edit(".txt");
}

void MainWindow::on_actionEdit_clipboard_as_color_triggered() {
	EditColorClipboard editColor(this);
	editColor.edit();
}

void MainWindow::on_actionEdit_clipboard_as_image_triggered() {
	EditImageClipboard editImage(this);
	editImage.edit(".png");
}

void MainWindow::on_actionClear_triggered()
{
    QMessageBox box(this);
	box.setIcon(QMessageBox::Icon::Question);
	box.setWindowTitle(tr("Clear clipboard"));
	box.setText(tr("Are you sure you want to clear the clipboard?"));
	box.setWindowModality(Qt::WindowModality::ApplicationModal);
	QPushButton* b_clearClipboard = box.addButton(tr("Yes, clear the clipboard"), QMessageBox::ButtonRole::YesRole);
	QPushButton* b_clearClipboardHistory = box.addButton(tr("Yes, clear the clipboard and the history"), QMessageBox::ButtonRole::YesRole);
	box.addButton(QMessageBox::Abort);
	box.exec();
	
	if (box.clickedButton() == b_clearClipboard)
		clip->clear();
	else if (box.clickedButton() == b_clearClipboardHistory) {
		clip->clear();
		clip->getHistory()->clear();
		ui->tw_history->clear();
		ui->tw_history->setRowCount(0);
	}
}

void MainWindow::on_actionAbout_Dragon_s_Drop_triggered() {
	QMessageBox::about(this, ui->actionAbout_Dragon_s_Drop->text(), R::getREADMEHtml());
}

void MainWindow::on_actionAbout_Qt_triggered() {
    QMessageBox::aboutQt(this, ui->actionAbout_Qt->text());
}

#ifdef QT_DEBUG
void MainWindow::actionCopyText_triggered() {
	data.clear();
	clip->setText("This is a text");
	cout << "MainWindow::actionCopyText_triggered> Text saved" << endl;
}

void MainWindow::actionCopyHtml_triggered() {
	QString html = QString("<html>\n"
						   "<body>\n"
						   "<!--StartFragment--><span style=\"color: rgb(84, 84, 84); font-family: arial, sans-serif; font-size: 14px; font-style: normal; font-variant-ligatures: normal; font-variant-caps: normal; font-weight: 400; letter-spacing: normal; orphans: 2; text-align: left; text-indent: 0px; text-transform: none; white-space: normal; widows: 2; word-spacing: 0px; -webkit-text-stroke-width: 0px; background-color: rgb(255, 255, 255); text-decoration-style: initial; text-decoration-color: initial; display: inline !important; float: none;\">Hello world</span><!--EndFragment-->\n"
						   "</body>\n"
						   "</html>");
	data.clear();
	data.setData("text/html", html.toUtf8());
	clip->setMimeType(&data);
	cout << "MainWindow::actionCopyHtml_triggered> Html saved" << endl;
}

void MainWindow::actionCopyUrl_triggered() {
	data.clear();
	data.setText("https://www.google.com");
	data.setData("text/uri-list", QString("https://www.google.com").toUtf8());
	clip->setMimeType(&data);
	cout << "MainWindow::actionCopyUrl_triggered> Url saved" << endl;
}

void MainWindow::actionCopyColor_triggered() {
	data.clear();
	data.setText("#3e85cf");
	clip->setMimeType(&data);
	cout << "MainWindow::actionCopyColor_triggered> Color saved" << endl;
}

void MainWindow::actionCopyImage_triggered() {
	QImage image = QImage(100, 50, QImage::Format_ARGB32_Premultiplied);
	image.fill(Qt::white);
	QPainter painter(&image);
	painter.fillRect(image.rect(), Qt::white);
	painter.drawText(image.rect(), Qt::AlignCenter | Qt::AlignVCenter, "Hello world!");
	painter.end();
	data.clear();
	data.setImageData(image);
	clip->setMimeType(&data);
	cout << "MainWindow::actionCopyImage_triggered> Image saved" << endl;
}

void MainWindow::actionPrintHistoryMap_triggered() {
	cout << endl;
	
	for (qint64 key : clip->getHistory()->keys()) {
		cout << QDateTime::fromMSecsSinceEpoch(key).toString(getDateTimeFormat()).toStdString() << " (" << key << ")\t->\t";
		QVariant v = clip->getHistory()->value(key, QVariant(""));
		if (v.isValid() && v.type() == QVariant::Type::String)
			cout << "'" << v.toString().toStdString() << "'";
		else
			cout << "<not_string>";
		cout << endl;
	}
	
	cout << endl;
}
#endif
