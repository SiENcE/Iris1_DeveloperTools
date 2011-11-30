#include "StdAfx.h"
#include ".\gfx.h"
#include "MainFrm.h"
#include "GumpEditorDoc.h"
#include <direct.h>

static CString strAligns[] = { "left", "center", "right" };

CString GfxAligntoText(TEXT_ALIGN align)
{
	if (align > ALIGN_RIGHT) align = ALIGN_LEFT;
	
	return strAligns[align];
}

TEXT_ALIGN GfxTexttoAlign(CString align)
{
	for (int i=0; i < sizeof(strAligns)/sizeof(strAligns[0]); i++)
		if (align.CompareNoCase(strAligns[i])==0) return (TEXT_ALIGN)i;

	return ALIGN_LEFT;
}


int GfxAtoX(CString strNum)
{
	strNum.Trim();
	int iNum=atoi(strNum);
	if (strNum.Left(2).CompareNoCase("0x")==0)
		sscanf(strNum, "%x", &iNum);
	return iNum;
}

CString GfxXtoA(int iNum)
{
	CString strNum;
	strNum.Format("0x%X", iNum);
	return strNum;
}

CString GfxGetFullPath(CString strPath)
{
	strPath.Replace('/', '\\');
	
	if (strPath.Find(":\\") < 0)
	{
		char dir[_MAX_PATH]={0,};
		_getcwd(dir, sizeof(dir));

		if (strPath.Left(1) != "\\")
			strPath = "\\" + strPath;

		strPath = CString(dir) + strPath;
	}

	return strPath;
}



