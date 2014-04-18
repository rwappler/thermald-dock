/*
 * thermald-dock.cpp
 *
 *  Created on: 18.04.2014
 *      Author: mary
 */

#include <QtGui>
#include <QDir>
#include <QMessageBox>
#include <QApplication>
#include "thermald-dock.hpp"


Window::Window() {
	this->trayIcon = new QSystemTrayIcon(this);
	this->trayIcon->setIcon(QIcon(":/resources/thermald-dock.png"));
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

	QDialog dummy;
	dummy.setVisible(false);
	return thermald_dock.exec();
}
