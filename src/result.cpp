
/************************************************************************
 * result.cpp								*
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

#include "result.h"
#define LABELS 32

Result::Result( QWidget * parent)
{
	setParent(parent);


	lb  = new QVector<QLabel*>(LABELS);

	lbMap  = new QMap<int, QString>;

	layout = new QGridLayout(this);
	setLayout(layout);

	lbMap->insert( 0, tr("minimum sell price"));
	lbMap->insert( 1, tr("unit"));
	lbMap->insert( 2, "---");
	lbMap->insert( 3, tr("run"));
	lbMap->insert( 4, "---");
	lbMap->insert( 5, tr("all"));
	lbMap->insert( 6, "---");
	lbMap->insert( 7, tr("target sell price"));
	lbMap->insert( 8, tr("unit"));
	lbMap->insert( 9, tr("run"));
	lbMap->insert(10, tr("all"));
	lbMap->insert(11, tr("cost/profit"));
	lbMap->insert(12, tr("buy"));
	lbMap->insert(13, "---");
	lbMap->insert(14, tr("buy tax"));
	lbMap->insert(15, "---");
	lbMap->insert(16, tr("production"));
	lbMap->insert(17, "---");
	lbMap->insert(18, tr("total cost"));
	lbMap->insert(19, "---");
	lbMap->insert(20, tr("sell"));
	lbMap->insert(21, "---");
	lbMap->insert(22, tr("sell tax"));
	lbMap->insert(23, "---");
	lbMap->insert(24, tr("broker fee"));
	lbMap->insert(25, "---");
	lbMap->insert(26, tr("profit"));
	lbMap->insert(27, "---");
	lbMap->insert(28, tr("total production time"));
	lbMap->insert(29, "---");
	lbMap->insert(30, tr("produced units"));
	lbMap->insert(31, "---");


	lbData = lb->data();
	for (int i=0; i<LABELS; i++)
	{
		lbData[i] =new QLabel ( lbMap->take(i), this );
	}
	
	layout->addWidget(lb->at(0), 0, 0, 1, 2, Qt::AlignCenter);

	for (int i=1; i<=5; i+=2)
	{
		layout->addWidget(lb->at(i)  , i, 0);
		layout->addWidget(lb->at(i+1), i, 1);
	}
	
	layout->addWidget(lb->at(7), 7, 0, 1, 2, Qt::AlignCenter);

	for (int i=8; i<=10; i++)
		layout->addWidget(lb->at(i)  , i, 0);

	sbUnitCost = new QDoubleSpinBox(this);
	sbRunCost = new QDoubleSpinBox(this);
	sbNRunCost = new QDoubleSpinBox(this);

	sbUnitCost->setMaximum(1000000000); 
        sbRunCost->setMaximum(1000000000); // 1 bio // more == integer overflow
	sbNRunCost->setMaximum(1000000000);
	sbUnitCost->setSuffix(" ISK"); 
        sbRunCost->setSuffix(" ISK");
	sbNRunCost->setSuffix(" ISK");

	layout->addWidget(sbUnitCost, 8, 1);
	layout->addWidget(sbRunCost , 9, 1);
	layout->addWidget(sbNRunCost, 10, 1);

	layout->addWidget(lb->at(11), 0, 2, 1, 2, Qt::AlignCenter);
	
	for (int i=12; i<=18; i+=2)
	{					//1, 2, 3, 4, 5, 6
		layout->addWidget(lb->at(i),   i/2-5, 2);
		layout->addWidget(lb->at(i+1), i/2-5, 3);
	}
	for (int i=20; i<=24; i+=2)
	{					//8, 9
		layout->addWidget(lb->at(i),   i/2-4, 2);
		layout->addWidget(lb->at(i+1), i/2-4, 3);
	}


	layout->addWidget(lb->at(26), 10, 2);
	layout->addWidget(lb->at(27), 10, 3);

	layout->addWidget(lb->at(28), 1, 4, 1, 2, Qt::AlignCenter);
	layout->addWidget(lb->at(29), 2, 4, 1, 2, Qt::AlignCenter);
	layout->addWidget(lb->at(30), 3, 4, 1, 2, Qt::AlignCenter);
	layout->addWidget(lb->at(31), 4, 4, 1, 2, Qt::AlignCenter);

	shopListButton = new QPushButton (tr("shopping list"), this);

	layout->addWidget(shopListButton, 6,4,1,2);

	layout->setHorizontalSpacing(20);

	adjustSize();


	connect(sbUnitCost, SIGNAL(editingFinished()), this, SLOT(onUnitCostChange()));
	connect(sbRunCost,  SIGNAL(editingFinished()), this, SLOT( onRunCostChange()));
	connect(sbNRunCost, SIGNAL(editingFinished()), this, SLOT(onNRunCostChange()));
	connect(shopListButton, SIGNAL(clicked()), this, SLOT(onShopListButtonClick()));
}

QString Result::toStr(int i)
{ return QString("%L1").arg(i, 3); }
QString Result::toStr(double d)
{ return QString("%L1").arg(d, 3, 'F', 3); }
QString Result::redStr(double d)
{ return "<font color=\"red\">" + toStr(d) + "</font>"; }
QString Result::greenStr(double d)
{ return "<font color=\"green\">" + toStr(d) + "</font>"; }
void Result::align(int i)
{ lbData[i]->setAlignment(Qt::AlignRight); }

void Result::onUnitCostChange()
{	emit targetUnitCostChanged(sbUnitCost->value());	}
void Result::onRunCostChange()
{	emit targetRunPriceChanged(sbRunCost->value());	}
void Result::onNRunCostChange()
{	emit targetNRunPriceChanged(sbNRunCost->value());	}
void Result::onShopListButtonClick()
{	emit showShopList();	}


void Result::setMinUnitCost(double d)
{	lb->at(2)->setText(toStr(d));align(2);	};
void Result::setMinRunCost(double d)
{	lb->at(4)->setText(toStr(d));align(4);	};
void Result::setMinNRunCost(double d)
{	lb->at(6)->setText(toStr(d));align(6);	};
			        
void Result::setBuy(double d)
{	lb->at(13)->setText(redStr(d));align(13);	};
void Result::setProd(double d)
{	lb->at(17)->setText(redStr(d));align(17);	};
void Result::setBuyTax(double d)
{	lb->at(15)->setText(redStr(d));align(15);	};
void Result::setSellTax(double d)
{	lb->at(23)->setText(redStr(d));align(23);	};
void Result::setCost(double d)
{	lb->at(19)->setText(redStr(d));align(19);	};
void Result::setSell(double d)
{	lb->at(21)->setText(greenStr(d));align(21);	};
void Result::setBrokerTax(double d)
{	lb->at(25)->setText(redStr(d));align(25);	};
void Result::setProfit(double d)
{
	(d < 0) ? lb->at(27)->setText(redStr(d)) : lb->at(27)->setText(greenStr(d));
	align(27);
};

void Result::setTargetUnitCost(double d)
{	sbUnitCost->setValue(d);	}
void Result::setTargetRunPrice(double d)
{	sbRunCost->setValue(d);	}
void Result::setTargetNRunPrice(double d)
{	sbNRunCost->setValue(d);	}

void Result::setTotalProdTime(int d, int h, int m, int s)
{	lbData[29]->setText(toStr(d) + "d " + toStr(h) + "h " + toStr(m) + "m " + toStr(s) + "s");	}
void Result::setTotalUnits(int i)
{	lbData[31]->setText(toStr(i));	}


Result::~Result()
{}

