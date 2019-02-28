#include "editclipboard.h"

EditClipboard::EditClipboard(QWidget* parent) : QObject (parent) {
	clip = Clip::getInstance(parent);
}

EditClipboard::~EditClipboard() {
}

void EditClipboard::edit(QString fileExtension) {
	// See https://doc.qt.io/qt-5/qtemporarydir.html
    //QTemporaryFile f(R::getTemporaryFileTemplate("txt"));
	QTemporaryDir dir(R::getTemporaryDirTemplate());
	QString path = dir.path() + (!dir.path().endsWith("/") ? "/" : "");
	
	if (!fileExtension.startsWith("."))
		fileExtension = "." + fileExtension;
	
	QString filepath = path + "dragonsdrop" + QString::number(QDateTime::currentDateTime().toSecsSinceEpoch()) + fileExtension;
	
	if (dir.isValid()) {
		// Create file
		QFile* f = new QFile(filepath);
		
		if (!f->open(writeFlags)) {
			QMessageBox::critical(qobject_cast<QWidget*>(parent()), tr("Error"), tr("An error occured while creating the temporary file."));
			return;
		}
		
		// Write file
		if (!write(f))
			return;
		
		f->close();
		
		// Open the app associated to the text files
		QString url = "file://" + QString(filepath.startsWith("/") ? "" : "/") + filepath;
#ifdef QT_DEBUG
		cout << "EditClipboard::edit> URL to open associated file: " << url.toStdString() << endl;
#endif
		QDesktopServices::openUrl(url);
		
		// Display message box
		QMessageBox box(qobject_cast<QWidget*>(parent()));
		box.setIconPixmap(R::getDragonsDropIcon());
		box.setWindowTitle(tr("Edit text..."));
		box.setText(tr("Press 'Operation finished' when you finished the operation.", "The name of the button must also be translated"));
		box.setStandardButtons(QMessageBox::Ok);
		box.setButtonText(QMessageBox::Ok, tr("Operation finished"));
		box.setModal(true);
		box.exec();
		
		f = new QFile(filepath);
		
		if (!f->open(readFlags)) {
			QMessageBox::critical(qobject_cast<QWidget*>(parent()), tr("Error"), tr("An error occured while creating the temporary file."));
			return;
		}
		
		// Read file.
		if (!read(f))
			return;
		
		f->close();
	}
	else
		QMessageBox::critical(qobject_cast<QWidget*>(parent()), tr("Error"), tr("An error occured while creating the temporary directory."));
}

/* GETTERS & SETTERS */

QIODevice::OpenMode EditClipboard::getWriteFlags() const {
	return writeFlags;
}

void EditClipboard::setWriteFlags(const QIODevice::OpenMode& value) {
	writeFlags = value;
}

QIODevice::OpenMode EditClipboard::getReadFlags() const {
	return readFlags;
}

void EditClipboard::setReadFlags(const QIODevice::OpenMode& value) {
	readFlags = value;
}
