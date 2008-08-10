
/************************************************************************
 * calcwidget.cpp							*
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

#include "calcwidget.h"
//#define SHOW_EVENTS
//#define DEBUG
#ifdef DEBUG
#include <QMessageBox>
#endif

CalcWidget::CalcWidget( QWidget * parent)
{
	setParent(parent);

	mCost = new QVector<double>(8);
	mCnt = new QVector<int>(8);
	install = rental = sellTaxValue = buyTaxValue = brokerValue = others = targetUnitCost = targetRunPrice = targetNRunPrice = mSumCost = sellPrice = prodCost = prodTime = sumCost = 0;
	stack = runs = cnt = 0;

	conf = new ConfigHandler;

	parent->setStyleSheet(conf->loadStyleSheet());
	
#ifdef DEBUG
	QMessageBox::information(NULL, "parse",conf->loadStyleSheet());
#endif

	layout = new QGridLayout(this);
	costs = new MineralCosts (conf->loadMineralPrice(), this);
	bp = new Blueprint (this);
	prod = new Production (conf->loadRental(), conf->loadInstallFee(), this);
	tax = new Tax(conf->loadBuyTax(), conf->loadSellTax(), conf->loadBrokerTax(), this);
	result = new Result(this);

	buyTax  = conf->loadBuyTax();
	sellTax = conf->loadSellTax();
	broker  = conf->loadBrokerTax();
	for (int i = 0; i<8; i++)
		mCost->insert(i, conf->loadMineralPrice().at(i));
	rental  = conf->loadRental();
	install = conf->loadInstallFee();

	bp->setBpList(conf->loadBpList());

	layout->addWidget(costs , 0, 0);
	layout->addWidget(bp	 , 0, 1, 1, 1);
	layout->addWidget(prod	 , 0, 2);
	layout->addWidget(tax	 , 1, 2);
	layout->addWidget(result, 1, 0, 1, 2);
	adjustSize();

	connect(costs,	SIGNAL(costChanged(QVector<double>*)),	this, SLOT(onCostChange(QVector<double>*)));
	connect(bp,	SIGNAL(cntChanged(QVector<int>*)),	this, SLOT(onCntChange(QVector<int>*)));
	connect(bp,	SIGNAL(stackChanged(int)),	this, SLOT(onStackChange(int))		);
	connect(tax,	SIGNAL(sellTaxChanged(double)),	this, SLOT(onSellTaxChange(double))	);
	connect(tax,	SIGNAL(buyTaxChanged(double)),	this, SLOT(onBuyTaxChange(double))	);
	connect(tax,	SIGNAL(brokerTaxChanged(double)),this,SLOT(onBrokerChange(double))	);
	connect(result,	SIGNAL(targetUnitCostChanged(double)),		this, SLOT(onTargetUnitCostChange(double))			);
	connect(result,	SIGNAL(targetRunPriceChanged(double)),		this, SLOT(onTargetRunPriceChange(double))			);
	connect(result,	SIGNAL(targetNRunPriceChanged(double)),		this, SLOT(onTargetNRunPriceChange(double))			);
	connect(result,	SIGNAL(showShopList()),		this, SLOT(genShopList())		);
	connect(prod,	SIGNAL(rentalChanged(double)),	this, SLOT(onRentalChange(double))	);
	connect(prod,	SIGNAL(installChanged(double)),	this, SLOT(onInstallChange(double))	);
	connect(prod,	SIGNAL(othersChanged(double)),	this, SLOT(onOthersChange(double))	);
	connect(prod,	SIGNAL(runsChanged(int)),	this, SLOT(onRunsChange(int))		);

	connect(conf,	SIGNAL(bpListChanged(QVector<QString>)),	bp, SLOT(setBpList(QVector<QString>))				);
	connect(bp,	SIGNAL(needBp(QString)),	this, SLOT(onBpMenuAction(QString))	);
	connect(bp,	SIGNAL(saveBp(BpConfig*)),	this, SLOT(onBpSaveClick(BpConfig*))	);
	connect(bp,	SIGNAL(delBp(QString)),		this, SLOT(onBpDelClick(QString))	);
	connect(bp,	SIGNAL(durationChanged(int)),	this, SLOT(onDurationChange(int))	);
}

void CalcWidget::recalc(bool rMSumCost, bool rBuyTax, bool rProdCost, bool rSumCost, bool rSellPrice, bool rSellTax, bool rBroker, bool rTargetPrice, bool rSuggest)
{
	if ( rMSumCost )
	{
		mSumCost = 0;
		for (int i = 0; i<8; i++)
			mSumCost += mCost->at(i) * mCnt->at(i);
		mSumCost *= runs;
		result->setBuy(mSumCost);
	}
		
	if(rBuyTax)
	{
		buyTaxValue = mSumCost * (buyTax/100);
		result->setBuyTax(buyTaxValue);
	}
	if(rProdCost)
	{
		prodCost = others + install + rental * ((prodTime * runs) / 3600);
		result->setProd(prodCost);
	        int d, h, m, s;
	        d = unsigned( ( prodTime * runs ) / 86400 );
	        h = unsigned( ( prodTime * runs ) - d * 86400) / 3600;
	        m = unsigned( ( prodTime * runs ) - d * 86400 - h * 3600 ) / 60;
	        s = unsigned( ( prodTime * runs ) - d * 86400 - h * 3600 - m * 60 );
		result->setTotalProdTime(d,h,m,s);
	}
	if(rSellPrice)
	{
		sellPrice = targetUnitCost * runs * stack;
		result->setSell(sellPrice);
		result->setTotalUnits(runs*stack);
	}
	if(rSumCost)
	{
		sumCost = prodCost + mSumCost + buyTaxValue;
		result->setCost(sumCost);
	}
	if(rSellTax)
	{
		sellTaxValue = sellPrice * (sellTax/100);
		result->setSellTax(sellTaxValue);
	}
	if(rBroker)
	{
		brokerValue = sellPrice * (broker/100);
		result->setBrokerTax(brokerValue);
	}
	if(rTargetPrice)
	{
       		targetRunPrice = targetUnitCost * stack;
	        targetNRunPrice = targetRunPrice * runs;
		result->setTargetUnitCost(targetUnitCost);
		result->setTargetRunPrice(targetRunPrice);
		result->setTargetNRunPrice(targetNRunPrice);
	}
	if(rSuggest)
	{
		suggest=calcSuggest(0,sumCost);
		result->setMinUnitCost(( runs == 0 || stack == 0 ) ?  0 : suggest / runs / stack );
		result->setMinRunCost(( runs == 0 ) ?  0 : suggest / runs );
		result->setMinNRunCost( suggest	);
	}


//	suggest=(
//	    	  (
//		    sumCost * (1+broker/100) * (1+sellTax/100)		// geil wa !?!
//		    * (1+broker/100) * (1+sellTax/100)
//		  ) - sumCost * (1+broker/100) * (1+sellTax/100)
//		) + sumCost;

	result->setProfit(sellPrice - sellTaxValue - brokerValue - sumCost);

}

void CalcWidget::onCostChange(QVector<double>* v)
{
	mCost = v;
	conf->saveMineralPrice(v);
	recalc(1,1,0,1,1,1,1,0,1);
}

void CalcWidget::onCntChange(QVector<int>* v)
{
	mCnt = v;
	recalc(1,1,0,1,1,1,1,0,1);
}

void CalcWidget::onDurationChange(int i)
{
	prodTime = i;
	recalc(0,0,1,1,0,0,0,0,1); // recalc
}

void CalcWidget::onRentalChange(double d)
{
	rental = d;
	conf->saveRental(d);
	recalc(0,0,1,1,0,1,1,0,1);
}

void CalcWidget::onInstallChange(double d)
{
	install = d;
	conf->saveInstallFee(d);
	recalc(0,0,1,1,0,1,1,0,1);
}

void CalcWidget::onSellTaxChange(double d)
{
	sellTax = d;
	conf->saveSellTax(d);
	recalc(0,0,0,0,0,1,0,0,1);
}

void CalcWidget::onBuyTaxChange(double d)
{
	buyTax = d;
	conf->saveBuyTax(d);
	recalc(0,1,0,1,0,0,0,0,1);
}

void CalcWidget::onBrokerChange(double d)
{
	broker = d;
	conf->saveBrokerTax(d);
	recalc(0,0,0,0,0,0,1,0,1);
}

void CalcWidget::onTargetUnitCostChange(double d)
{
	targetUnitCost = d;
	targetRunPrice = d * stack;
	targetNRunPrice = targetRunPrice * runs;
	recalc(0,0,0,0,1,1,1,1,0);
}

void CalcWidget::onTargetRunPriceChange(double d)
{
	targetRunPrice = d;
	targetUnitCost = d / stack;
	targetNRunPrice = d * runs;
	recalc(0,0,0,0,1,1,1,1,0);
}

void CalcWidget::onTargetNRunPriceChange(double d)
{
	targetNRunPrice = d;
	targetRunPrice = d / runs;
	targetUnitCost = targetRunPrice / stack;
	recalc(0,0,0,0,1,1,1,1,0);
}

void CalcWidget::onOthersChange(double d)
{
	others = d;
	recalc(0,0,1,1,0,0,0,0,1);
}
void CalcWidget::onStackChange(int i)
{
	stack = i;
	recalc(0,0,0,0,1,1,1,1,1);
}

void CalcWidget::onRunsChange(int i)
{
	runs = i;
	recalc(1,1,1,1,1,1,1,1,1);
}

double CalcWidget::calcSuggest(double d, double o)
{
#ifdef DEBUG
printf("%2d: %0.3f - %0.3f\n", cnt, d, o);
#endif

	double v = o * (1+((broker/100) + (sellTax/100))) - d;
	d = o * (broker/100 + sellTax/100);
	
	// break if value is reached || recursion is too deep
	if(o==v || ++cnt > 99) {cnt = 0; return o;}
	else	 return calcSuggest(d,v);
}

void CalcWidget::onBpMenuAction(QString s)
{
	bp->setBp(conf->loadBlueprint(s));
	recalc(1,1,1,1,1,1,1,1,1);
}

void CalcWidget::onBpSaveClick(BpConfig* c)
{
	conf->saveBlueprint(c);
}

void CalcWidget::onBpDelClick(QString s)
{
	conf->delBlueprint(s);
}

void CalcWidget::genShopList()
{
	ShopList *dialog = new ShopList (mCnt, runs, mCost, this);
	dialog->show();
}

CalcWidget::~CalcWidget()
{delete conf;}


