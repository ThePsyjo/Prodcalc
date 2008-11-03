
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
	doc = new QDomDocument;
}

void MineralCosts::onChange()
{
	for (int i=0; i<8; i++)
		costVector->insert(i, sb->at(i)->value());

	emit costChanged(costVector);
}

void MineralCosts::updatePriceFromWeb()
{
// prices here
// http://eve-central.com/api/marketstat?typeid=34&typeid=35&typeid=36&typeid=37&typeid=38&typeid=39&typeid=40&typeid=11399
	buf->open(QIODevice::ReadWrite);
/*
	QHttp http("www.eve-central.com");
	http.get("/api/marketstat?typeid=34&typeid=35&typeid=36&typeid=37&typeid=38&typeid=39&typeid=40&typeid=11399", buf);
	connect(&http, SIGNAL(done(bool)), this, SLOT(httpGetDone(bool))); 
	http.closeConnection();

*/
puts("webupdate");	
}

void MineralCosts::httpGetDone(bool b)
{
	QString errorStr;
	int errorLine;
	int errorColumn;

        if (!doc->setContent(buf, true, &errorStr, &errorLine, &errorColumn))
	{
		QMessageBox::warning(NULL, tr("parse error"),
		tr("Parse error in mineral price list\nat line %1, column %2:\n\"%3\"")
		.arg(errorLine)
		.arg(errorColumn)
		.arg(errorStr));

		return;
	}
	
	QDomElement el = doc->documentElement().firstChildElement("evec_api").firstChildElement("marketstat");

	printf("%.3f\n", el.firstChildElement("type").firstChildElement("all").firstChildElement("median").text().toDouble());
	buf->close();
}

MineralCosts::~MineralCosts()
{}

