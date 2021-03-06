
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

	bpConf = new BpConfig;
	mCost = new QVector<double>(8);
	install = rental = sellTaxValue = buyTaxValue = brokerValue = others = targetUnitCost = targetRunPrice = targetNRunPrice = mSumCost = sellPrice = prodCost = sumCost = 0;
	runs = cnt = industrySkill = producteffSkill = 0;

	conf = new ConfigHandler(QDir::toNativeSeparators(QDir::homePath ()  + "/.prodcalc.xml"));

	parent->setStyleSheet(conf->loadStyleSheet());
	
	layout = new QGridLayout(this);
	costs = new MineralCosts (conf->loadMineralPrice(), this);
	bp = new Blueprint (this);
	prod = new Production (conf->loadRental(), conf->loadInstallFee(), this);
	tax = new Tax(conf->loadBuyTax(), conf->loadSellTax(), conf->loadBrokerTax(), this);
	result = new Result(this);
	skill = new Skill(conf->loadIndustrySkill(), conf->loadProducteffSkill(), this);

	buyTax  = conf->loadBuyTax();
	sellTax = conf->loadSellTax();
	broker  = conf->loadBrokerTax();
	for (int i = 0; i<8; i++)
		mCost->insert(i, conf->loadMineralPrice().at(i));
	rental  = conf->loadRental();
	install = conf->loadInstallFee();
	
	industrySkill = conf->loadIndustrySkill();
	producteffSkill = conf->loadProducteffSkill();

	bp->setBpList(conf->loadBpList());

	layout->addWidget(costs , 0, 0);
	layout->addWidget(bp	 , 0, 1, 1, 4);
	layout->addWidget(skill	 , 1, 2);
	layout->addWidget(prod	 , 1, 3);
	layout->addWidget(tax	 , 1, 4);
	layout->addWidget(result, 1, 0, 1, 2);
	adjustSize();

	connect(costs,	SIGNAL(costChanged(QVector<double>*)),	this, SLOT(onCostChange(QVector<double>*)));
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

	connect(skill,	SIGNAL(indSkillChanged(int)),	this, SLOT(onIndustrySkillChange(int))	);
	connect(skill,	SIGNAL(peSkillChanged(int)),	this, SLOT(onProducteffSkillChange(int)));

	connect(conf,	SIGNAL(bpListChanged(QVector<QString>)),	bp, SLOT(setBpList(QVector<QString>))				);
	connect(bp,	SIGNAL(needBp(QString)),	this, SLOT(onBpMenuAction(QString))	);
	connect(bp,	SIGNAL(delBp(QString)),		this, SLOT(onBpDelClick(QString))	);
	connect(bp,	SIGNAL(saveBp(BpConfig*)),	this, SLOT(onBpSaveClick(BpConfig*))	);
	connect(bp,	SIGNAL(bpConfChanged(BpConfig*)),this,SLOT(onBpConfigChanged(BpConfig*)));

	connect(parent, SIGNAL(mineralPriceUpdateRequest()),	costs,	SLOT(updatePriceFromWeb()));
}

int CalcWidget::d2i(double d){return int(d<0?d-.5:d+.5);}
void CalcWidget::recalc()
{
//   reduce overhead here :/
// isn't there any chance to shorten this ??
// i hate it !
	bpConf->prodTime = d2i((bpConf->baseProdTime*( (bpConf->pe>=0) ? 0.8+0.2/(1+bpConf->pe) : 0.8+0.2*(pow(2,(bpConf->pe*-1))) ))*(1-0.04*industrySkill));
	for (int i = 0; i<8; i++)
		bpConf->cnt->insert(i, d2i(bpConf->baseCnt->at(i)*((bpConf->me>=0) ? 1+0.1/(bpConf->me+1) : 1+0.1*(pow(2,(bpConf->me*-1))) )*(1.25-0.05*producteffSkill)));
//		bpConf->cnt->insert(i, d2i(bpConf->baseCnt->at(i)*(1+0.1/(pow(2,bpConf->me)))*(1.25-0.05*producteffSkill))); // where is the trick ?
	bp->setBp(bpConf);
	//
	mSumCost = 0;
	for (int i = 0; i<8; i++)
		mSumCost += mCost->at(i) * bpConf->cnt->at(i);
	mSumCost *= runs;
	result->setBuy(mSumCost);
	//	
	buyTaxValue = mSumCost * (buyTax/100);
	result->setBuyTax(buyTaxValue);
	//
	prodCost = others + install + rental * ((bpConf->prodTime * runs) / 3600);
	result->setProd(prodCost);
	int d, h, m, s;
	d = unsigned( ( bpConf->prodTime * runs ) / 86400 );
	h = unsigned( ( bpConf->prodTime * runs ) - d * 86400) / 3600;
	m = unsigned( ( bpConf->prodTime * runs ) - d * 86400 - h * 3600 ) / 60;
	s = unsigned( ( bpConf->prodTime * runs ) - d * 86400 - h * 3600 - m * 60 );
	result->setTotalProdTime(d,h,m,s);
	//
	sumCost = prodCost + mSumCost + buyTaxValue;
	result->setCost(sumCost);
	//
	sellPrice = targetUnitCost * runs * bpConf->stackSize;
	result->setSell(sellPrice);
	result->setTotalUnits(runs*bpConf->stackSize);
	//
	sellTaxValue = sellPrice * (sellTax/100);
	result->setSellTax(sellTaxValue);
	//
	brokerValue = sellPrice * (broker/100);
	result->setBrokerTax(brokerValue);
	//
       	targetRunPrice = targetUnitCost * bpConf->stackSize;
	targetNRunPrice = targetRunPrice * runs;
	result->setTargetUnitCost(targetUnitCost);
	result->setTargetRunPrice(targetRunPrice);
	result->setTargetNRunPrice(targetNRunPrice);
	//
	suggest=calcSuggest(0,sumCost);
	result->setMinUnitCost(( runs == 0 || bpConf->stackSize == 0 ) ?  0 : suggest / runs / bpConf->stackSize );
	result->setMinRunCost(( runs == 0 ) ?  0 : suggest / runs );
	result->setMinNRunCost( suggest	);
	//
	result->setProfit(sellPrice - sellTaxValue - brokerValue - sumCost);
}

void CalcWidget::onCostChange(QVector<double>* v)
{
	mCost = v;
	conf->saveMineralPrice(v);
//	recalc(1,1,0,1,1,1,1,0,1);
	recalc();
}

void CalcWidget::onRentalChange(double d)
{
	rental = d;
	conf->saveRental(d);
//	recalc(0,0,1,1,0,1,1,0,1);
	recalc();
}

void CalcWidget::onInstallChange(double d)
{
	install = d;
	conf->saveInstallFee(d);
//	recalc(0,0,1,1,0,1,1,0,1);
	recalc();
}

void CalcWidget::onSellTaxChange(double d)
{
	sellTax = d;
	conf->saveSellTax(d);
//	recalc(0,0,0,0,0,1,0,0,1);
	recalc();
}

void CalcWidget::onBuyTaxChange(double d)
{
	buyTax = d;
	conf->saveBuyTax(d);
//	recalc(0,1,0,1,0,0,0,0,1);
	recalc();
}

void CalcWidget::onBrokerChange(double d)
{
	broker = d;
	conf->saveBrokerTax(d);
//	recalc(0,0,0,0,0,0,1,0,1);
	recalc();
}

void CalcWidget::onTargetUnitCostChange(double d)
{
	targetUnitCost = d;
	targetRunPrice = d * bpConf->stackSize;
	targetNRunPrice = targetRunPrice * runs;
//	recalc(0,0,0,0,1,1,1,1,0);
	recalc();
}

void CalcWidget::onTargetRunPriceChange(double d)
{
	targetRunPrice = d;
	targetUnitCost = d / bpConf->stackSize;
	targetNRunPrice = d * runs;
//	recalc(0,0,0,0,1,1,1,1,0);
	recalc();
}

void CalcWidget::onTargetNRunPriceChange(double d)
{
	targetNRunPrice = d;
	targetRunPrice = d / runs;
	targetUnitCost = targetRunPrice / bpConf->stackSize;
//	recalc(0,0,0,0,1,1,1,1,0);
	recalc();
}

void CalcWidget::onOthersChange(double d)
{
	others = d;
//	recalc(0,0,1,1,0,0,0,0,1);
	recalc();
}
void CalcWidget::onStackChange(int i)
{
	bpConf->stackSize = i;
//	recalc(0,0,0,0,1,1,1,1,1);
	recalc();
}

void CalcWidget::onRunsChange(int i)
{
	runs = i;
//	recalc(1,1,1,1,1,1,1,1,1);
	recalc();
}

void CalcWidget::onProducteffSkillChange(int i)
{
	producteffSkill = i;
        conf->saveProducteffSkill(i);
	recalc();
}

void CalcWidget::onIndustrySkillChange(int i)
{
	industrySkill = i;
        conf->saveIndustrySkill(i);
	recalc();
}

void CalcWidget::onBpConfigChanged(BpConfig *c)
{
	bpConf = c;
	recalc();
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
	bpConf = conf->loadBlueprint(s);
	//recalc(1,1,1,1,1,1,1,1,1);
	recalc();
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
	ShopList *dialog = new ShopList (bpConf->cnt, runs, mCost, this);
	dialog->show();
}

CalcWidget::~CalcWidget()
{delete conf;}


