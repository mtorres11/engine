//----------------------------------------------------------------------------------
// CJump class
// Author: Enric Vergara
//
// Description:
// Esta clase se encarga de gestionar la física del salto del Player.
//
//		a(t) = cte = g
//		v(t) = g*t + v0
//		y(t) = g*t^2 + v0*t + y0
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_JUMP_H_
#define INC_JUMP_H_

class CJump
{

public:
	CJump(): m_bJump(false), m_fV0(0.0f), m_fJumpTime(0.0f), m_fG(-4.9f), m_fDefaultGravity(-9.8f){}
	~CJump() {/*Nothing*/}

	void	StartJump	(float v0, float h0);
	void	StopJump	();
	float	GetHeight	(float elapsedTime);
  bool  IsOnJump  (); // retorna si actualmente está saltando o no

private:

	bool	m_bJump;
	float	m_fV0;
	float m_fH0;
  float	m_fJumpTime;
	float	m_fG;
	float	m_fDefaultGravity;
};

#endif // INC_JUMP_H_