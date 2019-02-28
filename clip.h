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
#include <QIcon>
#include <QPixmap>
#include <QBitmap>
#include <QVariant>
#include <QMetaType>
#include <QFont>
#include <QRegularExpression>
#include <QUuid>

using namespace std;

/**
 * This is the clipboard manager for the application. It is extended from
 * the class [`QClipboard`](https://doc.qt.io/qt-5/qclipboard.html). To use it, you must first
 * get the unique instance of the class by calling `getInstance()`:
 * 
 * @code
 * Clip* myClip = Clip::getInstance();
 * @endcode
 * 
 * It implements all methods from [`QClipboard`](https://doc.qt.io/qt-5/qclipboard.html),
 * except that the getters begin with the prefix `get` (e.g. `clipboard->text()` becomes
 * `clip->getText()`).
 * 
 * One of the most interesting feature in this class compared to
 * [`QClipboard`](https://doc.qt.io/qt-5/qclipboard.html) is the signals:
 * Instead of having on signal with a [`QMimeData`](https://doc.qt.io/qt-5/qmimedata.html),
 * the class emits a signal depending on the MIME Type (e.g. A text-flavored content will
 * emit the signal `Clip::textReceived(QString)`).
 * 
 * Finally, it also save the history of the clipboard.
 * @brief Clipboard manager. It implements the Singleton pattern design.
 * @author Valentin Berger
 */
class Clip : public QObject
{
	Q_OBJECT
private:
	Clip(QObject* parent = nullptr);
	
public:
	/**
	 * Get the unique instance of the class
	 * @param parent The parent of the class. It is not mandatory to provide it.
	 * @return Return the unique instance of the class. It is advised to store
	 * it in a pointer if you will use it frequently.
	 */
	static Clip* getInstance(QObject* parent = nullptr);
	
	/* METHOD */
	
	/**
	 * Check if the given string is a valid URL using regular expression.
	 * @param url The url to test.
	 * @return Return `true` is the given string is a valid url, `false` otherwise.
	 */
	static bool isUrlValid(QString url);
	
	/**
	 * Reprocess the actual clipboard content without changing it. As a result, the
	 * instance will call all corresponding signals according to the clipboard MIME Data.
	 * It is usefull when the clipboard have been changed before that the instance has
	 * been connected to slots, or at startup of the application to get the actual
	 * clipboard content.
	 */
	void reprocessClipboardContent();
	
	/**
	 * Convert a `QMimeData` instance into a `QVariant` if it is possible.
	 * @param data The mime data to convert into a variant.
	 * @return Return a variant if it is possible, otherwise return an empty variant.
	 */
	static QVariant mimeDataToVariant(const QMimeData& data);
	
	/**
	 * Convert a `QVariant` instance into a `QMimeData` if it is possible.
	 * @param data The variant data to convert into a mime data
	 * @return Return the pointer to a mime data if it is possible, otherwise return a null pointer.
	 */
	static QMimeData* variantToMimeData(const QVariant& data);
	
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
	
	QMap<qint64, QVariant>* getHistory();
	void setHistory(const QMap<qint64, QVariant>& value);
	
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
	QMap<qint64, QVariant> history;
	
	static QRegularExpression urlPattern;
};

#endif // CLIP_H
