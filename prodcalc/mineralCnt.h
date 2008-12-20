
/************************************************************************
 * mineralCnt.h								*
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

#ifndef MINERAL_CNT_H
#define MINERAL_CNT_H

#include <QVector>
#include "minerals.h"

class MineralCnt : public Minerals
{
Q_OBJECT
public:
	MineralCnt( QWidget * parent =0 );
	virtual ~MineralCnt();
private:
	QVector<QLabel*> *cntLb;
protected:
	QLabel** cnt;
};

#endif

