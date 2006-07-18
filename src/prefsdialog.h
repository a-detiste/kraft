/***************************************************************************
                   prefsdialog.h  - the preferences Dialog
                             -------------------
    begin                : Sun Jul 3 2004
    copyright            : (C) 2004 by Klaas Freitag
    email                : freitag@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
#ifndef PREFSDIALOG_H
#define PREFSDIALOG_H

#include <kdialogbase.h>

/**
 *  @author Klaas Freitag
 */
class PrefsDialog : public KDialogBase
{
public:
    PrefsDialog(QWidget *parent);

    ~PrefsDialog();

protected:
     void readConfig();
     void writeConfig();

protected slots:
     void slotOk();

private:
     QLineEdit *m_leHost;
     QLineEdit *m_leUser;
     QLineEdit *m_leName;
     QLineEdit *m_lePasswd;
};

#endif
