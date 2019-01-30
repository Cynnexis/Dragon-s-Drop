#include "p.h"

P* P::p = nullptr;

P::P(QObject *parent) : QObject(parent) {
	if (isFirstLaunch()) {
		setFirstLaunch(false);
	}
}

P* P::getInstance(QObject* parent) {
	if (P::p == nullptr)
		P::p = new P(parent);
	
	return P::p;
}

bool P::isFirstLaunch() {
	return settings.value("app/firstLaunch", defaultFirstLaunch()).toBool();
}

void P::setFirstLaunch(const bool& firstLaunch) {
	settings.setValue("app/firstLaunch", firstLaunch);
	emit firstLaunchChanged(firstLaunch);
}

bool P::defaultFirstLaunch() {
	return true;
}

void P::initFirstLaunch() {
	setFirstLaunch(defaultFirstLaunch());
}
