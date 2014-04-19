/*
 * thermald-dock.hpp
 *
 * Copyright 2014, Robert Wappler
 *
 * This file is part of thermald-dock.
 *
 * thermald-dock is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * thermald-dock is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with thermald-dock.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Created on: 18.04.2014
 *     Author: Robert Wappler
 */

#include <QSystemTrayIcon>
#include <QWidget>
#include <QStandardItemModel>
#include <QAction>
#include <QList>
#include <QDBusConnection>

#ifndef THERMALD_DOCK_HPP_
#define THERMALD_DOCK_HPP_

namespace thermald_dock {
class Dock: public QWidget {
Q_OBJECT

public:
	Dock();
	void show();
	void setPreferences(QObject& receiver, QList<QAction*>& preferences);

private slots:
	void toggleVisible(QSystemTrayIcon::ActivationReason);

private:
	QAction* exitAction;
	QSystemTrayIcon* trayIcon;
	QMenu* menu;
	void toggleVisible();
};

class ThermaldControl: public QObject {
Q_OBJECT

public:
	ThermaldControl();
	QList<QAction*>& preferences();

private:
	QList<QAction*> prefs;
	void currentPreference(QString str);

private slots:
	void currentPreference(QAction*);
	const QString currentPreference();
	void maxTemperature(uint degC);
	const uint maxTemperature();

// signals:
	// void preferenceChanged(String pref);

};

}

#endif /* THERMALD_DOCK_HPP_ */
