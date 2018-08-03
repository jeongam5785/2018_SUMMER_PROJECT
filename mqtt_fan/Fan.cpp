#include "Fan.h"

Fan::Fan(void(*update_callback)(Fan&, Flag))
	: m_update_callback(update_callback)
	, m_power(0)
	, m_speed(0)
	, m_swing(0)
	, m_update_flag(0)
{
}

Fan::~Fan()
{

}

void Fan::set_power(int power)
{
	m_power = power;
	notify(FLAG_POWER);
}

void Fan::set_speed(int speed)
{
	m_speed = speed;
	notify(FLAG_SPEED);
}

void Fan::set_swing(int swing)
{
	m_swing = swing;
	notify(FLAG_SWING);
}

void Fan::notify(Flag flag)
{
	m_update_flag |= flag;
}

void Fan::update()
{
	if (m_update_callback)
		m_update_callback(*this, (Flag)m_update_flag);

	m_update_flag = 0;
}

void Fan::update_force()
{
	m_update_callback(*this, (Flag)0xFFFFFFFF);
}

void Fan::poll()
{
	if (m_update_flag != 0)
	{
		update();
	}
}
