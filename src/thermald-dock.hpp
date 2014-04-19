/*
 * thermald-dock.hpp
 *
 *  Created on: 18.04.2014
 *      Author: mary
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
