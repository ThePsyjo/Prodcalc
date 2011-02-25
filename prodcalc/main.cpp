
/************************************************************************
 * main.cpp								*
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

#include <QApplication>
#include <QTranslator>
#include <QMessageBox>
#include "window.h"

#include <QCleanlooksStyle>
//
int main(int argc, char ** argv)
{
	QApplication app( argc, argv );

	QTranslator translator;

	if(! translator.load(":/" + QLocale::languageToString(QLocale::system().language())))
		if(! translator.load(":/English.qm"))
		{
			QMessageBox::warning(NULL, "Error", "Error while loading translation.");
			return 1;
		}

	app.installTranslator(&translator);

	app.setWindowIcon(QIcon(QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + "/res/isk1.png")));

	MainWindow win;
	win.show(); 

	app.setStyle(new QCleanlooksStyle); // comment this if it won't compile
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
