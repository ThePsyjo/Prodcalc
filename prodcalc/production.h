
/************************************************************************
 * production.h								*
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

#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <QWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QVBoxLayout>

#include <QGroupBox>
//class Production : public QWidget
class Production : public QGroupBox
{
Q_OBJECT
public:
	Production(double, double, QWidget*);
	virtual ~Production();
private:
	QLabel *lRental, *lRuns, *lOthers, *lInstall;
	QDoubleSpinBox *sbRental, *sbOthers, *sbInstall;
	QSpinBox *sbRuns;
	QVBoxLayout *layout;
private slots:
	void onRentalChange(double);
	void onRunsChange(int);
	void onOthersChange(double);
	void onInstallChange(double);
signals:
	void rentalChanged(double);
	void runsChanged(int);
	void othersChanged(double);
	void installChanged(double);
};



#endif

