#ifndef EDITTEXTCLIPBOARD_H
#define EDITTEXTCLIPBOARD_H

#include "editclipboard.h"

class EditTextClipboard : public EditClipboard
{
	Q_OBJECT
public:
	EditTextClipboard(QWidget* parent);
	~EditTextClipboard();
	
protected:
	virtual bool write(QFile* f);
	virtual bool read(QFile* f);
};

#endif // EDITTEXTCLIPBOARD_H
