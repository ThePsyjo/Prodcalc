
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

QDomElement ConfigHandler::genTag(QDomElement inMe, QString tagInMe)
{
	QDomElement tag = inMe.firstChildElement(tagInMe);
	if ( tag.tagName() != tagInMe )
	{
		inMe.appendChild(doc->createElement(tagInMe));
		tag = inMe.firstChildElement(tagInMe);
		tagCreated = true;
	}
	else tagCreated = false;
	return tag;
}

void ConfigHandler::saveMineralPrice(QVector<double>* v)
{
	QDomElement tag = genTag ( doc->documentElement(), "MineralPrice" );
	tag.setAttribute("Tritanium",	v->at(0));		
	tag.setAttribute("Pyerite",	v->at(1));		
	tag.setAttribute("Mexallon",	v->at(2));		
	tag.setAttribute("Isogen",	v->at(3));		
	tag.setAttribute("Nocxium",	v->at(4));		
	tag.setAttribute("Zydrine",	v->at(5));		
	tag.setAttribute("Megacyte",	v->at(6));		
	tag.setAttribute("Morphite",	v->at(7));		
}

QVector<double> ConfigHandler::loadMineralPrice()
{
	QVector<double> v;

	QDomElement tag = genTag ( doc->documentElement(), "MineralPrice" );

	v.append( tag.attribute("Tritanium",	"2.50"	).toDouble());
	v.append( tag.attribute("Pyerite",	"5"	).toDouble());
	v.append( tag.attribute("Mexallon",	"25"	).toDouble());
	v.append( tag.attribute("Isogen",	"65"	).toDouble());
	v.append( tag.attribute("Nocxium",	"120"	).toDouble());
	v.append( tag.attribute("Zydrine",	"2500"	).toDouble());
	v.append( tag.attribute("Megacyte",	"4500"	).toDouble());
	v.append( tag.attribute("Morphite",	"18000"	).toDouble());

	return v;
}

void ConfigHandler::saveBuyTax(double d)
{
	genTag ( doc->documentElement(), "buyTax" ).setAttribute("value", d);		
}

double ConfigHandler::loadBuyTax()
{
	return genTag ( doc->documentElement(), "buyTax" ).attribute("value", "0").toDouble();
}

void ConfigHandler::saveSellTax(double d)
{
	 genTag ( doc->documentElement(), "sellTax" ).setAttribute("value", d);		
}

double ConfigHandler::loadSellTax()
{
	return genTag ( doc->documentElement(), "sellTax" ).attribute("value", "0").toDouble();
}

///////////////////////  save/load Broker Fee //////////////////////
void ConfigHandler::saveBrokerTax(double d)
{
	genTag ( doc->documentElement(), "brokerTax" ).setAttribute("value", d);
}

double ConfigHandler::loadBrokerTax()
{
	return genTag ( doc->documentElement(), "brokerTax" ).attribute("value", "0").toDouble();
}

///////////////////////  save/load Install //////////////////////
void ConfigHandler::saveInstallFee(double d)
{
	genTag ( doc->documentElement(), "installFee" ).setAttribute("value", d);		
}

double ConfigHandler::loadInstallFee()
{
	return genTag ( doc->documentElement(), "installFee" ).attribute("value", "0").toDouble();
}

///////////////////////  save/load Rental //////////////////////
void ConfigHandler::saveRental(double d)
{
	genTag ( doc->documentElement(), "rental" ).setAttribute("value", d);		
}

double ConfigHandler::loadRental()
{
	return genTag ( doc->documentElement(), "rental" ).attribute("value", "0").toDouble();
}

void ConfigHandler::saveBlueprint(BpConfig* c)
{
	bpConf = c;
	QString bpTagName = bpConf->name;
	bpTagName.replace(' ', '_');

	QDomElement BlueprintTag = genTag( genTag(doc->documentElement(), "Blueprints"), bpTagName );
	if (tagCreated) emit bpListChanged(loadBpList()); // a new blueprint was added

	QDomElement 
	tag = genTag ( BlueprintTag, "name" );
	tag.setAttribute("value", bpConf->name);

	tag = genTag( BlueprintTag, "prodTime" );
	tag.setAttribute("value", bpConf->prodTime);

	tag = genTag( BlueprintTag, "stackSize" );
	tag.setAttribute("value", bpConf->stackSize);

	tag = genTag( BlueprintTag, "minerals" );
	tag.setAttribute("Tritanium",	bpConf->cnt->at(0));		
	tag.setAttribute("Pyerite",	bpConf->cnt->at(1));		
	tag.setAttribute("Mexallon",	bpConf->cnt->at(2));		
	tag.setAttribute("Isogen",	bpConf->cnt->at(3));		
	tag.setAttribute("Nocxium",	bpConf->cnt->at(4));		
	tag.setAttribute("Zydrine",	bpConf->cnt->at(5));		
	tag.setAttribute("Megacyte",	bpConf->cnt->at(6));		
	tag.setAttribute("Morphite",	bpConf->cnt->at(7));
}

BpConfig* ConfigHandler::loadBlueprint(QString name)
{
	bpConf->name = name;

	QDomElement BlueprintTag = genTag( genTag(doc->documentElement(), "Blueprints"), name.replace(' ', '_') ); // tag containing all blueprints

	QDomElement 
	tag = genTag ( BlueprintTag, "minerals" );

	bpConf->cnt->insert( 0, tag.attribute("Tritanium",	"0").toInt());
	bpConf->cnt->insert( 1, tag.attribute("Pyerite",	"0").toInt());
	bpConf->cnt->insert( 2, tag.attribute("Mexallon",	"0").toInt());
	bpConf->cnt->insert( 3, tag.attribute("Isogen",		"0").toInt());
	bpConf->cnt->insert( 4, tag.attribute("Nocxium",	"0").toInt());
	bpConf->cnt->insert( 5, tag.attribute("Zydrine",	"0").toInt());
	bpConf->cnt->insert( 6, tag.attribute("Megacyte",	"0").toInt());
	bpConf->cnt->insert( 7, tag.attribute("Morphite",	"0").toInt());

	bpConf->stackSize = genTag ( BlueprintTag, "stackSize" ).attribute("value", "100").toInt();
	bpConf->prodTime =  genTag ( BlueprintTag, "prodTime"  ).attribute("value", "0").toInt();

	return bpConf;
}

QVector<QString> ConfigHandler::loadBpList()
{
	QDomElement Blueprints = genTag(doc->documentElement(), "Blueprints") ; // tag containing all blueprints

	QVector<QString> v;
	QDomNode n = Blueprints.firstChild(); // a saved blueprint
	do	//	  . V nameTag of blueprint  .  V value of name
	{	v.append(n.firstChildElement("name").attribute("value", "Blueprint"));
		n = n.nextSibling();
	}while(! n.isNull());

	qSort(v.begin(), v.end());
	return v;
}

void ConfigHandler::delBlueprint(QString s)
{
        QDomElement bpTag = genTag(doc->documentElement(), "Blueprints");
        QDomElement bp    = bpTag.firstChildElement(s.replace(' ', '_'));

	bpTag.removeChild(bp);

	emit bpListChanged(loadBpList()); // a blueprint was removed
}

QString ConfigHandler::loadStyleSheet()
{
	return genTag(doc->documentElement(), "userStyle").text();
}

