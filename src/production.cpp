
/************************************************************************
 * production.cpp							*
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

#include "production.h"

Production::Production(double rental, double install, QWidget *parent)
{
	setParent(parent);

	lRental  = new QLabel (tr("rental"), this);
	lInstall = new QLabel (tr("install cost"), this);
	lRuns    = new QLabel (tr("runs"), this);
	lOthers  = new QLabel (tr("other\n(research, etc.)"), this);

	sbRental = new QDoubleSpinBox (this);
	sbInstall= new QDoubleSpinBox (this);
	sbRuns   = new QSpinBox (this);
	sbOthers = new QDoubleSpinBox (this);

	sbRental->setSuffix(" ISK/h");
	sbRental->setMaximum(1000000); //1mio
	sbRental->setValue(rental);
	sbInstall->setSuffix(" ISK");
	sbInstall->setMaximum(1000000); //1mio
	sbInstall->setValue(install);
	sbOthers->setSuffix(" ISK");
	sbOthers->setMaximum(1000000000); // 1bio
	sbRuns->setMaximum(1000000000); // 1bio


	layout   = new QVBoxLayout(this);

	layout->addWidget(lRental);
	layout->addWidget(sbRental);
	layout->addWidget(lInstall);
	layout->addWidget(sbInstall);
	layout->addWidget(lOthers);
	layout->addWidget(sbOthers);
	layout->addWidget(lRuns);
	layout->addWidget(sbRuns);

	adjustSize();

	connect(sbRental, SIGNAL(valueChanged(double)), this, SLOT(onRentalChange(double)));
	connect(sbInstall,SIGNAL(valueChanged(double)), this, SLOT(onInstallChange(double)));
	connect(sbRuns, SIGNAL(valueChanged(int)), this, SLOT(onRunsChange(int)));
	connect(sbOthers, SIGNAL(valueChanged(double)), this, SLOT(onOthersChange(double)));
}

void Production::onRentalChange(double d)
{	emit rentalChanged(d);	}
void Production::onInstallChange(double d)
{	emit installChanged(d);	}
void Production::onRunsChange(int i)
{	emit runsChanged(i);	}
void Production::onOthersChange(double d)
{	emit othersChanged(d);	}



Production::~Production()
{}


