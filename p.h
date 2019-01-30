#ifndef P_H
#define P_H

#include <iostream>
#include <QObject>
#include <QSettings>

using namespace std;

class P : public QObject
{
	Q_OBJECT
private:
	P(QObject* parent = nullptr);
	
public:
	static P* getInstance(QObject *parent = nullptr);
	
	bool isFirstLaunch();
	void setFirstLaunch(const bool& firstLaunch);
	bool defaultFirstLaunch();
	void initFirstLaunch();
	
signals:
	void firstLaunchChanged(bool);
	
private:
	QSettings settings;
	static P* p;
};

#endif // P_H
