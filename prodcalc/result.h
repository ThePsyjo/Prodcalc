
/************************************************************************
 * result.h								*
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

#ifndef RESULT_H
#define RESULT_H

#include <QVector>
#include <QMap>
#include <QLabel>
#include <QString>
#include <QDoubleSpinBox>
#include <QGroupBox>
//#include <QWidget>
#include <QGridLayout>
#include <QPushButton>

//class Result : public QWidget
class Result : public QGroupBox
{
Q_OBJECT
public:
	Result(QWidget *parent);
	~Result();

	void setMinUnitCost(double);
	void setMinRunCost(double);
	void setMinNRunCost(double);

	void setTargetUnitCost(double);
	void setTargetRunPrice(double);
	void setTargetNRunPrice(double);

	void setBuy(double);
	void setProd(double);
	void setBuyTax(double);
	void setSellTax(double);
	void setBrokerTax(double);
	void setCost(double);
	void setSell(double);
	void setProfit(double);
	void setTotalProdTime(int, int, int, int);
	void setTotalUnits(int);
private:
	QVector<QLabel*> *lb;
	QMap<int, QString> *lbMap;
	QLabel** lbData;
	QDoubleSpinBox *sbUnitCost, *sbRunCost, *sbNRunCost;
        QGridLayout *layout;
	QString toStr(int);
	QString toStr(double);
	QString redStr(double);
	QString greenStr(double);
	QPushButton *shopListButton;
	void align(int);
private slots:
	void onUnitCostChange();
	void onRunCostChange();
	void onNRunCostChange();
	void onShopListButtonClick();
signals:
	void targetUnitCostChanged(double);
	void targetRunPriceChanged(double);
	void targetNRunPriceChanged(double);
	void showShopList();
};



#endif

