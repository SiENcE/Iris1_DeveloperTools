#pragma once

enum TEXT_ALIGN { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT };

CString GfxAligntoText(TEXT_ALIGN align);
TEXT_ALIGN GfxTexttoAlign(CString strAlign);

int GfxAtoX(CString strNum);
CString GfxXtoA(int iNum);
CString GfxGetFullPath(CString strPath);

void GfxUpdateStatusBar(int iGumpID, CPoint point);

