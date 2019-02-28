#ifndef EDITCOLORCLIPBOARD_H
#define EDITCOLORCLIPBOARD_H

#include <QColorDialog>

#include "editclipboard.h"

class EditColorClipboard : public EditClipboard
{
	Q_OBJECT
public:
	EditColorClipboard(QWidget* parent);
	~EditColorClipboard();
	
	virtual void edit();
	virtual void edit(QString);
	
protected:
	virtual bool write(QFile*);
	virtual bool read(QFile*);
};

#endif // EDITCOLORCLIPBOARD_H
