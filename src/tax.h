
/************************************************************************
 * tax.h								*
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

#ifndef TAX_H
#define TAX_H

//#include <QWidget>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QVBoxLayout>

//class Tax : public QWidget
class Tax : public QGroupBox
{
Q_OBJECT
public:
	Tax(double, double, double, QWidget*);
	virtual ~Tax();
private:
	QLabel *lSell, *lBuy, *lBroker;
	QDoubleSpinBox *sbSell, *sbBuy, *sbBroker;
	QVBoxLayout *layout;
private slots:
	void onSellChange(double);
	void onBuyChange(double);
	void onBrokerChange(double);
signals:
	void sellTaxChanged(double);
	void buyTaxChanged(double);
	void brokerTaxChanged(double);
};

#endif

