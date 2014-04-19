/*
 * thermald-dock.cpp
 *
 * Copyright 2014, Robert Wappler
 *
 * This file is part of thermald-dock.
 *
 * Foobar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Created on: 18.04.2014
 *     Author: Robert Wappler
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
	connect(menu, SIGNAL(aboutToShow()), &receiver, SLOT(currentPreference()));

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

	currentPreference();
}

const QString ThermaldControl::currentPreference() {
	QDBusMessage reply = QDBusConnection::systemBus().call(
			QDBusMessage::createMethodCall("org.freedesktop.thermald",
					"/org/freedesktop/thermald", "org.freedesktop.thermald",
					"GetCurrentPreference"));
	if (reply.type() == QDBusMessage::ErrorMessage)
		qFatal("Could not determine current thermald profile");

	QList<QVariant> currentProfiles = reply.arguments();
	if (currentProfiles.length() < 1)
		qWarning("Got empty respnse, when asking for current thermald profile");
	else {
		QString prefStr = currentProfiles[0].value<QString>();
		for (QList<QAction*>::iterator action = prefs.begin();
				action != prefs.end(); action++) {
			(*action)->setChecked(
					(*action)->data().value<QString>() == prefStr);
		}
	}
	return QString();
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

