#ifndef EDITCLIPBOARD_H
#define EDITCLIPBOARD_H

#include <iostream>
#include <QObject>
#include <QIODevice>
#include <QFile>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QMessageBox>
#include <QDateTime>
#include <QDesktopServices>
#include <QUrl>

#include "clip.h"
#include "r.h"

using namespace std;

/**
 * Abstract class to edit the clipboard content. To use it, you need to create
 * a child class and implement the functions `write()` and `read()`, accordingly to
 * the non-virtual method `edit()`.
 * @author Valentin Berger
 */
class EditClipboard : public QObject
{
	Q_OBJECT
public:
	EditClipboard(QWidget* parent);
	virtual ~EditClipboard();
	
	/**
	 * Edit the clipboard content by creating a temporary file containing the clipboard data,
	 * open the default application associated with the given file extension, and then load
	 * again the file and apply the data to the clipboard.
	 * @param fileExtension The file extension that can open the corresponding application.
	 */
	virtual void edit(QString fileExtension);
	
protected:
	/* PURE VIRTUAL METHODS */
	
	/**
	 * Pure virtual method that writes the clipboard content in the given `QFile`. This
	 * file object is already instanciated and open. Please do not close the file inside
	 * this function. Returns `true` by default, `false` if there is an exception and
	 * the method `editClipboard()` must stop right after the execution of `write()`.
	 * @param f The temporary file where to write the data.
	 * @return Return `true` to continue the execution of the function `edit()`,
	 * otherwise `false` to stop it (in this case, the file `f` is not even closed,
	 * you can close it in this function if necessary).
	 */
	virtual bool write(QFile* f) = 0;
	
	/**
	 * Pure virtual method that reads the given `QFile` content to the clipboard.
	 * This file object is already instanciated and open. Please do not close the
	 * file inside this function. Returns `true` by default, `false` if there is
	 * an exception and the method `editClipboard()` must stop right after the
	 * execution of `write()`.
	 * @return Return `true` to continue the execution of the function `edit()`,
	 * otherwise `false` to stop it (in this case, the file `f` is not even closed,
	 * you can close it in this function if necessary).
	 */
	virtual bool read(QFile* f) = 0;
	
	/* GETTERS & SETTERS */
	
	QIODevice::OpenMode getWriteFlags() const;
	void setWriteFlags(const QIODevice::OpenMode& value);
	
	QIODevice::OpenMode getReadFlags() const;
	void setReadFlags(const QIODevice::OpenMode& value);
	
private:
	QIODevice::OpenMode writeFlags;
	QIODevice::OpenMode readFlags;
	
protected:
	Clip* clip;
};

#endif // EDITCLIPBOARD_H
