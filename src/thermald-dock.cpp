/*
 * thermald-dock.cpp
 *
 *  Created on: 18.04.2014
 *      Author: mary
 */

#include <algorithm>
#include <vector>
#include <iostream>
#include <QDir>
#include <QMessageBox>
#include <QApplication>
#include <QMenu>
#include <QDBusConnection>
#include <QDBusMessage>

#include "thermald-dock.hpp"

namespace thermald_dock {

Dock::Dock() {
	this->trayIcon = new QSystemTrayIcon(this);
	this->trayIcon->setIcon(QIcon(":/resources/thermald-dock.png"));
	this->setWindowIcon(QIcon(":/resources/thermald-dock.png"));
	this->exitAction = new QAction(QObject::tr("Exit"), this);

	connect(this->exitAction, SIGNAL(triggered()), QCoreApplication::instance(),
			SLOT(quit()));

	this->menu = new QMenu();
	this->menu->addAction(exitAction);
	this->trayIcon->setContextMenu(menu);
}

void Dock::setPreferences(QObject& receiver, QList<QAction*>& preferences) {
	QActionGroup* actions = new QActionGroup(this->menu);
	for (QList<QAction*>::iterator action = preferences.begin();
			action != preferences.end(); action++) {
		actions->addAction(*action);
		this->menu->insertAction(this->exitAction, *action);
	}

	this->menu->insertSeparator(this->exitAction);

	connect(menu, SIGNAL(triggered(QAction*)), &receiver,
			SLOT(currentPreference(QAction*)));

}

void Dock::toggleVisible() {
	setVisible(!isVisible());
}

void Dock::toggleVisible(QSystemTrayIcon::ActivationReason reason) {
	switch (reason) {
	case QSystemTrayIcon::Trigger:
		toggleVisible();
		break;
	default:
		break; // no-op
	}
}

void Dock::show() {
	// this->QWidget::show();
	trayIcon->setVisible(true);
}

ThermaldControl::ThermaldControl() {

	QAction* energyConservative = new QAction(
			QObject::tr("Energy Conservative"), this);
	QAction* performance = new QAction(QObject::tr("Performance"), this);
	QAction* fallback = new QAction(QObject::tr("Fallback"), this);
	QAction* disable = new QAction(QObject::tr("Disable"), this);

	energyConservative->setCheckable(true);
	energyConservative->setData("ENERGY_CONSERVE");
	performance->setCheckable(true);
	performance->setData("PERFORMANCE");
	fallback->setCheckable(true);
	fallback->setData("FALLBACK");
	disable->setCheckable(true);
	disable->setData("DISABLE");
	this->prefs << energyConservative << performance << fallback << disable;
}

const QString ThermaldControl::currentPreference() {

	return "";
}

void ThermaldControl::maxTemperature(uint degC) {
}

QList<QAction*>& ThermaldControl::preferences() {
	return this->prefs;
}

const uint ThermaldControl::maxTemperature() {
	return 0;
}

void ThermaldControl::currentPreference(QString profile) {

	QDBusConnection sysBus(QDBusConnection::systemBus());

	if (!sysBus.isConnected())
		qFatal("Could not connect to system bus.");

	QDBusMessage msg(
			QDBusMessage::createMethodCall("org.freedesktop.thermald",
					"/org/freedesktop/thermald", "org.freedesktop.thermald",
					"SetCurrentPreference"));
	msg << profile;

	QDBusMessage reply = sysBus.call(msg);
	if (reply.type() == QDBusMessage::ErrorMessage)
		std::cerr << reply.errorMessage().toStdString();
}

void ThermaldControl::currentPreference(QAction* a) {
	currentPreference(a->data().value<QString>());
}

}

int main(int argc, char** argv) {

	Q_INIT_RESOURCE(thermald_dock);
	QApplication thermald_dock(argc, argv);

	if (!QSystemTrayIcon::isSystemTrayAvailable()) {
		QMessageBox::critical(0, QObject::tr("Systray"),
				QObject::tr("System Tray not available on this system"));
		return 1;
	}

	QApplication::setQuitOnLastWindowClosed(false);

	thermald_dock::Dock dock;

	thermald_dock::ThermaldControl ctrl;

	dock.show();
	dock.setPreferences(ctrl, ctrl.preferences());
	return thermald_dock.exec();
}

