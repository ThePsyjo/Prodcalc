
/************************************************************************
 * configuration.h							*
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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//#include <QtXml/QDomDocument>
#include <QtXml> // works
#include <QFile>
#include <QVector>
#include <QString>
#include <QMessageBox>

#include "bpConfig.h"

class ConfigHandler : public QObject
{
Q_OBJECT
public:
	ConfigHandler();
	virtual ~ConfigHandler();

	// load functs
	QVector<QString> loadBpList();
	BpConfig* loadBlueprint(QString);
	QVector<double> loadMineralPrice();
	double loadInstallFee();
	double loadRental();
	double loadBuyTax();
	double loadSellTax();
	double loadBrokerTax();
	QString loadStyleSheet();
	int loadIndustrySkill();
	int loadProducteffSkill();
private:
	QDomDocument *doc;
	QFile *f;
	BpConfig *bpConf;
	QDomElement genTag(QDomElement, QString);
	bool tagCreated, saveOnExit;
public slots:
	// save functs
	void delBlueprint(QString);
	void saveBlueprint(BpConfig*);
	void saveMineralPrice(QVector<double>*);
	void saveBuyTax(double);
	void saveSellTax(double);
	void saveBrokerTax(double);
	void saveInstallFee(double);
	void saveRental(double);
	void saveIndustrySkill(int);
	void saveProducteffSkill(int);
signals:
	void bpListChanged(QVector<QString>);
};

#endif

