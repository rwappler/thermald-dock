/*
 * thermald-dock.hpp
 *
 *  Created on: 18.04.2014
 *      Author: mary
 */

#include <QSystemTrayIcon>
#include <QDialog>
#include <QStandardItemModel>
#include <QAction>

#ifndef THERMALD_DOCK_HPP_
#define THERMALD_DOCK_HPP_

namespace thermald_dock {
class Window: public QDialog {
Q_OBJECT

public:
	Window();
	void show();
	QSystemTrayIcon& trayicon();
	void setActions(QStandardItemModel&);

private slots:
	void toggleVisible();

private:
	QAction* exitAction;
	QSystemTrayIcon* trayIcon;
};

class ThermaldControl {

public:
	ThermaldControl(QStandardItemModel&);

private:
	QStandardItemModel& model;
};
}

#endif /* THERMALD_DOCK_HPP_ */
