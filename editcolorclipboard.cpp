#include "editcolorclipboard.h"

EditColorClipboard::EditColorClipboard(QWidget* parent) : EditClipboard(parent) {
}

EditColorClipboard::~EditColorClipboard() {
}

void EditColorClipboard::edit() {
	QColor* initialColor = nullptr;
	if (QColor::isValidColor(clip->getText()))
		initialColor = new QColor(clip->getText());
	else
		initialColor = new QColor(Qt::black);
	
	QColor newColor = QColorDialog::getColor(*initialColor, qobject_cast<QWidget*>(parent()), tr("Pick a new color"), QColorDialog::ShowAlphaChannel);
	
	if (newColor.isValid())
		clip->setText(newColor.name());
}

void EditColorClipboard::edit(QString) {
	return this->edit();
}

bool EditColorClipboard::write(QFile*) {
	return true;
}

bool EditColorClipboard::read(QFile*) {
	return true;
}
