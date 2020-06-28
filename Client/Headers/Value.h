#pragma once
#include "Base.h"

BEGIN(Client)

//BaseValue와 버프를 받아 수정된 value를 구분하기 위한 래퍼클래스.
template<typename T>
class CValue final : public CBase
{
public:
	explicit CValue(T _baseValue) { m_BaseValue = _baseValue; }
	explicit CValue(T _minValue, T _maxValue) { m_MinValue = _minValue; m_BaseValue = _maxValue; m_bRandom = true; }
	virtual ~CValue() = default;

private:
	//기본 값이자 랜덤의 최대값
	T m_BaseValue = 0;
	//추가 값
	T m_Modifier = 0;
	//랜덤 최소 값
	T m_MinValue = 0;
	//랜덤값인가
	_bool m_bRandom = false;

public:
	void AddModifier(T _Modifier) { m_Modifier += _Modifier if(m_bRandom) m_MinValue += _Modifier; }
	void RemoveModifire(T _fModifier) { m_Modifier -= _Modifier if (m_bRandom) m_MinValue -= _Modifier;}
	float GetValue()
	{
		//추가 값들을 더한다.
		T finalValue = m_BaseValue;
		//랜덤 최소값도 함께 처리하자.
		T finalMinValue = m_MinValue;

		finalValue += m_Modifier;
		finalMinValue += m_Modifier;

		//음수가 되는 것은 원하는 바가 아니다.
		if (finalValue < 0)
			finalValue = 0;
		if (finalMinValue < 0)
			finalMinValue = 0;

		//랜덤 값이면
		if (m_bRandom)
		{
			//0으로 나누지 못한다.
			if (finalValue - finalMinValue <= 0)
				return finalValue;

			//범위내 랜덤값을 구한다.
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
	// CBase을(를) 통해 상속됨
	virtual void Free() {};

};

END