
/************************************************************************
 * blueprintInputMask.cpp						*
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

#include "blueprintInputMask.h"

BlueprintInputMask::BlueprintInputMask( BpConfig *cnf )
//        : MineralsDialog(NULL)
{
	setWindowFlags(Qt::Dialog);
	c = cnf;
	//setModal(true);

	cntSb = new QVector<QSpinBox*>(8);
	sbData = cntSb->data();

	for (int i=0;i<8;i++)
	{
		sbData[i] = new QSpinBox (this);
		sbData[i]->setValue(c->cnt->at(i));
		sbData[i]->setMaximum(1000000); // 1 mio
		layout->addWidget(sbData[i], i, 2);
	}

        okButton	= new QPushButton (tr("&OK"), this);
        cancelButton	= new QPushButton (tr("&Cancel"), this);

	lDuration	= new QLabel(tr("productiontime"), this);

	int d, h, m, s;
	d = unsigned( c->prodTime / 86400 );
	h = unsigned( c->prodTime - d * 86400) / 3600;
	m = unsigned( c->prodTime - d * 86400 - h * 3600 ) / 60;
	s = unsigned( c->prodTime - d * 86400 - h * 3600 - m * 60 );

	eDurations	= new QSpinBox (this);
	eDurationm	= new QSpinBox (this);
	eDurationh	= new QSpinBox (this);
	eDurationd	= new QSpinBox (this);
	eDurations->setValue(s);
	eDurationm->setValue(m);
	eDurationh->setValue(h);
	eDurationd->setValue(d);
	lStack 		= new QLabel(tr("stack"));
	eStack		= new QSpinBox (this);
	eStack->setValue(c->stackSize);

	lPe  = new QLabel (tr("Production Level"), this);
	lMe  = new QLabel (tr("Material Level"), this);
	sbPe = new QSpinBox (this);
	sbMe = new QSpinBox (this);
	sbMe->setValue(c->me);
	sbPe->setValue(c->pe);

	lName = new QLabel("name", this);
	eName = new QLineEdit(this);
	eName->setText(c->name);

	eStack->setMaximum(1000000); // 1mio

	eDurationd->setSuffix("d");
	eDurationd->setMaximum(20);
	eDurationh->setSuffix("h");
	eDurationh->setMaximum(23);
	eDurationm->setSuffix("m");
	eDurationm->setMaximum(59);
	eDurations->setSuffix("s");
	eDurations->setMaximum(59);

	layout->addWidget(lDuration, 0, 3, 1, 4);

	layout->addWidget(eDurationd, 1, 3);
	layout->addWidget(eDurationh, 1, 4);
	layout->addWidget(eDurationm, 1, 5);
	layout->addWidget(eDurations, 1, 6);


	layout->addWidget(lStack, 2, 3, 1, 4);
	layout->addWidget(eStack, 3, 3, 1, 4);
	layout->addWidget(lMe, 4, 3, 1, 4);
	layout->addWidget(sbMe, 5, 3, 1, 4);
	layout->addWidget(lPe, 6, 3, 1, 4);
	layout->addWidget(sbPe, 7, 3, 1, 4);

	layout->addWidget(lName, 1, 7);
	layout->addWidget(eName, 2, 7);
	layout->addWidget(okButton, 6, 7);
	layout->addWidget(cancelButton, 7, 7);

	adjustSize();

	connect(okButton	, SIGNAL(clicked()), this, SLOT(onOkClick()));
	connect(cancelButton	, SIGNAL(clicked()), this, SLOT(onCancelClick()));
}

BlueprintInputMask::~BlueprintInputMask()
{}

void BlueprintInputMask::onOkClick()
{
	if( eName->text().isEmpty() )
	{
		lName->setStyleSheet("color:red;");
		return;
	}
	c->name      = eName->text();

	for (int i=0;i<8;i++)
		c->cnt->insert(i, sbData[i]->value());
	c->prodTime  = eDurationd->value() * 86400 + eDurationh->value() * 3600 + eDurationm->value() * 60 + eDurations->value();
	c->stackSize = eStack->value();
	c->me        = sbMe->value();
	c->pe        = sbPe->value();
	this->close();
}

void BlueprintInputMask::onCancelClick()
{this->close();}
