
/************************************************************************
 * blueprint.h								*
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

#ifndef BLUEPRINT_H
#define BLUEPRINT_H

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMenu>
#include <QVector>
#include <QString>
#include <QSpinBox>
#include <QInputDialog>
#include <QMessageBox>

#include "mineralCnt.h"
#include "bpConfig.h"
#include "bpDelDialog.h"
#include "blueprintInputMask.h"

class Blueprint : public MineralCnt
{
Q_OBJECT
public:
	Blueprint(QWidget*);
	virtual ~Blueprint();
private:
	QPushButton *bpSelect, *saveButton, *newButton, *delButton, *modButton;
	QLabel *lStack, *lProdTime, *lPe, *lMe; // descripter
	QLabel *lProdTimeV, *lStackV; // value holder
	QSpinBox *sbPe, *sbMe;
	QMenu *bpSelectMenu;
	void setBpSelectBold(bool);
	QString toStr(int, QString);
	BpConfig *bpConf;
private slots:
	void onMenuAction(QAction*);
	void onSaveClick();
	void onNewClick();
	void onDelClick();
	void onModClick();
	void onMeChange(int);
	void onPeChange(int);
signals:
	void needBp(QString);
	void delBp(QString);
	void saveBp(BpConfig*);
	void bpConfChanged(BpConfig*);
public slots:
	void setBpList(QVector<QString>);
	void setBp(BpConfig*);
	BpConfig* getBp();
};


#endif

