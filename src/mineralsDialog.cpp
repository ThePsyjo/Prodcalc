
/************************************************************************
 * mineralsDialog.cpp							*
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

#include "mineralsDialog.h"

MineralsDialog::MineralsDialog()
{
	lb  = new QVector<QLabel*>(8);
	wid = new QVector<QWidget*>(8 );

	lbData  = lb->data();
	widData = wid->data();

	styleMap = new QMap<int, QString>;
	nameMap  = new QMap<int, QString>;

	layout = new QGridLayout(this);
	setLayout(layout);

	QString dir = QCoreApplication::applicationDirPath();

	styleMap->insert(0, "background-image: url(\"" + dir + "/res/tri.png\");");
	styleMap->insert(1, "background-image: url(\"" + dir + "/res/pye.png\");");
	styleMap->insert(2, "background-image: url(\"" + dir + "/res/mex.png\");");
	styleMap->insert(3, "background-image: url(\"" + dir + "/res/iso.png\");");
	styleMap->insert(4, "background-image: url(\"" + dir + "/res/noc.png\");");
	styleMap->insert(5, "background-image: url(\"" + dir + "/res/zyd.png\");");
	styleMap->insert(6, "background-image: url(\"" + dir + "/res/meg.png\");");
	styleMap->insert(7, "background-image: url(\"" + dir + "/res/mor.png\");");

	nameMap->insert(0, "Tritanium");
	nameMap->insert(1, "Pyerite");
	nameMap->insert(2, "Mexallon");
	nameMap->insert(3, "Isogen");
	nameMap->insert(4, "Nocxium");
	nameMap->insert(5, "Zydrine");
	nameMap->insert(6, "Megacyte");
	nameMap->insert(7, "Morphite");


	for (int i=0; i<8; i++)
	{
		widData[i]  = new QWidget(this);
		lbData[i]  = new QLabel (nameMap->take(i), this);
		widData[i]->setMinimumSize(30,30);
		widData[i]->setMaximumSize(30,30);
		widData[i]->setStyleSheet(styleMap->take(i));
		layout->addWidget( widData[i], i, 0);
		layout->addWidget( lbData[i], i, 1);
	}

	
	adjustSize();
}


MineralsDialog::~MineralsDialog()
{}


