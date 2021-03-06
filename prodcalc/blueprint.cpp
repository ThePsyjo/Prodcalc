
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
        modButton	= new QPushButton (tr("&modify"), this);

	lProdTime	= new QLabel(tr("productiontime"), this);
	lProdTimeV	= new QLabel ("0", this);
	lStack 		= new QLabel(tr("stack"));
	lStackV		= new QLabel ("0", this);

	lPe		= new QLabel (tr("Production Level"), this);
	lMe		= new QLabel (tr("Material Level"), this);
	sbPe		= new QSpinBox (this);
	sbMe		= new QSpinBox (this);

	bpConf = new BpConfig;

	sbMe->setMaximum(1000000);
	sbPe->setMaximum(1000000);
	sbMe->setMinimum(-10);
	sbPe->setMinimum(-10);

	bpSelect->setMinimumWidth(120);
	bpSelect->setMenu(bpSelectMenu);

	layout->addWidget(lProdTime, 0, 3);
	layout->addWidget(lProdTimeV, 1, 3);

	layout->addWidget(lStack,  2, 3);
	layout->addWidget(lStackV, 3, 3);

	layout->addWidget(lMe,  4, 3);
	layout->addWidget(sbMe, 5, 3);
	layout->addWidget(lPe,  6, 3);
	layout->addWidget(sbPe, 7, 3);

	layout->addWidget(bpSelect, 0, 4);
	layout->addWidget(newButton, 2, 4);
	layout->addWidget(modButton, 3, 4);
	layout->addWidget(saveButton, 4, 4);
	layout->addWidget(delButton, 5, 4);

	adjustSize();

	connect(bpSelectMenu	, SIGNAL(triggered(QAction*)),	this, SLOT(onMenuAction(QAction*)));
	connect(saveButton	, SIGNAL(clicked()), this, SLOT(onSaveClick()));
	connect(newButton	, SIGNAL(clicked()), this, SLOT(onNewClick()));
	connect(delButton	, SIGNAL(clicked()), this, SLOT(onDelClick()));
	connect(modButton	, SIGNAL(clicked()), this, SLOT(onModClick()));

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
	bpConf = c;

	bpSelect->setText(c->name);

	for (int i = 0; i<8; i++)
		cnt[i]->setText(toStr(c->cnt->at(i), ""));

	int d, h, m, s;
	d    = unsigned( c->prodTime / 86400 );
	h    = unsigned( c->prodTime - d * 86400) / 3600;
	m    = unsigned( c->prodTime - d * 86400 - h * 3600 ) / 60;
	s    = unsigned( c->prodTime - d * 86400 - h * 3600 - m * 60 );
	lProdTimeV->setText(toStr(d, " d, ") + toStr(h, " h, ") + toStr(m, " m, ") + toStr(s, " s"));

	lStackV->setText(toStr(c->stackSize, ""));
	sbMe->setValue(c->me);
	sbPe->setValue(c->pe);
}

BpConfig* Blueprint::getBp()
{	return bpConf;	}

void Blueprint::setBpSelectBold(bool b)
{
        (b) ? bpSelect->setFont(QFont("Helvetica", 10, QFont::Bold)) : bpSelect->setFont(QFont("Helvetica", 10));
}

void Blueprint::onNewClick()
{
	BpConfig *c = new BpConfig;
	bool ready, done;
	do
	{
		BlueprintInputMask input(tr("new blueprint"), c, false, true,  this);
		input.show();
		done = input.exec();
		ready = true;

		if(done)
		{
			for ( int i = 0; i < bpSelectMenu->actions().count(); i++ )	//
			{								// check if name already exists
				if(bpSelectMenu->actions().at(i)->text()==c->name)	//
				{							//
					ready = false;					//
					QMessageBox::warning(this, tr("name collision"), tr("name \"%1\" already exists").arg(c->name));
				}							//
			}								//
		}
	}while(!ready);
	if(done)
	{
		emit bpConfChanged(c);
		bpSelectMenu->addAction(c->name);
		setBpSelectBold(1);
	}
}

void Blueprint::onSaveClick()
{
	emit saveBp(bpConf);
	setBpSelectBold(0);
}

void Blueprint::onModClick()
{
	BlueprintInputMask *input = new BlueprintInputMask(tr("mod blueprint"), bpConf, true, false, this);
	input->show();
	if(input->exec())
	{
		emit bpConfChanged(bpConf);
		setBpSelectBold(1);
	}
}

void Blueprint::onDelClick()
{
	DelDialog *dialog = new DelDialog (tr("really?"), tr("Really remove \"%1\" ?").arg(bpSelect->text()), this);
	dialog->show();
	if(dialog->exec())
	{
		emit delBp(bpSelect->text());
		setBpSelectBold(0);
		bpSelect->setText(tr("Blueprint"));
	}
}

void Blueprint::onMeChange(int i)
{
	bpConf->me=i;
	emit bpConfChanged(bpConf);
	setBpSelectBold(1);
}

void Blueprint::onPeChange(int i)
{
	bpConf->pe=i;
	emit bpConfChanged(bpConf);
	setBpSelectBold(1);
}
