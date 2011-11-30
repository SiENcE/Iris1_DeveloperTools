#pragma once

#include "cdib.h"

class CShareDib : public CDib
{
	int m_nRefCount;
public:
	CShareDib(void);
	virtual ~CShareDib(void);
	int Delete();
};
