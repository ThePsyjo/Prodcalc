
/************************************************************************
 * skill.cpp								*
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

#include "skill.h"

Skill::Skill(int Ind, int Pe, QWidget *parent)
{
	setParent(parent);

	lInd	= new QLabel (tr("Industry level"), this);
	lPe	= new QLabel (tr("Production Efficiency Level"), this);

	sbInd	= new QSpinBox (this);
	sbPe	= new QSpinBox (this);

	sbInd->setValue(Ind);
	sbPe->setValue(Pe);

	sbInd->setMaximum(5);
	sbInd->setMinimum(0);
	sbPe->setMaximum(5);
	sbPe->setMinimum(0);

	layout   = new QVBoxLayout(this);

	layout->addWidget(lInd);
	layout->addWidget(sbInd);
//spacer
	layout->addWidget(lPe);
	layout->addWidget(sbPe);

	adjustSize();

	connect(sbInd,	  SIGNAL(valueChanged(int)), this, SLOT(onIndChange(int)));
	connect(sbPe,	  SIGNAL(valueChanged(int)), this, SLOT(onPeChange(int)));
}

void Skill::onIndChange(int i)
{	emit indSkillChanged(i);}
void Skill::onPeChange(int i)
{	emit peSkillChanged(i);	}

Skill::~Skill()
{}

