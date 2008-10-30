
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

	cntLb = new QVector<QLabel*>(8);
	cnt = cntLb->data();

	for (int i=0; i<8; i++)
	{
		cnt[i] = new QLabel ("0", this);
		layout->addWidget(cnt[i], i, 2);
		cnt[i]->adjustSize();
	}
	
	adjustSize();
}

MineralCnt::~MineralCnt()
{}


