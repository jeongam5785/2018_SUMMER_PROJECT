#include <functional>

class Fan
{
public:
	enum Flag
	{
		FLAG_POWER = 0x0001,
		FLAG_SPEED = 0x0002,
		FLAG_SWING = 0x0004,
	};

public:
	Fan(void(*update_callback)(Fan&, Flag));
	~Fan();

public:
	int power() const { return m_power; }
	int speed() const { return m_speed; }
	int swing() const { return m_swing; }

public:
	void set_power(int power);
	void set_speed(int speed);
	void set_swing(int swing);

public:
	void update();
	void update_force();

public:
	void poll();

private:
	void notify(Flag flag);

private:
	//int m_port_num;
	void(*m_update_callback)(Fan&, Flag);

private:
	int m_power;
	int m_speed;
	int m_swing;

private:
	int m_update_flag;
};
