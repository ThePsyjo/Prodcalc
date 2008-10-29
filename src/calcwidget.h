
/************************************************************************
 * calcwidget.h								*
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

#ifndef CALC_WIDGET_H
#define CALC_WIDGET_H

#include <QLabel>
#include <QVector>
#include <QGridLayout>
#include "mineralCosts.h"
#include "blueprint.h"
#include "production.h"
#include "tax.h"
#include "skill.h"
#include "result.h"
#include "configuration.h"
#include "shoppingList.h"

class CalcWidget : public QWidget
{
Q_OBJECT
public:
	CalcWidget( QWidget * parent =0 );
	virtual ~CalcWidget();
private:
	BpConfig *bpConf;

	QGridLayout *layout;

	MineralCosts *costs;
	Blueprint *bp;
	Production *prod;
	Tax *tax;
	Result *result;
	ConfigHandler *conf;
	Skill *skill;

	/*calculating variables*/
	QVector<double> *mCost;
	double install, rental, sellTax, buyTax, sellTaxValue, buyTaxValue, broker, brokerValue, targetUnitCost, targetRunPrice, targetNRunPrice, others, mSumCost, sellPrice, prodCost, sumCost, suggest;
	int stack, runs, cnt, industrySkill, producteffSkill;
	double calcSuggest(double, double);
	int d2i(double);
public slots:
	void recalc();
private slots:
	void onCostChange(QVector<double>*);
	void onRentalChange(double);
	void onSellTaxChange(double);
	void onBuyTaxChange(double);
	void onBrokerChange(double);
	void onTargetUnitCostChange(double);
	void onTargetRunPriceChange(double);
	void onTargetNRunPriceChange(double);
	void onOthersChange(double);
	void onStackChange(int);
	void onRunsChange(int);
	void onInstallChange(double);
	void onBpMenuAction(QString);
	void onBpSaveClick(BpConfig*);
	void onBpDelClick(QString);
	void genShopList();
	void onProducteffSkillChange(int);
	void onIndustrySkillChange(int);
	void onBpConfigChanged(BpConfig*);
};

#endif

