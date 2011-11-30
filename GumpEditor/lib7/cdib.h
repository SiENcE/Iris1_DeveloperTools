// cdib.h declaration for Inside Visual C++ CDib class

#ifndef _INSIDE_VISUAL_CPP_CDIB
#define _INSIDE_VISUAL_CPP_CDIB

// packed 24bit dib pixel data, RGB() 매크로와 b/r 자리가 다르다.
#define QUADRGB(r,g,b) ((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)))

//##ModelId=3BDB8C2D0133
class AFX_EXT_CLASS CDib : public CObject
{
public:
	enum Alloc {noAlloc, crtAlloc, heapAlloc};
	DECLARE_SERIAL(CDib)

	//##ModelId=3BDB8C2D01B5
	LPVOID m_lpvColorTable;
	//##ModelId=3BDB8C2D01B4
	HBITMAP m_hBitmap;
	//##ModelId=3BDB8C2D01AA
	LPBYTE m_lpImage;  // starting address of DIB bits
	//##ModelId=3BDB8C2D01A2
	LPBITMAPINFOHEADER m_lpBMIH; //  buffer containing the BITMAPINFOHEADER

//protected:
	//##ModelId=3BDB8C2D0196
	HGLOBAL m_hGlobal; // For external windows we need to free;
	                   //  could be allocated by this class or allocated externally
	//##ModelId=3BDB8C2D018E
	Alloc m_nBmihAlloc;
	//##ModelId=3BDB8C2D0184
	Alloc m_nImageAlloc;
	//##ModelId=3BDB8C2D0150
	DWORD m_dwSizeImage; // of bits -- not BITMAPINFOHEADER or BITMAPFILEHEADER
	//##ModelId=3BDB8C2D0149
	int m_nColorTableEntries;
	
	//##ModelId=3BDB8C2D0148
	HANDLE m_hFile;
	//##ModelId=3BDB8C2D0147
	HANDLE m_hMap;
	//##ModelId=3BDB8C2D0146
	LPVOID m_lpvFile;
	//##ModelId=3BDB8C2D013E
	HPALETTE m_hPalette;

	//##ModelId=3BDB8C2D013D
	BOOL m_bStretchBlt;

public:
	//##ModelId=3BDB8C2D0329
	CDib();
	//##ModelId=3BDB8C2D0331
	CDib(CSize size, int nBitCount);	// builds BITMAPINFOHEADER
	//##ModelId=3BDB8C2D0328
	~CDib();
	//##ModelId=3BDB8C2D0327
	int GetSizeImage() {return m_dwSizeImage;}
	//##ModelId=3BDB8C2D031F
	int GetSizeHeader()
		{return sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;}
	//##ModelId=3BDB8C2D031E
	CSize GetDimensions();
	//##ModelId=3BDB8C2D0313
	BOOL AttachMapFile(const char* strPathname, BOOL bShare = FALSE);
	//##ModelId=3BDB8C2D030A
	BOOL CopyToMapFile(const char* strPathname);
	//##ModelId=3BDB8C2D02E1
	BOOL AttachMemory(LPVOID lpvMem, BOOL bMustDelete = FALSE, HGLOBAL hGlobal = NULL);
	// until we implemnt CreateDibSection
	//##ModelId=3BDB8C2D02C5
	BOOL Draw(CDC* pDC, CPoint ptDst, CSize sizeDst = CSize(-1,-1), 
		CPoint ptSrc = CPoint(0,0), CSize sizeSrc = CSize(-1,-1));
	//##ModelId=3BDB8C2D02C3
	HBITMAP CreateSection(CDC* pDC = NULL);
	//##ModelId=3BDB8C2D02B9
	UINT UsePalette(CDC* pDC, BOOL bBackground = FALSE);
	//##ModelId=3BDB8C2D02B1
	BOOL MakePalette();
	//##ModelId=3BDB8C2D02AF
	BOOL SetSystemPalette(CDC* pDC);
	//##ModelId=3BDB8C2D029D
	BOOL Compress(CDC* pDC, BOOL bCompress = TRUE); // FALSE means decompress
	//##ModelId=3BDB8C2D029B
	HBITMAP CreateBitmap(CDC* pDC);
	//##ModelId=3BDB8C2D0287
	BOOL Read(CFile* pFile);
	//##ModelId=3BDB8C2D0273
	BOOL ReadSection(CFile* pFile, CDC* pDC = NULL);
	//##ModelId=3BDB8C2D0260
	BOOL Write(CFile* pFile);
	//##ModelId=3BDB8C2D0257
	void Serialize(CArchive& ar);
	//##ModelId=3BDB8C2D0256
	void Empty();
	//##ModelId=3BDB8C2D024D
	void SetStretchBlt(BOOL bStretchBlt) { m_bStretchBlt = bStretchBlt; }
	
private:
	//##ModelId=3BDB8C2D024C
	void DetachMapFile();
	//##ModelId=3BDB8C2D0242
	void ComputePaletteSize(int nBitCount);
	//##ModelId=3BDB8C2D0241
	void ComputeMetrics();

public:

	//
	// 추가 - HK
	//

	// pDib 을 복사
	//##ModelId=3BDB8C2D0238
	BOOL Copy(CDib *pDib);

	//##ModelId=3BDB8C2D0289
	BOOL Read(LPCTSTR lpszPath);
	// resource 에서 읽는다.
	//##ModelId=3BDB8C2D0292
	BOOL Read(UINT nResourceID);

	//##ModelId=3BDB8C2D0269
	BOOL Write(LPCTSTR lpszFile);

	// BitCount 반환
	//##ModelId=3BDB8C2D0237
	int GetBitCount() const
	{
		if (m_lpBMIH == NULL) return 0;
		return m_lpBMIH->biBitCount;
	}

	// 이미지 너비
	//##ModelId=3BDB8C2D022E
	int GetWidth() const
	{
		if (m_lpBMIH == NULL) return 0;
		return m_lpBMIH->biWidth;
	}

	// 이미지 높이
	//##ModelId=3BDB8C2D022D
	int GetHeight() const
	{
		if (m_lpBMIH == NULL) return 0;
		return m_lpBMIH->biHeight;
	}

	// 한 줄이 차지하는 크기
	//##ModelId=3BDB8C2D0224
	int GetStorageWidth() const;

	// image(x,y) 의 주소, 8 bit 이상 dib 만 가능
	//##ModelId=3BDB8C2D021A
	LPBYTE GetPixelAddress (int x, int y);
	// 8 bit 이하만
	//##ModelId=3BDB8C2D0211
	int GetPaletteIndex(COLORREF c);

	// image(x,y) 의 값, 모든 dib 가능
	//##ModelId=3BDB8C2D0206
	RGBQUAD GetPixel(int x, int y) const;

	// 특정 위치(x,y) 에 비트맵(pDib)을 복사, pColorKey 는 투명키
	//##ModelId=3BDB8C2D01F1
	BOOL Paste(CDib* pDib, int x, int y, LPBYTE pColorKey = NULL);
	//##ModelId=3BDB8C2D01DF
	BOOL ChangePixel(LPBYTE pOld, LPBYTE pNew);

	//##ModelId=3BDB8C2D01D3
	HBITMAP CreateBitMaskBitmap(CDC* pDC, COLORREF ColorKey);
	//##ModelId=3BDB8C2D01C8
	BOOL DrawTransparent(CDC* pDC, CPoint ptDst, COLORREF ColorKey, 
		CSize sizeDst = CSize(-1,-1),  CPoint ptSrc = CPoint(0,0), CSize sizeSrc = CSize(-1,-1));

#ifdef _DEBUG
	//##ModelId=3BDB8C2D01BE
	virtual void Dump( CDumpContext& dc ) const;
#endif
};

// image(x,y) 의 주소, 8 bit 이상 dib 만 가능
inline LPBYTE CDib::GetPixelAddress (int x, int y)
{
	if (!m_lpBMIH || m_lpBMIH->biBitCount < 8 || !m_lpImage)
		return NULL;
	// image 는 뒤집어서 저장되어 있다.
	return m_lpImage + (GetHeight()-y-1) * GetStorageWidth() + x;
}

// 한 줄이 차지하는 크기
inline int CDib::GetStorageWidth() const
{
	if (m_lpBMIH == NULL || m_lpImage == NULL)
		return 0;
	
	// 한 줄은 4 bytes 단위로 저장된다.
	DWORD dwBytes = ((DWORD) m_lpBMIH->biWidth * m_lpBMIH->biBitCount) / 32;
	if(((DWORD) m_lpBMIH->biWidth * m_lpBMIH->biBitCount) % 32) {
		dwBytes++;
	}
	dwBytes *= 4;

	return dwBytes;
}

// image(x,y) 의 값, 모든 dib 가능
inline RGBQUAD CDib::GetPixel(int x, int y) const
{
	RGBQUAD rgbq = { 0,};
	BYTE &r = rgbq.rgbRed, &g = rgbq.rgbGreen, &b = rgbq.rgbBlue;

	if (m_lpBMIH == NULL || m_lpImage == NULL)
		return rgbq;
	
	BYTE index;
	LPBYTE lpLine = m_lpImage + (GetHeight()-y-1) * GetStorageWidth();
	LPRGBQUAD lpColorTable = (LPRGBQUAD)m_lpvColorTable;

	switch (m_lpBMIH->biBitCount)
	{
	case 1 :
		index = *(lpLine + x / 8);
		index = index >> (7 - x % 8) & 0x01;
		rgbq = *(lpColorTable + index);
		break;
	case 4 :
		// BYTE (low|high)
		index = *(lpLine + x / 2);
		if (x % 2) index &= 0x0F;
		else index >>= 4;
		rgbq = *(lpColorTable + index);
		break;
	case 8 : 
		rgbq = *(lpColorTable + *(lpLine + x));
		break;
	case 16 :
		// 테스트 필요 - 99/02/28
		b = *lpLine >> 3;
		g = ((*lpLine & 0x07) << 2) + (*(lpLine + 1) >> 6);
		r = (*(lpLine + 1) >> 1) & 0x1F;
		break;
	case 24 :
		b = *(lpLine + x * 3 + 0);
		g = *(lpLine + x * 3 + 1);
		r = *(lpLine + x * 3 + 2);
		break;
	case 32 :
		// 테스트 필요 - 99/02/28
		b = *(lpLine + x * 4 + 0);
		g = *(lpLine + x * 4 + 1);
		r = *(lpLine + x * 4 + 2);
		break;
	default :
		ASSERT(FALSE);
	}

	return rgbq;
}

#endif // _INSIDE_VISUAL_CPP_CDIB
