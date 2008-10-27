
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
	lDurations	= new QLabel ("0", this);
	lDurationm	= new QLabel ("0", this);
	lDurationh	= new QLabel ("0", this);
	lDurationd	= new QLabel ("0", this);
	lStack 		= new QLabel(tr("stack"));
	lStackV		= new QLabel ("0", this);

	lPe		= new QLabel (tr("Production Level"), this);
	lMe		= new QLabel (tr("Material Level"), this);
	sbPe		= new QSpinBox (this);
	sbMe		= new QSpinBox (this);

	sbMe->setMaximum(1000000);
	sbPe->setMaximum(1000000);

	hl = new QHBoxLayout;

	bpSelect->setMinimumWidth(120);
	bpSelect->setMenu(bpSelectMenu);

	layout->addWidget(lDuration, 0, 3, 1, 3);

	hl->addWidget(lDurationd);
	hl->addWidget(lDurationh);
	hl->addWidget(lDurationm);
	hl->addWidget(lDurations);

	layout->addLayout(hl, 1, 3, 1, 3);

	layout->addWidget(lStack,  2, 3, 1, 3);
	layout->addWidget(lStackV, 3, 3, 1, 3);

	layout->addWidget(lMe,  4, 3, 1, 3);
	layout->addWidget(sbMe, 5, 3, 1, 3);
	layout->addWidget(lPe,  6, 3, 1, 3);
	layout->addWidget(sbPe, 7, 3, 1, 3);

	layout->addWidget(bpSelect, 0, 7);
	layout->addWidget(delButton, 2, 7);
	layout->addWidget(newButton, 3, 7);
	layout->addWidget(saveButton, 4, 7);

	adjustSize();

	connect(bpSelectMenu	, SIGNAL(triggered(QAction*)),	this, SLOT(onMenuAction(QAction*)));
	connect(saveButton	, SIGNAL(clicked()), this, SLOT(onSaveClick()));
	connect(newButton	, SIGNAL(clicked()), this, SLOT(onNewClick()));
	connect(delButton	, SIGNAL(clicked()), this, SLOT(onDelClick()));

	connect(sbMe		, SIGNAL(valueChanged(int)), this, SLOT(onMeChange(int)));
	connect(sbPe		, SIGNAL(valueChanged(int)), this, SLOT(onPeChange(int)));
}

Blueprint::~Blueprint()
{}

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

QString Blueprint::toStr(int i, QString ex)
{ return QString("%L1" + ex).arg(i, 3); }

void Blueprint::setBp(BpConfig* c)
{
	bpSelect->setText(c->name);

	for (int i = 0; i<8; i++)
		cnt[i]->setText(toStr(c->cnt->at(i), ""));

	int d, h, m, s;
	d    = unsigned( c->prodTime / 86400 );
	h    = unsigned( c->prodTime - d * 86400) / 3600;
	m    = unsigned( c->prodTime - d * 86400 - h * 3600 ) / 60;
	s    = unsigned( c->prodTime - d * 86400 - h * 3600 - m * 60 );
	lDurationd->setText(toStr(d, " d"));
	lDurationh->setText(toStr(h, " h"));
	lDurationm->setText(toStr(m, " m"));
	lDurations->setText(toStr(s, " s"));

	lStackV->setText(toStr(c->stackSize, ""));
	sbMe->setValue(c->me);
	sbPe->setValue(c->pe);
}

BpConfig* Blueprint::getBp()
{
	BpConfig *bpConf = new BpConfig;
	
	bpConf->name = bpSelect->text();

	for (int i = 0; i<8; i++)
		bpConf->cnt->insert( i,	cnt[i]->text().toInt());

	bpConf->prodTime = lDurationd->text().toInt() * 86400 + lDurationh->text().toInt() * 3600 + lDurationm->text().toInt() * 60 + lDurations->text().toInt();

	bpConf->stackSize	= lStackV->text().toInt();
	bpConf->me		= sbMe->value();
	bpConf->pe		= sbPe->value();

	return bpConf;
}

void Blueprint::setBpSelectBold(bool b)
{
        (b) ? bpSelect->setFont(QFont("Helvetica", 10, QFont::Bold)) : bpSelect->setFont(QFont("Helvetica", 10));
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

void Blueprint::onSaveClick()
{
	emit saveBp();
	setBpSelectBold(0);
}

void Blueprint::onDelClick()
{
	DelDialog *dialog = new DelDialog (tr("really?"), tr("Really remove \"%1\" ?").arg(bpSelect->text()), this);
	dialog->show();
	if(!dialog->exec())
	{
		return;
	}
	emit delBp(bpSelect->text());
	setBpSelectBold(0);
	bpSelect->setText(tr("Blueprint"));
}

void Blueprint::onMeChange(int i)
{
	emit meChanged(i);
	setBpSelectBold(1);
}

void Blueprint::onPeChange(int i)
{
	emit peChanged(i);
	setBpSelectBold(1);
}

