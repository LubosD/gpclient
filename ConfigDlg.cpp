#include "ConfigDlg.h"
#include "gpconfig.h"

#include <QLabel>
#include "palette.h"

ConfigDlg::ConfigDlg(QWidget* parent)
	: QDialog(parent)
{
	setupUi(this);
	load();
	
	connect(this, SIGNAL(accepted()), this, SLOT(save()));
	applyPalette(this, PaletteMain);
	//applyPalette(checkColouredNicks, PaletteCheck);
	applyPalette(checkUserInform, PaletteCheck);
	applyPalette(checkGPColors, PaletteCheck);
	applyPalette(checkSmileys, PaletteCheck);
	applyPalette(spinRefreshInterval, PaletteCheck);
}

ConfigDlg::~ConfigDlg()
{
	delete [] m_pValues;
	delete [] m_pLabels;
}

void ConfigDlg::save()
{
	QList<GameConfig>& config = getGameConfig();
	
	//settings.setValue("general/colouredNicks", checkColouredNicks->checkState() == Qt::Checked);
	settings.setValue("general/userInform", checkUserInform->checkState() == Qt::Checked);
	settings.setValue("general/gpColors", checkGPColors->checkState() == Qt::Checked);
	settings.setValue("general/smileys", checkSmileys->checkState() == Qt::Checked);
	settings.setValue("general/refreshInterval", spinRefreshInterval->value());
	setApplyPalette(checkGPColors->checkState() == Qt::Checked);
	
	for(int i=0;i<config.size();i++)
	{
		settings.setValue(QString("games/")+config[i].id, m_pValues[i].text());
	}
}

void ConfigDlg::load()
{
	QList<GameConfig>& config = getGameConfig();
	
	m_pValues = new QLineEdit[config.size()];
	m_pLabels = new QLabel[config.size()];
	
	//checkColouredNicks->setCheckState( (settings.value("general/colouredNicks", true).toBool()) ? Qt::Checked : Qt::Unchecked);
	checkUserInform->setCheckState( (settings.value("general/userInform", true).toBool()) ? Qt::Checked : Qt::Unchecked);
	checkGPColors->setCheckState( (settings.value("general/gpColors", true).toBool()) ? Qt::Checked : Qt::Unchecked);
	checkSmileys->setCheckState( (settings.value("general/smileys", true).toBool()) ? Qt::Checked : Qt::Unchecked);
	
	for(int i=0;i<config.size();i++)
	{
		applyPalette(&m_pValues[i], PaletteEdit);
		applyPalette(&m_pLabels[i], PaletteMain);
		
		m_pLabels[i].setText( config[i].name );
		m_pValues[i].setText( settings.value( QString("games/")+config[i].id, config[i].command ).toString() );
		
		gridLayout2->addWidget(&m_pLabels[i], i, 0);
		gridLayout2->addWidget(&m_pValues[i], i, 1);
	}
	
	spinRefreshInterval->setValue( settings.value("general/refreshInterval", 10).toInt() );
}

