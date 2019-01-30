#include "clip.h"

Clip* Clip::clip = nullptr;

Clip::Clip(QObject* parent) : QObject(parent) {
	connect(clipboard, SIGNAL(changed(QClipboard::Mode)), this, SLOT(onModeChanged(QClipboard::Mode)));
	connect(clipboard, SIGNAL(dataChanged()), this, SLOT(onDataChanged()));
}

Clip* Clip::getInstance(QObject* parent) {
	if (Clip::clip == nullptr)
		Clip::clip = new Clip(parent);
	
	return Clip::clip;
}

/* CLIPBOARD METHODS */

void Clip::clear() {
	clipboard->clear();
}

bool Clip::ownsClipboardData() const {
	return clipboard->ownsClipboard();
}

const QMimeData*Clip::getMimeData() const {
	return clipboard->mimeData();
}

QString Clip::getText() const {
	return clipboard->text();
}

QString Clip::getText(QString& subtype) const {
	return clipboard->text(subtype);
}

QImage Clip::getImage() const {
	return clipboard->image();
}

QPixmap Clip::getPixmap() const {
	return clipboard->pixmap();
}

void Clip::setMimeType(QMimeData* src) {
	clipboard->setMimeData(src);
}

void Clip::setText(const QString& text) {
	clipboard->setText(text);
}

void Clip::setImage(const QImage& image) {
	clipboard->setImage(image);
}

void Clip::setPixmap(const QPixmap& pixmap) {
	clipboard->setPixmap(pixmap);
}

/* GETTER & SETTER */

QMap<uint, QVariant>* Clip::getHistory() {
	return &history;
}

void Clip::setHistory(const QMap<uint, QVariant>& value) {
	history = value;
}

/* PRIVATE SLOTS */

void Clip::onModeChanged(QClipboard::Mode mode) {
	emit modeChanged(mode);
}

void Clip::onDataChanged() {
	uint index = QDateTime().toTime_t();
	
	const QMimeData* data = clipboard->mimeData();
	emit dataChanged(data);
	
	if (data->hasHtml()) {
		history.insert(index, data->html());
		emit htmlReceived(data->html());
#ifdef QT_DEBUG
		cout << "Clip> Received html = \"" + data->html().toStdString() + "\"" << endl;
#endif
	}
	else if (data->hasUrls()) {
		QString urls = "";
		for (int i = 0, max = data->urls().count() ; i < max ; i++) {
			urls += data->urls().at(i).toString();
			if (i < max-1)
				urls += ";";
		}
		history.insert(index, urls);
		emit urlsReceived(data->urls());
#ifdef QT_DEBUG
		cout << "Clip> Received urls = \"" + urls.toStdString() + "\"" << endl;
#endif
	}
	else if (data->hasColor()) {
		QColor color = data->colorData().value<QColor>();
		emit colorReceived(color);
		history.insert(index, color);
#ifdef QT_DEBUG
		cout << "Clip> Received color = \"" + color.name().toStdString() + "\"" << endl;
#endif
	}
	else if (data->hasText()) {
		history.insert(index, data->text());
		emit textReceived(data->text());
#ifdef QT_DEBUG
		cout << "Clip> Received text = \"" + data->text().toStdString() + "\"" << endl;
#endif
	}
	else if (data->hasImage()) {
		QImage image = data->colorData().value<QImage>();
		history.insert(index, image);
		emit imageReceived(image);
#ifdef QT_DEBUG
		cout << "Clip> Received image = (" + QString::number(image.size().width()).toStdString() + "x" + QString::number(image.size().height()).toStdString() + ")" << endl;
#endif
	}
}
