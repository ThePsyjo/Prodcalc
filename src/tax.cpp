
/************************************************************************
 * tax.cpp								*
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

#include "tax.h"

Tax::Tax(double bT, double sT, double brT, QWidget *parent)
{
	setParent(parent);

	lSell	= new QLabel (tr("sell tax"), this);
	lBuy	= new QLabel (tr("buy tax"), this);
	lBroker	= new QLabel (tr("broker fee"), this);

	sbSell	= new QDoubleSpinBox (this);
	sbBuy	= new QDoubleSpinBox (this);
	sbBroker= new QDoubleSpinBox (this);

	sbSell->setSuffix("%");
	sbSell->setValue(sT);
	sbBuy->setSuffix("%");
	sbBuy->setValue(bT);
	sbBroker->setSuffix("%");
	sbBroker->setValue(brT);;

	layout   = new QVBoxLayout(this);

	layout->addWidget(lBuy);
	layout->addWidget(sbBuy);

	layout->addWidget(lSell);
	layout->addWidget(sbSell);
	
	layout->addWidget(lBroker);
	layout->addWidget(sbBroker);

	adjustSize();

	connect(sbSell,	  SIGNAL(valueChanged(double)),	this, SLOT(onSellChange(double)));
	connect(sbBuy,	  SIGNAL(valueChanged(double)), this, SLOT(onBuyChange(double)));
	connect(sbBroker, SIGNAL(valueChanged(double)), this, SLOT(onBrokerChange(double)));
}

void Tax::onSellChange(double d)
{	emit sellTaxChanged(d);	}
void Tax::onBuyChange(double d)
{	emit buyTaxChanged(d);	}
void Tax::onBrokerChange(double d)
{	emit brokerTaxChanged(d);	}



Tax::~Tax()
{}


