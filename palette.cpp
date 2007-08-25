#include "palette.h"
#include <QSettings>
#include <QPalette>

static QPalette *paletteMain, *paletteCheck, *paletteEdit, *paletteTree, *paletteToolbar;
static bool m_bApply = true;
		
void setApplyPalette(bool bApply)
{
	m_bApply = bApply;
}

void applyPalette(QWidget* pWidget, PaletteType type)
{
	if(!m_bApply)
		return;
	switch(type)
	{
	case PaletteMain:
		pWidget->setPalette(*paletteMain); break;
	case PaletteCheck:
		pWidget->setPalette(*paletteCheck); break;
	case PaletteEdit:
		pWidget->setPalette(*paletteEdit); break;
	case PaletteTree:
		pWidget->setPalette(*paletteTree); break;
	case PaletteToolbar:
		pWidget->setPalette(*paletteToolbar); break;
	}
}

void freePalettes()
{
	delete paletteMain;
	delete paletteCheck;
	delete paletteEdit;
	delete paletteTree;
	delete paletteToolbar;
}

void initPalettes()
{
	paletteMain = new QPalette;
	paletteCheck = new QPalette;
	paletteEdit = new QPalette;
	paletteTree = new QPalette;
	paletteToolbar = new QPalette;
	
	paletteMain->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(0), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(1), QColor(253, 153, 13));
	paletteMain->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteMain->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteMain->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(6), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(8), QColor(0, 0, 0));
	paletteMain->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(9), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(10), QColor(0, 0, 0));
	paletteMain->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(11), QColor(172, 172, 172));
	paletteMain->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(12), QColor(103, 141, 178));
	paletteMain->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteMain->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteMain->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
	paletteMain->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(0), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(1), QColor(253, 153, 13));
	paletteMain->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteMain->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteMain->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(6), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(8), QColor(0, 0, 0));
	paletteMain->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(9), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(10), QColor(0, 0, 0));
	paletteMain->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(11), QColor(172, 172, 172));
	paletteMain->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(12), QColor(103, 141, 178));
	paletteMain->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteMain->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteMain->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
	paletteMain->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(0), QColor(128, 128, 128));
	paletteMain->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(1), QColor(253, 153, 13));
	paletteMain->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteMain->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteMain->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(6), QColor(199, 199, 199));
	paletteMain->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(8), QColor(128, 128, 128));
	paletteMain->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(9), QColor(0, 0, 0));
	paletteMain->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(10), QColor(0, 0, 0));
	paletteMain->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(11), QColor(172, 172, 172));
	paletteMain->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(12), QColor(86, 117, 148));
	paletteMain->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteMain->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteMain->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteMain->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
	
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(0), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(1), QColor(253, 153, 13));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(6), QColor(0, 0, 0));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(8), QColor(0, 0, 0));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(9), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(10), QColor(0, 0, 0));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(11), QColor(172, 172, 172));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(12), QColor(103, 141, 178));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(0), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(1), QColor(253, 153, 13));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(6), QColor(0, 0, 0));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(8), QColor(0, 0, 0));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(9), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(10), QColor(0, 0, 0));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(11), QColor(172, 172, 172));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(12), QColor(103, 141, 178));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(0), QColor(128, 128, 128));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(1), QColor(253, 153, 13));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(6), QColor(199, 199, 199));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(8), QColor(128, 128, 128));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(9), QColor(0, 0, 0));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(10), QColor(0, 0, 0));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(11), QColor(172, 172, 172));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(12), QColor(86, 117, 148));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
	
	paletteEdit->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(0), QColor(255, 255, 255));
	paletteEdit->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(1), QColor(253, 153, 13));
	paletteEdit->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteEdit->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteEdit->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteEdit->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteEdit->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(6), QColor(0, 0, 0));
	paletteEdit->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteEdit->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(8), QColor(0, 0, 0));
	paletteEdit->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(9), QColor(255, 255, 255));
	paletteEdit->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(10), QColor(0, 0, 0));
	paletteEdit->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(11), QColor(172, 172, 172));
	paletteEdit->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(12), QColor(103, 141, 178));
	paletteEdit->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteEdit->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteEdit->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteEdit->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
	paletteEdit->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(0), QColor(255, 255, 255));
	paletteEdit->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(1), QColor(253, 153, 13));
	paletteEdit->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteEdit->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteEdit->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteEdit->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteEdit->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(6), QColor(0, 0, 0));
	paletteEdit->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteEdit->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(8), QColor(0, 0, 0));
	paletteEdit->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(9), QColor(255, 255, 255));
	paletteEdit->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(10), QColor(0, 0, 0));
	paletteEdit->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(11), QColor(172, 172, 172));
	paletteEdit->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(12), QColor(103, 141, 178));
	paletteEdit->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteEdit->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteEdit->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteEdit->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
	paletteEdit->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(0), QColor(128, 128, 128));
	paletteEdit->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(1), QColor(253, 153, 13));
	paletteEdit->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteEdit->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteEdit->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteEdit->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteEdit->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(6), QColor(199, 199, 199));
	paletteEdit->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteEdit->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(8), QColor(128, 128, 128));
	paletteEdit->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(9), QColor(0, 0, 0));
	paletteEdit->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(10), QColor(0, 0, 0));
	paletteEdit->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(11), QColor(172, 172, 172));
	paletteEdit->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(12), QColor(86, 117, 148));
	paletteEdit->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteEdit->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteEdit->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteEdit->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
	
	paletteToolbar->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(0), QColor(255, 204, 49));
	paletteToolbar->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(1), QColor(253, 155, 13));
	paletteToolbar->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteToolbar->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteToolbar->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteToolbar->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteToolbar->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(6), QColor(255, 255, 255));
	paletteToolbar->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteToolbar->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(8), QColor(255, 255, 255));
	paletteToolbar->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(9), QColor(255, 249, 71));
	paletteToolbar->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(10), QColor(255, 186, 65));
	paletteToolbar->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(11), QColor(172, 172, 172));
	paletteToolbar->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(12), QColor(103, 141, 178));
	paletteToolbar->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteToolbar->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteToolbar->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteToolbar->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
	paletteToolbar->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(0), QColor(255, 204, 49));
	paletteToolbar->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(1), QColor(253, 155, 13));
	paletteToolbar->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteToolbar->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteToolbar->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteToolbar->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteToolbar->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(6), QColor(255, 255, 255));
	paletteToolbar->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteToolbar->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(8), QColor(255, 255, 255));
	paletteToolbar->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(9), QColor(255, 249, 71));
	paletteToolbar->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(10), QColor(255, 186, 65));
	paletteToolbar->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(11), QColor(172, 172, 172));
	paletteToolbar->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(12), QColor(103, 141, 178));
	paletteToolbar->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteToolbar->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteToolbar->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteToolbar->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
	paletteToolbar->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(0), QColor(128, 128, 128));
	paletteToolbar->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(1), QColor(253, 155, 13));
	paletteToolbar->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteToolbar->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteToolbar->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteToolbar->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteToolbar->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(6), QColor(199, 199, 199));
	paletteToolbar->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteToolbar->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(8), QColor(128, 128, 128));
	paletteToolbar->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(9), QColor(255, 186, 65));
	paletteToolbar->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(10), QColor(255, 186, 65));
	paletteToolbar->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(11), QColor(172, 172, 172));
	paletteToolbar->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(12), QColor(86, 117, 148));
	paletteToolbar->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteToolbar->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteToolbar->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteToolbar->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
	
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(0), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(1), QColor(253, 153, 13));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(6), QColor(0, 0, 0));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(8), QColor(0, 0, 0));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(9), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(10), QColor(0, 0, 0));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(11), QColor(172, 172, 172));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(12), QColor(103, 141, 178));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteTree->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(0), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(1), QColor(253, 153, 13));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(6), QColor(0, 0, 0));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(8), QColor(0, 0, 0));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(9), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(10), QColor(0, 0, 0));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(11), QColor(172, 172, 172));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(12), QColor(103, 141, 178));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteTree->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(0), QColor(128, 128, 128));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(1), QColor(253, 153, 13));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(6), QColor(199, 199, 199));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(8), QColor(128, 128, 128));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(9), QColor(0, 0, 0));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(10), QColor(0, 0, 0));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(11), QColor(172, 172, 172));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(12), QColor(86, 117, 148));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteTree->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
	
	paletteCheck->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(0), QColor(255, 255, 255));
	paletteCheck->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(1), QColor(253, 153, 13));
	paletteCheck->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteCheck->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteCheck->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteCheck->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteCheck->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(6), QColor(255, 255, 255));
	paletteCheck->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteCheck->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(8), QColor(0, 0, 0));
	paletteCheck->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(9), QColor(253, 153, 13));
	paletteCheck->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(10), QColor(0, 0, 0));
	paletteCheck->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(11), QColor(0, 0, 0));
	paletteCheck->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(12), QColor(103, 141, 178));
	paletteCheck->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteCheck->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteCheck->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteCheck->setColor(QPalette::Active, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
	paletteCheck->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(0), QColor(255, 255, 255));
	paletteCheck->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(1), QColor(253, 153, 13));
	paletteCheck->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteCheck->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteCheck->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteCheck->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteCheck->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(6), QColor(255, 255, 255));
	paletteCheck->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteCheck->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(8), QColor(0, 0, 0));
	paletteCheck->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(9), QColor(253, 153, 13));
	paletteCheck->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(10), QColor(0, 0, 0));
	paletteCheck->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(11), QColor(0, 0, 0));
	paletteCheck->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(12), QColor(103, 141, 178));
	paletteCheck->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteCheck->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteCheck->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteCheck->setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
	paletteCheck->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(0), QColor(128, 128, 128));
	paletteCheck->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(1), QColor(253, 153, 13));
	paletteCheck->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
	paletteCheck->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(3), QColor(255, 255, 255));
	paletteCheck->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(4), QColor(85, 85, 85));
	paletteCheck->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(5), QColor(199, 199, 199));
	paletteCheck->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(6), QColor(199, 199, 199));
	paletteCheck->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
	paletteCheck->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(8), QColor(128, 128, 128));
	paletteCheck->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(9), QColor(0, 0, 0));
	paletteCheck->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(10), QColor(0, 0, 0));
	paletteCheck->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(11), QColor(0, 0, 0));
	paletteCheck->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(12), QColor(86, 117, 148));
	paletteCheck->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
	paletteCheck->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 238));
	paletteCheck->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(15), QColor(82, 24, 139));
	paletteCheck->setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(16), QColor(232, 232, 232));
}

