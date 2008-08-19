
/************************************************************************
 * window.cpp								*
 * Copyright (C) 2008  Psyjo						*
 *									*
 * This program is free software; you can redistribute it and/or modify	*
 * it under the terms of the GNU General Public License as published by	*
 * the Free Software Foundation; either version 3 of the License,	*
 * or (at your option) any later version.				*
 *									*
 * This program is distributed in the hope that it will be useful, but	*
 * WITHOUT ANY WARRANTY; without even the implied warranty of		*
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.			*
 * See the GNU General Public License for more details.			*
 *									*
 * You should have received a copy of the GNU General Public License	*
 * along with this program; if not, see <http://www.gnu.org/licenses/>. *
 ************************************************************************/

#include "window.h"

MainWindow::MainWindow( QWidget * parent, Qt::WFlags f)
        : QMainWindow(parent, f)
{
	setWindowTitle(tr("appName"));
	calc = new CalcWidget (this);
	about = new QMenu(this);
	about = menuBar()->addMenu(tr("&about"));
	about->addAction("ProdCalc");
	about->addAction("Qt");
	connect(about, SIGNAL(triggered(QAction*)), this, SLOT(handleAboutAction(QAction*)));

	setCentralWidget(calc);
	adjustSize();
}

void MainWindow::handleAboutAction(QAction* a)
{
	if (a->text() == "ProdCalc") QMessageBox::about ( this, tr("about"), tr("appName") + tr("about_prodcalc", "dummy"));
	if (a->text() == "Qt") QMessageBox::aboutQt ( this, tr("about"));
}

MainWindow::~MainWindow()
{}

