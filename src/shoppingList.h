
/************************************************************************
 * shoppingList.h							*
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

#ifndef SHOPPING_LIST_H
#define SHOPPING_LIST_H

#include <QPushButton> 
#include <QLabel> 
#include <QVector> 

#include "minerals.h" 

class ShopList : public Minerals
{
Q_OBJECT
public:
	ShopList(QVector<int>*, int, QVector<double>*, QWidget* = 0);
	virtual ~ShopList();
private:
	QPushButton *okButton;
	QVector<QLabel*> *lCntV, *lCostV;
	QLabel **lCntData, **lCostData;
	QString toStr(double);
	QString toStr(int);

};

#endif
