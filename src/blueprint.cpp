
/************************************************************************
 * blueprint.cpp							*
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

#include "blueprint.h"

Blueprint::Blueprint( QWidget * parent)
        : MineralCnt(parent)
{
        bpSelect	= new QPushButton (tr("Blueprint"), this);
        bpSelectMenu	= new QMenu (this);
        saveButton	= new QPushButton (tr("&save"), this);
        newButton	= new QPushButton (tr("&new..."), this);
        delButton	= new QPushButton (tr("&delete"), this);
	lDuration	= new QLabel(tr("productiontime"), this);
	eDurations	= new QSpinBox (this);
	eDurationm	= new QSpinBox (this);
	eDurationh	= new QSpinBox (this);
	eDurationd	= new QSpinBox (this);
	lStack 		= new QLabel(tr("stack"));
	eStack		= new QSpinBox (this);

	vl = new QVBoxLayout;
	hl = new QHBoxLayout;

	bpSelect->setMinimumWidth(120);
	bpSelect->setMenu(bpSelectMenu);

	eStack->setMaximum(1000000); // 1mio

	eDurationd->setSuffix("d");
	eDurationd->setMaximum(20);
	eDurationh->setSuffix("h");
	eDurationh->setMaximum(23);
	eDurationm->setSuffix("m");
	eDurationm->setMaximum(59);
	eDurations->setSuffix("s");
	eDurations->setMaximum(59);

	vl->addWidget(bpSelect);
	vl->addWidget(lDuration);

	hl->addWidget(eDurationd);
	hl->addWidget(eDurationh);
	hl->addWidget(eDurationm);
	hl->addWidget(eDurations);

	vl->addLayout(hl);

	vl->addWidget(lStack);
	vl->addWidget(eStack);
	vl->addWidget(delButton);
	vl->addWidget(newButton);
	vl->addWidget(saveButton);

	layout->addLayout(vl, 0,3, 8,3);

	adjustSize();


	connect(eDurationd	, SIGNAL(valueChanged(int)),	this, SLOT(onDurationChange()));
	connect(eDurationh	, SIGNAL(valueChanged(int)),	this, SLOT(onDurationChange()));
	connect(eDurationm	, SIGNAL(valueChanged(int)),	this, SLOT(onDurationChange()));
	connect(eDurations	, SIGNAL(valueChanged(int)),	this, SLOT(onDurationChange()));
	connect(eStack		, SIGNAL(valueChanged(int)),	this, SLOT(onStackChange(int)));
	connect(bpSelectMenu	, SIGNAL(triggered(QAction*)),	this, SLOT(onMenuAction(QAction*)));
	connect(saveButton	, SIGNAL(clicked()), this, SLOT(onSaveClick()));
	connect(newButton	, SIGNAL(clicked()), this, SLOT(onNewClick()));
	connect(delButton	, SIGNAL(clicked()), this, SLOT(onDelClick()));

	connect(this		, SIGNAL(cntChanged(QVector<int>*)),	this, SLOT(onMCntChange()));
}

Blueprint::~Blueprint()
{}

void Blueprint::setBpSelectBold(bool b)
{
	(b) ? bpSelect->setFont(QFont("Helvetica", 10, QFont::Bold)) : bpSelect->setFont(QFont("Helvetica", 10));
}

void Blueprint::onDurationChange()
{
	setBpSelectBold(1);
	emit durationChanged(eDurationd->value() * 24 * 60 * 60 + eDurationh->value() * 60 * 60 + eDurationm->value() * 60 + eDurations->value());
}
void Blueprint::onStackChange(int i)
{	setBpSelectBold(1); emit stackChanged(i);	}
void Blueprint::onMCntChange()
{	setBpSelectBold(1);	}

void Blueprint::setBpList(QVector<QString> v)
{
	bpSelectMenu->clear();

	foreach(QString s, v)
		bpSelectMenu->addAction(s);
}

void Blueprint::onMenuAction(QAction* a)
{
	emit needBp(a->text());
	setBpSelectBold(0);
}

void Blueprint::setBp(BpConfig* c)
{
	bpSelect->setText(c->name);

	for (int i = 0; i<8; i++)
		sbData[i]->setValue(c->cnt->at(i));

	eStack->setValue(c->stackSize);

	int d, h, m, s;
	d    = unsigned( c->prodTime / 86400 );
	h    = unsigned( c->prodTime - d * 86400) / 3600;
	m    = unsigned( c->prodTime - d * 86400 - h * 3600 ) / 60;
	s    = unsigned( c->prodTime - d * 86400 - h * 3600 - m * 60 );
	eDurationd->setValue(d);
	eDurationh->setValue(h);
	eDurationm->setValue(m);
	eDurations->setValue(s);
}

void Blueprint::onSaveClick()
{
	BpConfig *c = new BpConfig;

	c->name = bpSelect->text();
	for (int i = 0; i<8; i++)
		c->cnt->insert(i, sbData[i]->value());
	c->stackSize = eStack->value();
	c->prodTime = eDurationd->value() * 24 * 60 * 60 + eDurationh->value() * 60 * 60 + eDurationm->value() * 60 + eDurations->value();

	emit saveBp(c);
	setBpSelectBold(0);
}
void Blueprint::onNewClick()
{
	bool ok;
	QString text = 
	QInputDialog::getText(this, tr("save blueprint"), tr("Type the name of the new blueprint"), QLineEdit::Normal, "", &ok);
	if (ok && !text.isEmpty())
	{
/*		foreach(QString s, )
		{
			if ( s == text )
				return;
		}
*/						// check if it is already there
		bpSelectMenu->addAction(text);
		emit needBp(text);
		setBpSelectBold(1);
	}
}

void Blueprint::onDelClick()
{
	DelDialog *dialog = new DelDialog (tr("really?"), tr("Really remove \"") + bpSelect->text() + tr("\" ?"), this);
	dialog->show();
	if(!dialog->exec())
	{
		return;
	}
	emit delBp(bpSelect->text());
	setBpSelectBold(0);
	bpSelect->setText(tr("Blueprint"));
}








