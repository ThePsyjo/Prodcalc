
/************************************************************************
 * configuration.cpp							*
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

#include "configuration.h"
#include <QMessageBox>

ConfigHandler::ConfigHandler()
{
	doc = new QDomDocument ( "ConfigFile" );
	f = new QFile (QDir::toNativeSeparators(QDir::homePath ()  + "/.prodcalc.xml"));
	bpConf = new BpConfig;

	f->open( QIODevice::ReadOnly );

	//doc->setContent(f);
	//
	QString errorStr;
	int errorLine;
	int errorColumn;

	if (!doc->setContent(f, true, &errorStr, &errorLine, &errorColumn))
	{
		QMessageBox::information(NULL, tr("parse error"),
						tr("Parse error at line %1, column %2:\n%3")
						.arg(errorLine)
						.arg(errorColumn)
						.arg(errorStr));
	}



	f->close();

	if ( ! (doc->documentElement().tagName() == "ProdCalcConfig") )
	{
#ifdef DEBUG
		puts("create new root");
#endif
		QDomElement root = doc->createElement("ProdCalcConfig");
		doc->appendChild(root);
	}
};

ConfigHandler::~ConfigHandler()
{
#ifdef DEBUG
	puts("save");
#endif
	f->open( QIODevice::WriteOnly );
	QTextStream stream( f );
	doc->save(stream,3);
	f->close();
};

void ConfigHandler::saveMineralPrice(QVector<double>* v)
{
	QDomElement root  = doc->documentElement();
	QDomElement ore = root.firstChildElement ( "MineralPrice" ); 
	if ( ore.tagName() == "MineralPrice" )
	{
		ore.setAttribute("Tritanium",	v->at(0));		
		ore.setAttribute("Pyerite",	v->at(1));		
		ore.setAttribute("Mexallon",	v->at(2));		
		ore.setAttribute("Isogen",	v->at(3));		
		ore.setAttribute("Nocxium",	v->at(4));		
		ore.setAttribute("Zydrine",	v->at(5));		
		ore.setAttribute("Megacyte",	v->at(6));		
		ore.setAttribute("Morphite",	v->at(7));		
	}
	else
	{
		QDomElement ore = doc->createElement("MineralPrice");
		root.appendChild(ore);
		saveMineralPrice(v);
	}
}

QVector<double> ConfigHandler::loadMineralPrice()
{
	QVector<double> v;

	QDomElement root = doc->documentElement();
	QDomElement ore  = root.firstChildElement ( "MineralPrice" );

	v.append( ore.attribute("Tritanium",	"2.50"	).toDouble());
	v.append( ore.attribute("Pyerite",	"5"	).toDouble());
	v.append( ore.attribute("Mexallon",	"25"	).toDouble());
	v.append( ore.attribute("Isogen",	"65"	).toDouble());
	v.append( ore.attribute("Nocxium",	"120"	).toDouble());
	v.append( ore.attribute("Zydrine",	"2500"	).toDouble());
	v.append( ore.attribute("Megacyte",	"4500"	).toDouble());
	v.append( ore.attribute("Morphite",	"18000"	).toDouble());

	return v;
}

void ConfigHandler::saveBuyTax(double d)
{
	QDomElement root  = doc->documentElement();
	QDomElement buyTaxTag = root.firstChildElement ( "buyTax" ); 
	if ( buyTaxTag.tagName() == "buyTax" )
		buyTaxTag.setAttribute("value", d);		
	else
	{
		QDomElement buyTaxTag = doc->createElement("buyTax");
		root.appendChild(buyTaxTag);
		saveBuyTax(d);
	}
}

double ConfigHandler::loadBuyTax()
{
	QDomElement root  = doc->documentElement();
	QDomElement buyTaxTag = root.firstChildElement ( "buyTax" ); 

	return buyTaxTag.attribute("value", "0").toDouble();
}

void ConfigHandler::saveSellTax(double d)
{
	QDomElement root  = doc->documentElement();
	QDomElement sellTaxTag = root.firstChildElement ( "sellTax" ); 
	if ( sellTaxTag.tagName() == "sellTax" )
		sellTaxTag.setAttribute("value", d);		
	else
	{
		QDomElement sellTaxTag = doc->createElement("sellTax");
		root.appendChild(sellTaxTag);
		saveSellTax(d);
	}
}

double ConfigHandler::loadSellTax()
{
	QDomElement root  = doc->documentElement();
	QDomElement sellTaxTag = root.firstChildElement ( "sellTax" ); 

	return sellTaxTag.attribute("value", "0").toDouble();
}

void ConfigHandler::saveBrokerTax(double d)
{
	QDomElement root  = doc->documentElement();
	QDomElement brokerTaxTag = root.firstChildElement ( "brokerTax" ); 
	if ( brokerTaxTag.tagName() == "brokerTax" )
		brokerTaxTag.setAttribute("value", d);		
	else
	{
		QDomElement brokerTaxTag = doc->createElement("brokerTax");
		root.appendChild(brokerTaxTag);
		saveBrokerTax(d);
	}
}
///////////////////////  save/load Install //////////////////////
double ConfigHandler::loadInstallFee()
{
	QDomElement root  = doc->documentElement();
	QDomElement installFeeTag = root.firstChildElement ( "installFee" ); 

	return installFeeTag.attribute("value", "0").toDouble();
}

void ConfigHandler::saveInstallFee(double d)
{
	QDomElement root  = doc->documentElement();
	QDomElement installFeeTag = root.firstChildElement ( "installFee" ); 
	if ( installFeeTag.tagName() == "installFee" )
		installFeeTag.setAttribute("value", d);		
	else
	{
		QDomElement installFeeTag = doc->createElement("installFee");
		root.appendChild(installFeeTag);
		saveBrokerTax(d);
	}
}

///////////////////////  save/load Rental //////////////////////
double ConfigHandler::loadRental()
{
	QDomElement root  = doc->documentElement();
	QDomElement rentalTag = root.firstChildElement ( "rental" ); 

	return rentalTag.attribute("value", "0").toDouble();
}

void ConfigHandler::saveRental(double d)
{
	QDomElement root  = doc->documentElement();
	QDomElement rentalTag = root.firstChildElement ( "rental" ); 
	if ( rentalTag.tagName() == "rental" )
		rentalTag.setAttribute("value", d);		
	else
	{
		QDomElement rentalTag = doc->createElement("rental");
		root.appendChild(rentalTag);
		saveBrokerTax(d);
	}
}

///////////////////////  save/load Broker Fee //////////////////////
double ConfigHandler::loadBrokerTax()
{
	QDomElement root  = doc->documentElement();
	QDomElement brokerTaxTag = root.firstChildElement ( "brokerTax" ); 

	return brokerTaxTag.attribute("value", "0").toDouble();
}

void ConfigHandler::saveBlueprint(BpConfig* c)
{
	bpConf = c;
	QDomElement root  = doc->documentElement();
	QDomElement bpTag = root.firstChildElement ( "Blueprints" ); 
	QString bpTagName = bpConf->name;
	QString name = bpConf->name;
	bpTagName.replace(' ', '_');

	if ( bpTag.tagName() == "Blueprints" ) // <Blueprints>
	{
		QDomElement bp = bpTag.firstChildElement ( bpTagName );

		if ( bp.tagName() == bpTagName ) // <BP Name>
		{
			QDomElement content = bp.firstChildElement("name");

			if ( content.tagName() == "name" ) // <stackSize>
				content.setAttribute("value", name);
			else
			{
				content = doc->createElement("name");
				bp.appendChild(content);
				saveBlueprint(c);
			}

			content = bp.firstChildElement("prodTime");

			if ( content.tagName() == "prodTime" ) // <prodTime>
				content.setAttribute("value", bpConf->prodTime);
			else
			{
				content = doc->createElement("prodTime");
				bp.appendChild(content);
				saveBlueprint(c);
			}

			content = bp.firstChildElement("stackSize");

			if ( content.tagName() == "stackSize" ) // <stackSize>
				content.setAttribute("value", bpConf->stackSize);
			else
			{
				content = doc->createElement("stackSize");
				bp.appendChild(content);
				saveBlueprint(c);
			}

			content = bp.firstChildElement("minerals");

			if ( content.tagName() == "minerals" ) // <minerals>
			{
				content.setAttribute("Tritanium",	bpConf->cnt->at(0));		
				content.setAttribute("Pyerite",		bpConf->cnt->at(1));		
				content.setAttribute("Mexallon",	bpConf->cnt->at(2));		
				content.setAttribute("Isogen",		bpConf->cnt->at(3));		
				content.setAttribute("Nocxium",		bpConf->cnt->at(4));		
				content.setAttribute("Zydrine",		bpConf->cnt->at(5));		
				content.setAttribute("Megacyte",	bpConf->cnt->at(6));		
				content.setAttribute("Morphite",	bpConf->cnt->at(7));
			}
			else
			{
				content = doc->createElement("minerals");
				bp.appendChild(content);
				saveBlueprint(c);
			}

		}
		else
		{
			QDomElement tag = doc->createElement(bpTagName);
			bpTag.appendChild(tag);
			saveBlueprint(c);
			emit bpListChanged(loadBpList()); // a new blueprint was added
		}
	}
	else
	{
		QDomElement bpTag = doc->createElement("Blueprints");
		root.appendChild(bpTag);
		saveBlueprint(c);
	}
}

BpConfig* ConfigHandler::loadBlueprint(QString name)
{
	bpConf->name = name;
	QString bpTagName = name;
	bpTagName.replace(' ', '_');

	QDomElement root  = doc->documentElement();
	QDomElement bpTag = root.firstChildElement ( "Blueprints" ); 
	QDomElement bp = bpTag.firstChildElement ( bpTagName );
	QDomElement content = bp.firstChildElement ( "minerals" );



	bpConf->cnt->insert( 0, content.attribute("Tritanium",	"0").toInt());
	bpConf->cnt->insert( 1, content.attribute("Pyerite",	"0").toInt());
	bpConf->cnt->insert( 2, content.attribute("Mexallon",	"0").toInt());
	bpConf->cnt->insert( 3, content.attribute("Isogen",	"0").toInt());
	bpConf->cnt->insert( 4, content.attribute("Nocxium",	"0").toInt());
	bpConf->cnt->insert( 5, content.attribute("Zydrine",	"0").toInt());
	bpConf->cnt->insert( 6, content.attribute("Megacyte",	"0").toInt());
	bpConf->cnt->insert( 7, content.attribute("Morphite",	"0").toInt());

	content = bp.firstChildElement ( "stackSize" );
	bpConf->stackSize = content.attribute("value", "100").toInt();

	content = bp.firstChildElement ( "prodTime" );
	bpConf->prodTime = content.attribute("value", "0").toInt();

	return bpConf;
}

QVector<QString> ConfigHandler::loadBpList()
{
        QDomElement root  = doc->documentElement();
        QDomElement bpTag = root.firstChildElement ( "Blueprints" );
	QDomElement name;

	QVector<QString> v;
	QDomNode n = bpTag.firstChild();
	do
	{	
		v.append(n.firstChildElement("name").attribute("value", "Blueprint"));
		n = n.nextSibling();
	}while(! n.isNull());

	qSort(v.begin(), v.end());
	return v;
}

void ConfigHandler::delBlueprint(QString s)
{
        QDomElement root  = doc->documentElement();
        QDomElement bpTag = root.firstChildElement ( "Blueprints" );
        QDomElement bp    = bpTag.firstChildElement(s.replace(' ', '_'));

	bpTag.removeChild(bp);
	emit bpListChanged(loadBpList()); // a blueprint was removed
}

QString ConfigHandler::loadStyleSheet()
{
	QDomElement root  = doc->documentElement();
	QDomElement styleTag = root.firstChildElement ( "userStyle" );

	return styleTag.text();
}

