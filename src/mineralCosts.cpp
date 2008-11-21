
/************************************************************************
 * mineralCosts.cpp							*
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

#include "mineralCosts.h"

MineralCosts::MineralCosts( QVector<double> v, QWidget * parent )
        : Minerals(parent)
{
	setParent(parent);

	sb = new QVector<QDoubleSpinBox*>(8);
	sbData = sb->data();
	costVector = new QVector<double>(8);

	for (int i=0; i<8; i++)
	{
		sbData[i] = new QDoubleSpinBox (this);
		sbData[i]->setMaximum(99999);
		sbData[i]->setSuffix(" ISK");
		sbData[i]->setValue(v.at(i));
		layout->addWidget(sbData[i], i, 2);
		connect(sbData[i], SIGNAL(valueChanged(double)), this, SLOT(onChange()));
	}
	
	adjustSize();
	buf = new QBuffer;
	buf->open(QIODevice::ReadWrite);
	doc = new QDomDocument;
	http = new QHttp(this);
	url = new QUrl("http://eve-central.com/api/marketstat");

	connect(http, SIGNAL(done(bool)), this, SLOT(httpGetDone(bool))); 
}

void MineralCosts::onChange()
{
	for (int i=0; i<8; i++)
		costVector->insert(i, sb->at(i)->value());

	emit costChanged(costVector);
}

void MineralCosts::updatePriceFromWeb()
{
	setCursor(Qt::WaitCursor);
	buf->reset();
	http->setHost(url->host());
	http->get(url->path() + "?typeid=34&typeid=35&typeid=36&typeid=37&typeid=38&typeid=39&typeid=40&typeid=11399", buf);
}

void MineralCosts::setValue(QString typeId, double value)
{
	// map mineralSpinBox to typeId's (I need a switch(QString) :/)
	if(typeId == QString("34"))
		sbData[0]->setValue(value);
	if(typeId == QString("35"))
		sbData[1]->setValue(value);
	if(typeId == QString("36"))
		sbData[2]->setValue(value);
	if(typeId == QString("37"))
		sbData[3]->setValue(value);
	if(typeId == QString("38"))
		sbData[4]->setValue(value);
	if(typeId == QString("39"))
		sbData[5]->setValue(value);
	if(typeId == QString("40"))
		sbData[6]->setValue(value);
	if(typeId == QString("11399"))
		sbData[7]->setValue(value);
}

void MineralCosts::httpGetDone(bool error)
{
	if(error)
	{
		QMessageBox::warning(this, tr("download error"),tr("error while downloading mineral prices.\npage: \"%1\"\n\"%2\"")
								.arg(url->host() + url->path())
								.arg(http->errorString()));
		return;
	}

//puts("----------");
//qDebug() << buf->data();
//puts("----------");

	QString errorStr;
	int errorLine;
	int errorColumn;
        if (!doc->setContent(buf->data(), true, &errorStr, &errorLine, &errorColumn))
	{
		QMessageBox::warning(this, tr("parse error"),
		tr("Parse error in mineral price list\nat line %1, column %2:\n\"%3\"")
		.arg(errorLine)
		.arg(errorColumn)
		.arg(errorStr));
		return;
	}
	
	QDomElement el = doc->documentElement().firstChildElement("marketstat").firstChildElement("type");
	for(int i = 0; i < 8; i++)
	{
		setValue(el.attribute("id"), el.firstChildElement("all").firstChildElement("median").text().toDouble());
		el = el.nextSiblingElement("type");
	}
	setCursor(Qt::ArrowCursor);
}

MineralCosts::~MineralCosts()
{}

