
/************************************************************************
 * shoppingList.cpp							*
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

#include "shoppingList.h"

ShopList::ShopList(QVector<int>* mCnt, int runs, QVector<double>* mCost, QWidget* parent)
: Minerals(parent)
{
	setParent(parent);
	setWindowFlags(Qt::Window);
	QString style = "* { background-color:rgb(245,232,153); font: strong } QPushButton { background-color: rgba(128,128,128,128); }";
	QString color = "* { color: rgb(30,30,255); }";
	setStyleSheet(color + style);
	for ( int i = 0; i < 8; i++ )
		lbData[i]->setStyleSheet(color);
	setWindowTitle(tr("shopping list"));
//	setModal(1);


	lCntV = new QVector<QLabel*>(8);
	lCostV = new QVector<QLabel*>(8);
	
	lCntData  = lCntV->data();
	lCostData = lCostV->data();

	for (int i = 0; i < 8; i++)
	{
		lCntData[i] = new QLabel(toStr(mCnt->at(i) * runs) + " x ", this);
		layout->addWidget( lCntData[i], i, 2, Qt::AlignCenter );
		lCostData[i] = new QLabel(tr("up to ") + toStr(mCost->at(i)) + " ISK/" + tr("unit"), this);
		layout->addWidget( lCostData[i], i, 3, Qt::AlignCenter );
	}

	okButton = new QPushButton ("OK", this);
	okButton->setFocus();
	connect(okButton, SIGNAL(clicked()), this, SLOT(close()));


	layout->addWidget(okButton, 9, 0, 1, 4);

	setLayout(layout);
}

QString ShopList::toStr(double d)
{ return QString("%L1").arg(d, 3, 'F', 2); }

QString ShopList::toStr(int i)
{ return QString("%L1").arg(i, 3); }

ShopList::~ShopList()
{};
