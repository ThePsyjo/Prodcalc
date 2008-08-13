
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

ConfigHandler::ConfigHandler()
{
	doc = new QDomDocument ( "ConfigFile" );
	f = new QFile (QDir::toNativeSeparators(QDir::homePath ()  + "/.prodcalc.xml"));

	f->open( QIODevice::ReadOnly );

	//doc->setContent(f);
	//
	QString errorStr;
	int errorLine;
	int errorColumn;

	if (!doc->setContent(f, true, &errorStr, &errorLine, &errorColumn))
	{
		QMessageBox::information(NULL, "parse error",
						QString("Parse error at line %1, column %2:\n%3")
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

QMessageBox::information(NULL, "finish", QString("all done"));

};

QDomElement ConfigHandler::genTag(QDomElement inMe, QString tagInMe)
{
	QDomElement tag = inMe.firstChildElement(tagInMe);
	if ( tag.tagName() != tagInMe )
	{
		inMe.appendChild(doc->createElement(tagInMe));
		tag = inMe.firstChildElement(tagInMe);
	}
	return tag;
}

void ConfigHandler::start()
{
	QDomElement blueprints = genTag(doc->documentElement(), "Blueprints") ; // tag containing all blueprints

	QDomElement n = blueprints.firstChildElement(); // single blueprint
	QDomElement tag;
	do	//	 V. blueprint 
	{
		printf("try converting \"%s\" ...\n",n.tagName().toAscii().data());

		tag = n.firstChildElement("name");
		if ( tag.tagName() == "name" )
		{
			printf("name...\n");
			n.setAttribute("name", tag.attribute("value", "Blueprint"));
			n.removeChild(tag);
			printf("\tOK\n");
		}

		tag = n.firstChildElement("stackSize");
		if ( tag.tagName() == "stackSize" )
		{
			printf("stackSize...\n");
			n.setAttribute("stackSize", tag.attribute("value", "100"));
			n.removeChild(tag);
			printf("\tOK\n");
		}

		tag = n.firstChildElement("prodTime");
		if ( tag.tagName() == "prodTime" )
		{
			printf("prodTime...\n");
			n.setAttribute("prodTime", tag.attribute("value", "0"));
			n.removeChild(tag);
			printf("\tOK\n");
		}
		else
			printf("\tno convert neded\n");

		n = n.nextSiblingElement(); // next blueprint in "Bleprints"
	}while(! n.isNull());
}
