
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

BlueprintInputMask::BlueprintInputMask( QString name, BpConfig *c, bool nameRo, bool templateList,  QWidget* parent )
{
	setParent(parent);
	setWindowFlags(Qt::Dialog);
	setWindowTitle(name);
	setModal(1);
	bpConf = c;

	cntSb = new QVector<QSpinBox*>(8);
	sbData = cntSb->data();

	for (int i=0;i<8;i++)
	{
		sbData[i] = new QSpinBox (this);
		sbData[i]->setMaximum(1000000000); //  1bio
		sbData[i]->setValue(bpConf->baseCnt->at(i));
		layout->addWidget(sbData[i], i, 2);
	}

        okButton	= new QPushButton (tr("&OK"), this);
        cancelButton	= new QPushButton (tr("&Cancel"), this);


	lDuration	= new QLabel(tr("productiontime"), this);

	int d, h, m, s;
	d = unsigned( bpConf->baseProdTime / 86400 );
	h = unsigned( bpConf->baseProdTime - d * 86400) / 3600;
	m = unsigned( bpConf->baseProdTime - d * 86400 - h * 3600 ) / 60;
	s = unsigned( bpConf->baseProdTime - d * 86400 - h * 3600 - m * 60 );

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
	eStack->setMaximum(1000000); // 1mio
	eStack->setValue(bpConf->stackSize);

	lPe  = new QLabel (tr("Production Level"), this);
	lMe  = new QLabel (tr("Material Level"), this);
	sbPe = new QSpinBox (this);
	sbMe = new QSpinBox (this);
	sbMe->setMaximum(1000000);
	sbPe->setMaximum(1000000);
	sbMe->setMinimum(-1000000);
	sbPe->setMinimum(-1000000);
	sbMe->setValue(bpConf->me);
	sbPe->setValue(bpConf->pe);

	lName = new QLabel(tr("name"), this);
	eName = new QLineEdit(this);
	eName->setReadOnly(nameRo);
//	eName->setMaxLength(32);
//	eName->setMinimumWidth(200);
	eName->setText(bpConf->name);


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

	layout->addWidget(lName, 0, 7);
	layout->addWidget(eName, 1, 7);
	layout->addWidget(okButton, 6, 7);
	layout->addWidget(cancelButton, 7, 7);


	connect(okButton	, SIGNAL(clicked()), this, SLOT(onOkClick()));
	connect(cancelButton	, SIGNAL(clicked()), this, SLOT(reject()));

	if(templateList)
	{
		conf = new ConfigHandler(QCoreApplication::applicationDirPath()
					+ "/res/templates/"
					+ QDir(QCoreApplication::applicationDirPath() + "/res/templates/", "*.xml").entryList().at(0));
		// take first templatefile in templates/

		templateButton		= new QPushButton(tr("&template"), this);
		templateFileButton	= new QPushButton(tr("&template file"), this);
		templateMenu	= new QMenu(this);
		templateFileMenu= new QMenu(this);

		templateButton->setMinimumWidth(120);
		templateButton->setMenu(templateMenu);

		templateFileButton->setMinimumWidth(120);
		templateFileButton->setMenu(templateFileMenu);

        	templateMenu->clear();
	        foreach(QString s, conf->loadBpList())
	                templateMenu->addAction(s);

		QDir d(QCoreApplication::applicationDirPath() + "/res/templates/", "*.xml");
		foreach(QString file, d.entryList())
			templateFileMenu->addAction(file);

		templateFileButton->setText(QDir(QCoreApplication::applicationDirPath() + "/res/templates/", "*.xml").entryList().at(0));

		layout->addWidget(templateFileButton, 3, 7);
		layout->addWidget(templateButton, 4, 7);

		connect(templateMenu	, SIGNAL(triggered(QAction*)),	this, SLOT(onTemplateMenuAction(QAction*)));
		connect(templateFileMenu, SIGNAL(triggered(QAction*)),	this, SLOT(onTemplateFileMenuAction(QAction*)));
	}

	adjustSize();
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
	bpConf->name      = eName->text();

	for (int i=0;i<8;i++)
		bpConf->baseCnt->insert(i, sbData[i]->value());
	bpConf->baseProdTime  = eDurationd->value() * 86400 + eDurationh->value() * 3600 + eDurationm->value() * 60 + eDurations->value();
	bpConf->stackSize = eStack->value();
	bpConf->me        = sbMe->value();
	bpConf->pe        = sbPe->value();
	accept();
}

void BlueprintInputMask::onTemplateMenuAction(QAction* a)
{
	BpConfig *c = new BpConfig;
	c = conf->loadBlueprint(a->text());
	// conf->loadBp would destroy bpConf pointer

	eName->setText(c->name);

	for (int i=0;i<8;i++)
		sbData[i]->setValue(c->baseCnt->at(i));

	int d, h, m, s;
	d = unsigned( c->baseProdTime / 86400 );
	h = unsigned( c->baseProdTime - d * 86400) / 3600;
	m = unsigned( c->baseProdTime - d * 86400 - h * 3600 ) / 60;
	s = unsigned( c->baseProdTime - d * 86400 - h * 3600 - m * 60 );

	eDurations->setValue(s);
	eDurationm->setValue(m);
	eDurationh->setValue(h);
	eDurationd->setValue(d);

	eStack->setValue(c->stackSize);

	sbMe->setValue(c->me);
	sbPe->setValue(c->pe);
}

void BlueprintInputMask::onTemplateFileMenuAction(QAction* a)
{
	//if(conf!=NULL)delete conf; // cause crash
	delete conf;
	// does new free memory before allocation ??
	conf = new ConfigHandler(QCoreApplication::applicationDirPath() + "/res/templates/" + a->text());

	templateFileButton->setText(a->text());
        templateMenu->clear();
	foreach(QString s, conf->loadBpList())
		templateMenu->addAction(s);
}
