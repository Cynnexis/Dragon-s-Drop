#ifndef R_H
#define R_H

#include <iostream>
#include <QImage>
#include <QFile>
#include <QPixmap>
#include <QDomElement>
#include <QSvgRenderer>
#include <QPainter>
#include <QIcon>
#include <QTextStream>
#include <QDir>

using namespace std;

namespace R {
	
	QString getTemporaryFileTemplate(QString suffix = "XXXXXX");
	QString getTemporaryDirTemplate();
	
	QPixmap getDragonsDropIcon();
	QPixmap getDragonsDropIcon(int size);
	
	QPixmap getSettings(QColor tint);
	QPixmap getSettings();
	
	QPixmap getPower(QColor tint);
	QPixmap getPower();
	
	QColor getPrimaryColor();
	
	QPixmap getSvg(const QString& path);
	QPixmap getSvg(const QString& path, const QColor& tint);
	
	QString getCodeFromPath(const QString path);
	void setAttrRec(QDomElement& elem, const QString& tagName, const QString& attr, const QString& value);
};

#endif // R_H
