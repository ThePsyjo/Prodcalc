
/************************************************************************
 * skill.h								*
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

#ifndef SKILL_H
#define SKILL_H

//#include <QWidget>
#include <QGroupBox>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>

//class Skill : public QWidget
class Skill : public QGroupBox
{
Q_OBJECT
public:
	Skill(int, int, QWidget*);
	virtual ~Skill();
private:
	QLabel *lInd, *lPe;
	QSpinBox *sbInd, *sbPe;
	QVBoxLayout *layout;
private slots:
	void onIndChange(int);
	void onPeChange(int);
signals:
	void indSkillChanged(int);
	void peSkillChanged(int);
};

#endif

