#ifndef PARAMSDLG_H
#define PARAMSDLG_H
#include "ui_ParamsDlg.h"
#include "gpconfig.h"
#include <QDialog>
#include <QGridLayout>
#include <QPair>

class ParamsDlg : public QDialog, public Ui_ParamsDlg
{
Q_OBJECT
public:
	ParamsDlg(QWidget* parent, QList<GameParam*>& params, // celkem humus konstruktor :-/
		  GameAllInfo& info, QStringList& maps, QPair<int,int> minmax);
	~ParamsDlg();
public slots:
	void save();
private:
	QList<GameParam*>& m_params;
	QList<QPair<QWidget*,QWidget*> > m_widgets;
	QGridLayout m_layout;
	GameAllInfo& m_info;

};

#endif

