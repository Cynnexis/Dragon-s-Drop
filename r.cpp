#include "r.h"

QString R::getTemporaryFileTemplate(QString suffix) {
	QString dir = QDir::tempPath();
	
	if (!dir.endsWith("/"))
		dir += "/";
	
	if (!suffix.startsWith("."))
		suffix = "." + suffix;
	
	QString temp = dir + "dragonsdropXXXXXX" + suffix;
	return temp;
}

QString R::getTemporaryDirTemplate() {
	QString dir = QDir::tempPath();
	
	if (!dir.endsWith("/"))
		dir += "/";
	
	QString temp = dir + "dragonsdropXXXXXX";
	return temp;
}

QString R::getREADMEHtml() {
	QFile f(":/txt/README_html");
	
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
		return "";
	
	QString content = "";
	QTextStream in(&f);
	
	while (!in.atEnd())
		content += in.readLine() + "\n";
	
	f.close();
	
	return content;
}

QPixmap R::getDragonsDropIcon() {
	return getSvg(":/img/svg/dragonsdrop");
}

QPixmap R::getDragonsDropIcon(int size) {
	return getSvg(":/img/svg/dragonsdrop" + QString::number(size));
}

QPixmap R::getSettings(QColor tint) {
	return getSvg(":/img/svg/settings", tint);
}

QPixmap R::getSettings() {
	return getSettings(getPrimaryColor());
}

QPixmap R::getPower(QColor tint) {
	return getSvg(":/img/svg/power", tint);
}

QPixmap R::getPower() {
	return getPower(getPrimaryColor());
}

QColor R::getPrimaryColor() {
	return QColor("#c22f32");
}

QPixmap R::getSvg(const QString& path) {
	return QPixmap(path);
}

QPixmap R::getSvg(const QString& path, const QColor& tint) {	
	// Source code inspired from https://stackoverflow.com/questions/15123544/change-the-color-of-an-svg-in-qt
	QFile f(path);
	
	if (!f.open(QIODevice::ReadOnly))
		throw "Cannot open " + path.toStdString();
	
	QByteArray data = f.readAll();
	
	QDomDocument doc;
	doc.setContent(data);
	QDomElement docElem = doc.documentElement();
	setAttrRec(docElem, "path", "fill", tint.name());
	
	QSvgRenderer renderer(doc.toByteArray());
	QPixmap pixmap(renderer.defaultSize());
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	renderer.render(&painter);
	return pixmap;
}

QString R::getCodeFromPath(const QString path) {
	QFile f(path);
	
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
		throw "Cannot open the credential files.\n";
	
	QString content = f.readAll().replace("\r\n", "").replace("\r", "").replace("\n", "");
	
	f.close();
	
	return content;
}

void R::setAttrRec(QDomElement& elem, const QString& tagName, const QString& attr, const QString& value) {
	if (elem.tagName().compare(tagName) == 0 &&
		(attr != "fill" || (attr == "fill" && elem.attribute(attr, "") != "none"))) {
		elem.setAttribute(attr, value);
	}
	
	for (int i = 0, imax = elem.childNodes().count(); i < imax; i++) {
		QDomNode child = elem.childNodes().at(i);
		
		if (!child.isElement())
			continue;
		
		QDomElement childElem = child.toElement();
		setAttrRec(childElem, tagName, attr, value);
	}
}
