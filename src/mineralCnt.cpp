
/************************************************************************
 * mineralCnt.cpp							*
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

#include "mineralCnt.h"

MineralCnt::MineralCnt( QWidget * parent)
        : Minerals(parent)
{
	setParent(parent);

	sb = new QVector<QSpinBox*>(8);
	sbData = sb->data();
	cntVector = new QVector<int>(8);

	for (int i=0; i<8; i++)
	{
		sbData[i] = new QSpinBox (this);
		sbData[i]->setMaximum(1000000000); // 1bio
		layout->addWidget(sbData[i], i, 2);
		connect(sbData[i], SIGNAL(valueChanged(int)), this, SLOT(onChange()));
	}
	
	adjustSize();
}

void MineralCnt::onChange()
{
	for (int i=0; i<8; i++)
		cntVector->insert(i, sb->at(i)->value());

	emit cntChanged(cntVector);
}



MineralCnt::~MineralCnt()
{}


