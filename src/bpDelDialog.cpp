
/************************************************************************
 * bpDelDialog.cpp							*
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

#include "bpDelDialog.h"

DelDialog::DelDialog(QString name, QString text, QWidget* parent)
{
	setParent(parent);
	setWindowFlags(Qt::Dialog);
	setWindowTitle(name);
	setModal(1);

	layout = new QGridLayout;
	label = new QLabel (text, this);
	acceptButton = new QPushButton (tr("&yes"), this);
	rejectButton = new QPushButton (tr("&no"), this);

	rejectButton->setFocus();

	connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(rejectButton, SIGNAL(clicked()), this, SLOT(reject()));

	layout->addWidget(label, 0, 0, 1, 0);
	layout->addWidget(acceptButton, 1, 0);
	layout->addWidget(rejectButton, 1, 1);

	setLayout(layout);
}

DelDialog::~DelDialog()
{};
