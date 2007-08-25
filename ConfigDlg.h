
#ifndef CONFIGDLG_H
#define CONFIGDLG_H
#include "ui_ConfigDlg.h"
#include <QDialog>
#include <QSettings>
#include <QLabel>
#include <QLineEdit>


class ConfigDlg : public QDialog, public Ui_ConfigDlg
{
Q_OBJECT
public:
	ConfigDlg(QWidget* parent);
	~ConfigDlg();
public slots:
	void save();
private:
	void load();
	QSettings settings;
	QLineEdit* m_pValues;
	QLabel* m_pLabels;
};

#endif
