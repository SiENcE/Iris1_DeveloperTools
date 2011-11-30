#include "StdAfx.h"
#include "TagInfo.h"
#include <stdio.h>

int CTagInfo::ReadTagInfoFromBuf(LPCTSTR source)
{
	char fname[MAX_PATH+1];
	if (!GetTempFileName(".", "~tag", 0, fname)) return -1;
	
	FILE* fp = fopen(fname, "w+");
	if (!fp) return false;
	fwrite(source, 1, strlen(source), fp);
	fclose(fp);

	GenerateTagInfo(fname, "temp.tag");
	ReadTagInfoFromFile("temp.tag");

	DeleteFile(fname);

	return GetSize();;
}

int CTagInfo::FindTagLine(LPCTSTR tag)
{
	for (int i = 0; i < GetSize(); i++) {
		if (GetAt(i).name == tag) return atoi(GetAt(i).line);
	}

	return -1;
}

bool CTagInfo::GenerateTagInfo(LPCTSTR cslfilename, LPCTSTR tagfilename)
{
	CString strCmd;
	strCmd.Format("ctags.exe --c-types=f --language-force=c --sort=yes  -o %s --fields=+n-k %s", 
		tagfilename, cslfilename);
	GfxExecuteExternalFile(strCmd,"",true);;
	return true;
}

int CTagInfo::ReadTagInfoFromFile(LPCTSTR filename)
{
	RemoveAll();

	CString str;
	CString fname1,fname2,file,line;
	
	CStdioFile f;
	if (!f.Open(filename, CFile::modeRead | CFile::typeText))
		return -1;
	
	while (f.ReadString(str) && str.GetLength() > 0) {
		if (str[0] == '!') continue; // comment
					
		// handler_on_decrease_stat	startup.csl	/^handler_on_decrease_stat (const sender)$/;"	line:308
		
		int i = str.Find('\t');
		fname1 = str.Left(i);
		int k = str.Find('\t',++i);
		if (k<0) continue;
		
		file = str.Mid(i,k-i);
		i=k+3;
		k=str.Find("$/;""",i);
		if (k<0) continue;

		fname2 = str.Mid(i,k-i);
		i=k+5;
		k=str.Find("line:",i);
		if (k<0) continue;

		line = str.Right(str.GetLength()-k-5);
		
		// TRACE("%s %s %s %s\n",fname1,fname2,file,line);

		Add(sTagInfo(fname1,fname2,file,line));
		//int iItem = ctrl.AddItem(file,line,fname2);
		//ctrl.SetItemData(iItem, i++);
	}

	return GetSize();
}