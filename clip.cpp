#include "clip.h"

Clip* Clip::clip = nullptr;
QRegularExpression Clip::urlPattern = QRegularExpression("^([a-z0-9+.-]+):(?://(?:((?:[a-z0-9-._~!$&'()*+,;=:]|%[0-9A-F]{2})*)@)?((?:[a-z0-9-._~!$&'()*+,;=]|%[0-9A-F]{2})*)(?::(\\d*))?(/(?:[a-z0-9-._~!$&'()*+,;=:@/]|%[0-9A-F]{2})*)?|(/?(?:[a-z0-9-._~!$&'()*+,;=:@]|%[0-9A-F]{2})+(?:[a-z0-9-._~!$&'()*+,;=:@/]|%[0-9A-F]{2})*)?)(?:\\?((?:[a-z0-9-._~!$&'()*+,;=:/?@]|%[0-9A-F]{2})*))?(?:#((?:[a-z0-9-._~!$&'()*+,;=:/?@]|%[0-9A-F]{2})*))?$", QRegularExpression::CaseInsensitiveOption);

Clip::Clip(QObject* parent) : QObject(parent) {
	connect(clipboard, SIGNAL(changed(QClipboard::Mode)), this, SLOT(onModeChanged(QClipboard::Mode)));
	connect(clipboard, SIGNAL(dataChanged()), this, SLOT(onDataChanged()));
}

Clip* Clip::getInstance(QObject* parent) {
	if (Clip::clip == nullptr)
		Clip::clip = new Clip(parent);
	
	return Clip::clip;
}

bool Clip::isUrlValid(QString url) {
	return urlPattern.match(url).hasMatch();
}

void Clip::reprocessClipboardContent() {
	onDataChanged();
}

QVariant Clip::mimeDataToVariant(const QMimeData& data) {
	QVariant variant;
	
	if (data.hasHtml())
		variant = data.html();
	else if (data.hasColor())
		variant = data.colorData().value<QColor>();
	else if (QColor::isValidColor(data.text()))
		variant = QColor(data.text());
	else if (data.hasUrls()) {
		QString urls = "";
		for (int i = 0, max = data.urls().count() ; i < max ; i++) {
			urls += data.urls().at(i).toString();
			if (i < max-1)
				urls += ";";
		}
		variant = urls;
	}
	else if (data.hasText() || isUrlValid(data.text()))
		variant = data.text();
	else if (data.hasImage())
		variant = data.imageData().value<QImage>();
}

QMimeData*Clip::variantToMimeData(const QVariant& data) {
	if (!data.isValid())
		return nullptr;
	
	QMimeData* mime = new QMimeData();
	
	// See https://doc.qt.io/qt-5/qmetatype.html#Type-enum
	switch (data.type()) {
		default:
			return nullptr;
		case QVariant::Bool:
			mime->setText(data.toBool() ? "true" : "false");
			break;
		case QVariant::Int:
			mime->setText(QString::number(data.toInt()));
			break;
		case QVariant::UInt:
			mime->setText(QString::number(data.toUInt()));
			break;
		case QVariant::Double:
			mime->setText(QString::number(data.toDouble()));
			break;
		case QVariant::ULongLong:
		case QVariant::LongLong:
			mime->setText(QString::number(data.toLongLong()));
			break;
		case QVariant::Char:
			mime->setText(data.toChar());
			break;
		case QVariant::String:
#ifdef QT_DEBUG
			cout << "variantToMimeData> Variant to string: " << data.toString().toStdString() << endl;
#endif
			mime->setText(data.toString());
			break;
		case QVariant::ByteArray:
			mime->setText(data.toByteArray());
			break;
		case QVariant::Date:
			mime->setText(data.toDate().toString(Qt::SystemLocaleDate));
			break;
		case QVariant::Size:
			mime->setText(QString::number(data.toSize().width()) + "x" + QString::number(data.toSize().height()));
			break;
		case QVariant::SizeF:
			mime->setText(QString::number(data.toSizeF().width()) + "x" + QString::number(data.toSizeF().height()));
			break;
		case QVariant::Time:
			mime->setText(data.toTime().toString(Qt::SystemLocaleDate));
			break;
		case QVariant::Color:
			mime->setText(data.value<QColor>().name());
			break;
		case QVariant::StringList:
			mime->setText(data.toStringList().join(";"));
			break;
		case QVariant::Icon:
			mime->setImageData(data.value<QIcon>());
			break;
		case QVariant::Url:
			mime->setText(data.toUrl().toString());
			break;
		case QVariant::RegExp:
			mime->setText(data.toRegExp().pattern());
			break;
		case QVariant::RegularExpression:
			mime->setText(data.toRegularExpression().pattern());
			break;
		case QVariant::DateTime:
			mime->setText(data.toDateTime().toString(Qt::SystemLocaleDate));
			break;
		case QVariant::Font:
			mime->setText(data.value<QFont>().toString());
			break;
		case QVariant::Image:
			mime->setImageData(data.value<QImage>());
			break;
		case QVariant::Pixmap:
			mime->setImageData(data.value<QPixmap>());
			break;
		case QVariant::Bitmap:
			mime->setImageData(data.value<QBitmap>());
			break;
		case QVariant::Uuid:
			mime->setImageData(data.toUuid().toString());
			break;
	}
	
	return mime;
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

QMap<qint64, QVariant>*Clip::getHistory() {
	return &history;
}

void Clip::setHistory(const QMap<qint64, QVariant>& value) {
	history = value;
}

/* PRIVATE SLOTS */

void Clip::onModeChanged(QClipboard::Mode mode) {
	emit modeChanged(mode);
}

void Clip::onDataChanged() {
	qint64 index = QDateTime::currentDateTime().toMSecsSinceEpoch();
	
	const QMimeData* data = clipboard->mimeData();
	emit dataChanged(data);
	
	if (data->hasHtml()) {
		history.insert(index, data->html());
		emit htmlReceived(data->html());
#ifdef QT_DEBUG
		cout << "Clip> Received html = \"" + data->html().toStdString() + "\"" << endl;
#endif
	}
	else if (data->hasColor()) {
		QColor color = data->colorData().value<QColor>();
		emit colorReceived(color);
		history.insert(index, color);
#ifdef QT_DEBUG
		cout << "Clip> Received color (1) = \"" + color.name().toStdString() + "\"" << endl;
#endif
	}
	else if (QColor::isValidColor(data->text())) {
		QColor color = QColor(data->text());
		emit colorReceived(color);
		history.insert(index, color);
#ifdef QT_DEBUG
		cout << "Clip> Received color (2) = \"" + color.name().toStdString() + "\"" << endl;
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
		cout << "Clip> Received urls (1) = \"" + urls.toStdString() + "\"" << endl;
#endif
	}
	else if (isUrlValid(data->text())) {
		history.insert(index, data->text());
		emit urlsReceived({data->text()});
#ifdef QT_DEBUG
		cout << "Clip> Received urls (2) = \"" + data->text().toStdString() + "\"" << endl;
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
		QImage image = data->imageData().value<QImage>();
		history.insert(index, image);
		emit imageReceived(image);
#ifdef QT_DEBUG
		cout << "Clip> Received image = (" << image.size().width() << "x" << image.size().height() << ")" << endl;
#endif
	}
}
