#include "editimageclipboard.h"

EditImageClipboard::EditImageClipboard(QWidget* parent) : EditClipboard(parent) {
	setWriteFlags(QIODevice::WriteOnly);
	setReadFlags(QIODevice::ReadOnly);
}

EditImageClipboard::~EditImageClipboard() {
}

bool EditImageClipboard::write(QFile* f) {
	if (clip->getImage().isNull()) {
		QMessageBox::warning(qobject_cast<QWidget*>(parent()), tr("Error"), tr("No image in the clipboard"));
		return false;
	}
	else {
#ifdef QT_DEBUG
		cout << "EditImageClipboard::write> Saving image to file: " << clip->getImage().size().width() << "x" << clip->getImage().size().height() << endl;
#endif
		QImageWriter writer(f, "PNG");
		
		if (!writer.canWrite()) {
			QMessageBox::critical(qobject_cast<QWidget*>(parent()), "Error", "Cannot write the temporary image file. You should execute this application with higher permissions.");
			f->close();
			return false;
		}
		
		writer.write(clip->getImage());
		
		return true;
	}
}

bool EditImageClipboard::read(QFile* f) {
	// Reset position carret
	QTextStream in(f);
	in.seek(0);
	
	// Load the image
	QImage image(QFileInfo(*f).absoluteFilePath());
	
	if (image.isNull())
		QMessageBox::warning(qobject_cast<QWidget*>(parent()), tr("Error"), tr("Cannot load back the image. Did you delete the file?"));
	else {
#ifdef QT_DEBUG
		cout << "EditImageClipboard::read> Image: " << image.size().width() << "x" << image.size().height() << endl;
#endif
		clip->setImage(image);
	}
	
	return true;
}
