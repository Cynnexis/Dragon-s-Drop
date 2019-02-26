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
	
	QMimeData* mime = nullptr;
	
	// See https://doc.qt.io/qt-5/qmetatype.html#Type-enum
	switch (data.type()) {
		case QMetaType::UnknownType:
		case QMetaType::Void:
		case QMetaType::Nullptr:
		case QMetaType::VoidStar:
		case QMetaType::QObjectStar:
		case QMetaType::QVariant:
		default:
			return nullptr;
		case QMetaType::Bool:
			mime->setText(data.toBool() ? "true" : "false");
			break;
		case QMetaType::Short:
		case QMetaType::UShort:
		case QMetaType::Int:
			mime->setText(QString::number(data.toInt()));
			break;
		case QMetaType::UInt:
			mime->setText(QString::number(data.toUInt()));
			break;
		case QMetaType::Double:
			mime->setText(QString::number(data.toDouble()));
			break;
		case QMetaType::Float:
			mime->setText(QString::number(data.toFloat()));
			break;
		case QMetaType::ULong:
		case QMetaType::Long:
		case QMetaType::ULongLong:
		case QMetaType::LongLong:
			mime->setText(QString::number(data.toLongLong()));
			break;
		case QMetaType::QChar:
		case QMetaType::Char:
		case QMetaType::SChar:
		case QMetaType::UChar:
			mime->setText(data.toChar());
			break;
		case QMetaType::QString:
			mime->setText(data.toString());
			break;
		case QMetaType::QByteArray:
			mime->setText(data.toByteArray());
			break;
		case QMetaType::QDate:
			mime->setText(data.toDate().toString(Qt::SystemLocaleDate));
			break;
		case QMetaType::QSize:
			mime->setText(QString::number(data.toSize().width()) + "x" + QString::number(data.toSize().height()));
			break;
		case QMetaType::QSizeF:
			mime->setText(QString::number(data.toSizeF().width()) + "x" + QString::number(data.toSizeF().height()));
			break;
		case QMetaType::QTime:
			mime->setText(data.toTime().toString(Qt::SystemLocaleDate));
			break;
		case QMetaType::QColor:
			mime->setText(data.value<QColor>().name());
			break;
		case QMetaType::QStringList:
			mime->setText(data.toStringList().join(";"));
			break;
		case QMetaType::QIcon:
			mime->setImageData(data.value<QIcon>());
			break;
		case QMetaType::QUrl:
			mime->setText(data.toUrl().toString());
			break;
		case QMetaType::QRegExp:
			mime->setText(data.toRegExp().pattern());
			break;
		case QMetaType::QRegularExpression:
			mime->setText(data.toRegularExpression().pattern());
			break;
		case QMetaType::QDateTime:
			mime->setText(data.toDateTime().toString(Qt::SystemLocaleDate));
			break;
		case QMetaType::QFont:
			mime->setText(data.value<QFont>().toString());
			break;
		case QMetaType::QImage:
			mime->setImageData(data.value<QImage>());
			break;
		case QMetaType::QPixmap:
			mime->setImageData(data.value<QPixmap>());
			break;
		case QMetaType::QBitmap:
			mime->setImageData(data.value<QBitmap>());
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
