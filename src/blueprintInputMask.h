
/************************************************************************
 * blueprintInputMask.h							*
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

#ifndef BLUEPRINT_INPUT_H
#define BLUEPRINT_INPUT_H

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVector>
#include <QString>
#include <QSpinBox>
#include <QDialog>

#include "mineralsDialog.h"
#include "bpConfig.h"

class BlueprintInputMask : public MineralsDialog
{
Q_OBJECT
public:
	BlueprintInputMask(QString, BpConfig*, QWidget* = 0);
	virtual ~BlueprintInputMask();
private:
	QVector<QSpinBox*> *cntSb;
	QSpinBox **sbData;
	QPushButton *okButton, *cancelButton;
	QLabel *lStack, *lDuration, *lMe, *lPe, *lName;
	QSpinBox *eDurationd, *eDurationh, *eDurationm, *eDurations;
	QSpinBox *eStack;
	QSpinBox *sbMe, *sbPe;
	QLineEdit *eName;
	BpConfig *c;
private slots:
	void onOkClick();
};


#endif

