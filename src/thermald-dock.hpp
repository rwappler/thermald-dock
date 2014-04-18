/*
 * thermald-dock.hpp
 *
 *  Created on: 18.04.2014
 *      Author: mary
 */

#include <QSystemTrayIcon>
#include <QDialog>

#ifndef THERMALD_DOCK_HPP_
#define THERMALD_DOCK_HPP_

class Window: QDialog {
Q_OBJECT

public:
	Window();

private:
	QSystemTrayIcon* trayIcon;
};

#endif /* THERMALD_DOCK_HPP_ */
