#ifndef PALETTE_H
#define PALETTE_H
#include <QWidget>

enum PaletteType { PaletteMain, PaletteCheck, PaletteEdit, PaletteTree, PaletteToolbar };
void initPalettes();
void freePalettes();
void setApplyPalette(bool bApply);
void applyPalette(QWidget* pWidget, PaletteType type);

#endif
