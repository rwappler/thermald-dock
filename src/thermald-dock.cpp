/*
 * thermald-dock.cpp
 *
 *  Created on: 18.04.2014
 *      Author: mary
 */

#include <algorithm>
#include <QtGui>
#include <QDir>
#include <QMessageBox>
#include <QApplication>
#include <QStandardItemModel>
#include <QMenu>
#include "thermald-dock.hpp"

namespace thermald_dock {

ThermaldControl::ThermaldControl(QStandardItemModel& itemModel) : model(itemModel) {}
Window::Window() {
	this->trayIcon = new QSystemTrayIcon(this);
	this->trayIcon->setIcon(QIcon(":/resources/thermald-dock.png"));
	this->setWindowIcon(QIcon(":/resources/thermald-dock.png"));
	this->trayIcon->connect(trayIcon,
			SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
			SLOT(toggleVisible()));
	this->exitAction = new QAction(QObject::tr("Exit"), this);
}

QSystemTrayIcon& Window::trayicon() {
	return *this->trayIcon;
}

void Window::setActions(QStandardItemModel& model) {
	/* TODO: Register Actions from ThermaldControl.
	 * - Add Quit-Action
	 * - Consider STL-Vector for passing actions
	 * - Provide slots in the controller
	 */
}

void Window::toggleVisible() {
	setVisible(!isVisible());
}
void Window::show() {
	this->QDialog::show();
	trayIcon->setVisible(true);
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

	thermald_dock::Window window;
	window.setVisible(false);
	window.show();
	return thermald_dock.exec();
}
