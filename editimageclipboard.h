#ifndef EDITIMAGECLIPBOARD_H
#define EDITIMAGECLIPBOARD_H

#include <QImageWriter>

#include "editclipboard.h"

class EditImageClipboard : public EditClipboard
{
	Q_OBJECT
public:
	EditImageClipboard(QWidget* parent);
	~EditImageClipboard();
	
protected:
	virtual bool write(QFile* f);
	virtual bool read(QFile* f);
};

#endif // EDITIMAGECLIPBOARD_H
