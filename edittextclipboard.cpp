#include "edittextclipboard.h"

EditTextClipboard::EditTextClipboard(QWidget* parent) : EditClipboard(parent) {
	setWriteFlags(QIODevice::WriteOnly | QIODevice::Text);
	setReadFlags(QIODevice::ReadOnly | QIODevice::Text);
}

EditTextClipboard::~EditTextClipboard() {
}

bool EditTextClipboard::write(QFile* f) {
	QTextStream out(f);
	out << clip->getText() << endl;
	out.flush();
	return true;
}

bool EditTextClipboard::read(QFile* f) {
	QString result = "";
	QTextStream in(f);
	in.seek(0);
	while (!in.atEnd())
		result += in.readLine() + "\n";
	
	// Remove last "\n"
	if (result.endsWith("\n"))
		result.remove(QRegularExpression("\\n$"));
#ifdef QT_DEBUG
	cout << "Result: " << result.toStdString() << endl;
#endif
	clip->setText(result);
	return true;
}
