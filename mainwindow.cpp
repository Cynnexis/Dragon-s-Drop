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
	
	ui->menuFile->setTitle(QApplication::applicationName());
	
	ui->actionSettings->setIcon(R::getSettings());
	ui->actionExit->setIcon(R::getPower());
	
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

void MainWindow::showEvent(QShowEvent*) {
	clip->reprocessClipboardContent();
}

void MainWindow::editClipboard(QString fileExtension,
							   std::function<bool(QFile*)> write,
							   std::function<bool(QFile*)> read,
							   QIODevice::OpenMode writeFlags,
							   QIODevice::OpenMode readFlags) {
	// See https://doc.qt.io/qt-5/qtemporarydir.html
    //QTemporaryFile f(R::getTemporaryFileTemplate("txt"));
	QTemporaryDir dir(R::getTemporaryDirTemplate());
	QString path = dir.path() + (!dir.path().endsWith("/") ? "/" : "");
	
	if (!fileExtension.startsWith("."))
		fileExtension = "." + fileExtension;
	
	QString filepath = path + "dragonsdrop" + QString::number(QDateTime::currentDateTime().toTime_t()) + fileExtension;
	
	if (dir.isValid()) {
		// Create file
		QFile* f = new QFile(filepath);
		
		if (!f->open(writeFlags)) {
			QMessageBox::critical(this, tr("Error"), tr("An error occured while creating the temporary file."));
			return;
		}
		
		// Write file
		if (!write(f))
			return;
		
		f->close();
		
		// Open the app associated to the text files
		QString url = "file://" + QString(filepath.startsWith("/") ? "" : "/") + filepath;
#ifdef QT_DEBUG
		cout << url.toStdString() << endl;
#endif
		QDesktopServices::openUrl(url);
		
		// Display message box
		QMessageBox box(this);
		box.setIconPixmap(R::getDragonsDropIcon());
		box.setWindowTitle(tr("Edit text..."));
		box.setText(tr("Press 'Operation finished' when you finished the operation.", "The name of the button must also be translated"));
		box.setStandardButtons(QMessageBox::Ok);
		box.setButtonText(QMessageBox::Ok, tr("Operation finished"));
		box.setModal(true);
		box.exec();
		
		f = new QFile(filepath);
		
		if (!f->open(readFlags)) {
			QMessageBox::critical(this, tr("Error"), tr("An error occured while creating the temporary file."));
			return;
		}
		
		// Read file.
		if (!read(f))
			return;
		
		f->close();
	}
	else
		QMessageBox::critical(this, tr("Error"), tr("An error occured while creating the temporary directory."));
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

void MainWindow::on_actionExit_triggered() {
    qApp->exit();
}

void MainWindow::on_actionEdit_clipboard_as_text_triggered() {
	editClipboard(".txt",
	[this](QFile* f) {
		// Write
		QTextStream out(f);
		out << clip->getText() << endl;
		out.flush();
		return true;
	},
	[this](QFile* f) {
		// Read
		QString result = "";
		QTextStream in(f);
		in.seek(0);
		while (!in.atEnd())
			result += in.readLine() + "\n";
		
		// Remove last "\n"
		if (result.endsWith("\n"))
			result.remove(QRegularExpression("\\n$"));
#ifdef QT_DEBUG
		cout << "Result: " << result.toStdString() << endl;
#endif
		clip->setText(result);
		return true;
	});
}

void MainWindow::on_actionEdit_clipboard_as_color_triggered() {
	QColor* initialColor = nullptr;
	if (QColor::isValidColor(clip->getText()))
		initialColor = new QColor(clip->getText());
	else
		initialColor = new QColor(Qt::black);
	
	QColor newColor = QColorDialog::getColor(*initialColor, this, "Pick a new color", QColorDialog::ShowAlphaChannel);
	
	if (newColor.isValid())
		clip->setText(newColor.name());
}

void MainWindow::on_actionEdit_clipboard_as_image_triggered() {
	editClipboard(".png",
	[this](QFile* f) {
		// Write
		if (clip->getImage().isNull()) {
			QMessageBox::warning(this, tr("Error"), tr("No image in the clipboard"));
			return false;
		}
		else {
#ifdef QT_DEBUG
			cout << "MainWindow> Saving image to file: " << clip->getImage().size().width() << "x" << clip->getImage().size().height() << endl;
#endif
			QImageWriter writer(f, "PNG");
			
			if (!writer.canWrite()) {
				QMessageBox::critical(this, "Error", "Cannot write the temporary image file. You should execute this application with higher permissions.");
				f->close();
				return false;
			}
			
			writer.write(clip->getImage());
			
			return true;
		}
	},
	[this](QFile* f) {
		// Read
		
		// Reset position carret
		QTextStream in(f);
		in.seek(0);
		
		// Load the image
		QImage image(QFileInfo(*f).absoluteFilePath());
		
		if (image.isNull()) {
			QMessageBox::warning(this, tr("Error"), tr("Cannot load back the image. Did you delete the file?"));
		}
		else {
#ifdef QT_DEBUG
			cout << "Result: " << image.size().width() << "x" << image.size().height() << endl;
#endif
			clip->setImage(image);
		}
		
		return true;
	},
	QIODevice::WriteOnly,
	QIODevice::ReadOnly);
}

void MainWindow::on_actionAbout_Dragon_s_Drop_triggered() {
    QMessageBox::about(this, ui->actionAbout_Dragon_s_Drop->text(),
					   "<p><b>Dragon's Drop</b> is an application that can synchronize your clipboard on multiple devices! It is written in C++ with <a href='https://www.qt.io/'>Qt</a> Framework.</p>"
					   "<h2>Functionalities</h2>"
					   "<ul>"
					   "<li><b>Clipboard Manager:</b> You can manage your clipboard from here.</li>"
					   "<li><b>Clipboard History:</b> What did you just copy? Check it out on the app!</li>"
					   "<li><b>Edit your clipboard:</b> Open what's on yur clipboard, edit it, and paste it everywhere!</li>"
					   "<li><b>Sync:</b> Copy your data from one devie, and paste it on another one!</li>"
					   "</ul>"
					   "<h2>Built with</h2>"
					   "<p>This project has been built with:</p>"
					   "<ul>"
					   "<li><b><a href='https://www.qt.io/'>Qt</a>:</b> The best C++ framework to create GUI application.</li>"
					   "<li><b><a href='https://atom.io/'>Atom</a>:</b> The hackable text editor for the 21st century.</li>"
					   "<li><b><a href='https://desktop.github.com/'>GitHub Desktop</a>:</b> GitHub client software. Highly use-friendly.</li>"
					   "<li><b>Hope:</b> Because we can't do anything without it!</li>"
					   "</ul>"
					   "<h2>Versioning</h2>"
					   "<p>The project uses Git as a version control system, and <a href='https://github.com/'>GitHub</a> as a hub. <a href='https://desktop.github.com/'>GitHub Desktop</a> as been used as a client.</p>"
					   "<h2>Author</h2>"
					   "<p>This project has been made by <b>Valentin Berger</b> (username: <a href='https://github.com/Cynnexis/'>Cynnexis</a>)</p>"
					   "<h2>License</h2>"
					   "<p>This project is under the <b>GNU Affero General Public License v3.0</b> - see <a href='https://github.com/Cynnexis/Dragon-s-Drop/blob/master/LICENSE.txt'>LICENSE.txt</a> for more detail (I know that nobody will look at this file, licenses are so boring...)</p>"
					   );
}

void MainWindow::on_actionAbout_Qt_triggered() {
    QMessageBox::aboutQt(this, ui->actionAbout_Qt->text());
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
	image.fill(Qt::white);
	QPainter painter(&image);
	painter.fillRect(image.rect(), Qt::white);
	painter.drawText(image.rect(), Qt::AlignCenter | Qt::AlignVCenter, "Hello world!");
	painter.end();
	image.save("image.png");
	data->clear();
	data->setImageData(image);
	clip->setMimeType(data);
	cout << "MainWindow::actionCopyImage_triggered> Image saved" << endl;
}
#endif
