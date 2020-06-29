#pragma once
#include "Base.h"

BEGIN(Client)
class CStageUIMgr : public CBase
{
	DECLARE_SINGLETON(CStageUIMgr)
public:
	explicit CStageUIMgr();
	virtual ~CStageUIMgr() = default;

private:

};
END
