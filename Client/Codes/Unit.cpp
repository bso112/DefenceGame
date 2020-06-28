#include "stdafx.h"
#include "..\Headers\Unit.h"


CUnit::CUnit(PDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CUnit::CUnit(const CUnit & _rhs)
	:CGameObject(_rhs)
{
}

void CUnit::Attack()
{

}

void CUnit::TakeDamage(_int iDamage, _int iInfection)
{

}

void CUnit::OnTakeDamage(_int iDamage, _int iInfection)
{
}

void CUnit::Free()
{
	CGameObject::Free();
}


