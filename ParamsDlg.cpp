#include "ParamsDlg.h"
#include "palette.h"

ParamsDlg::ParamsDlg(QWidget* parent, QList<GameParam*>& params, GameAllInfo& info,
	QStringList& maps, QPair<int,int> minmax)
	: QDialog(parent), m_params(params), m_info(info)
{
	setupUi(this);
	
	for(int i=0;i<params.size();i++)
	{
		QWidget *left=0, *right=0;
		
		if(params[i])
		{
			params[i]->createWidget(info.vecParamValues[i], &left, &right);
			
			if(left != 0)
				m_layout.addWidget(left, i, 0, 1, 1);
			m_layout.addWidget(right, i, 1, 1, 1);
		}
		
		m_widgets.append(QPair<QWidget*,QWidget*>(left,right));
	}
	
	tabExtended->setLayout(&m_layout);
	okButton->setEnabled(info.bAdmin);
	
	lineName->setText(info.strName);
	linePassword->setText(info.strRoomPassword);
	
	comboMap->addItems(maps);
	comboMap->setCurrentIndex(maps.indexOf(info.strGameMap));
	checkAllowStarted->setCheckState((info.bCanJoinAfterStart) ? Qt::Checked : Qt::Unchecked);
	spinPlayers->setRange(minmax.first, minmax.second);
	spinPlayers->setValue(info.nMaxPlayers);
	
	QString temp = info.strDescr;
	temp.replace("\\r\\n", "\n");
	textDescr->setPlainText(temp);
	
	connect(this, SIGNAL(accepted()), this, SLOT(save()));
	
	applyPalette(this, PaletteMain);
	applyPalette(lineName, PaletteEdit);
	applyPalette(linePassword, PaletteEdit);
	applyPalette(textDescr, PaletteEdit);
	applyPalette(spinPlayers, PaletteEdit);
	applyPalette(comboMap, PaletteEdit);
	applyPalette(checkAllowStarted, PaletteCheck);
}

ParamsDlg::~ParamsDlg()
{
	for(int i=0;i<m_widgets.size();i++)
	{
		delete m_widgets[i].first;
		delete m_widgets[i].second;
	}
}

void ParamsDlg::save()
{
	for(int i=0;i<m_params.size();i++)
	{
		if(m_params[i])
			m_info.vecParamValues[i] = m_params[i]->getWidgetValue(m_widgets[i].second);
	}
	
	m_info.strName = lineName->text();
	m_info.strRoomPassword = linePassword->text();
	m_info.strGameMap = comboMap->currentText();
	m_info.bCanJoinAfterStart = checkAllowStarted->checkState() == Qt::Checked;
	m_info.nMaxPlayers = spinPlayers->value();
	
	QString temp = textDescr->toPlainText();
	temp.replace("\n", "\\r\\n");
	m_info.strDescr = temp;
}

