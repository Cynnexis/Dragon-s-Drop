#ifndef CLIP_H
#define CLIP_H

#include <iostream>
#include <QObject>
#include <QGuiApplication>
#include <QClipboard>
#include <QMap>
#include <QDateTime>
#include <QMimeData>
#include <QUrl>
#include <QColor>
#include <QImage>
#include <QPixmap>
#include <QVariant>
#include <QRegularExpression>

using namespace std;

class Clip : public QObject
{
	Q_OBJECT
private:
	Clip(QObject* parent = nullptr);
	
public:
	static Clip* getInstance(QObject* parent = nullptr);
	
	/* METHOD */
	
	static bool isUrlValid(QString url);
	
	/* CLIPBOARD METHODS */
	
	void clear();
	bool ownsClipboardData() const;
	const QMimeData* getMimeData() const;
	QString getText() const;
	QString getText(QString& subtype) const;
	QImage getImage() const;
	QPixmap getPixmap() const;
	
	void setMimeType(QMimeData* src);
	void setText(const QString& text);
	void setImage(const QImage& image);
	void setPixmap(const QPixmap& pixmap);
	
	/* GETTER & SETTERS */
	
	QMap<uint, QVariant>* getHistory();
	void setHistory(const QMap<uint, QVariant>& value);
	
signals:
	void modeChanged(QClipboard::Mode);
	void dataChanged(const QMimeData*);
	
	void textReceived(QString);
	void htmlReceived(QString);
	void urlsReceived(QList<QUrl>);
	void colorReceived(QColor);
	void imageReceived(QImage);
	
private slots:
	
	/* PRIVATE SLOTS */
	
	void onModeChanged(QClipboard::Mode mode);
	void onDataChanged();
	
private:
	static Clip* clip;
	QClipboard* clipboard = QGuiApplication::clipboard();
	QMap<uint, QVariant> history;
	
	static QRegularExpression urlPattern;
};

#endif // CLIP_H
