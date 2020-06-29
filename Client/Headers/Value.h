#pragma once
#include "Base.h"

BEGIN(Client)

//BaseValue�� ������ �޾� ������ value�� �����ϱ� ���� ����Ŭ����.
template<typename T>
class CValue final : public CBase
{
public:
	explicit CValue(T _baseValue) { m_BaseValue = _baseValue; }
	explicit CValue(T _minValue, T _maxValue) { m_MinValue = _minValue; m_BaseValue = _maxValue; m_bRandom = true; }
	virtual ~CValue() = default;

private:
	//�⺻ ������ ������ �ִ밪
	T m_BaseValue = 0;
	//�߰� ��
	T m_Modifier = 0;
	//���� �ּ� ��
	T m_MinValue = 0;
	//�������ΰ�
	_bool m_bRandom = false;

public:
	void AddModifier(T _Modifier) { m_Modifier += _Modifier if(m_bRandom) m_MinValue += _Modifier; }
	void RemoveModifire(T _fModifier) { m_Modifier -= _Modifier if (m_bRandom) m_MinValue -= _Modifier;}
	float GetValue()
	{
		//�߰� ������ ���Ѵ�.
		T finalValue = m_BaseValue;
		//���� �ּҰ��� �Բ� ó������.
		T finalMinValue = m_MinValue;

		finalValue += m_Modifier;
		finalMinValue += m_Modifier;

		//������ �Ǵ� ���� ���ϴ� �ٰ� �ƴϴ�.
		if (finalValue < 0)
			finalValue = 0;
		if (finalMinValue < 0)
			finalMinValue = 0;

		//���� ���̸�
		if (m_bRandom)
		{
			//0���� ������ ���Ѵ�.
			if (finalValue - finalMinValue <= 0)
				return finalValue;

			//������ �������� ���Ѵ�.
			finalValue = T(rand() % (int)(finalValue - finalMinValue));
			finalValue += finalMinValue;
		}

		return finalValue;
	};
	float GetMaxValue()
	{
		return m_BaseValue + m_Modifier;
	}
	float GetMinValue()
	{
		return m_MinValue + m_Modifier;
	}


public:
	// CBase��(��) ���� ��ӵ�
	virtual void Free() {};

};

END