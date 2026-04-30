#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>


class Timer
{
public:
	Timer() = default;

	~Timer() = default;

	void restart()
	{
		pass_time = 0;
		shotted = false;
	}

	void set_wait_time(double val)
	{
		wait_time = val;
	}

	void set_one_shot(bool flg)
	{
		one_shot = false;
	}

	void set_on_timeout(std::function<void()> on_timeout)
	{
		this->on_timeout = on_timeout;
	}

	void pause()
	{
		paused = true;
	}

	void resume()
	{
		paused = false; 
	}

	void on_update(double delta)
	{
		if (paused) return;
	
		pass_time += delta;
		if (pass_time >= wait_time)
		{
			bool can_shot = (!one_shot || (one_shot && !shotted));
			shotted = true;
			if (can_shot && on_timeout)
				on_timeout();

			pass_time -= wait_time;
		}
	
	}


private:
	double pass_time;
	double wait_time;
	bool paused = false;
	bool shotted = false;
	bool one_shot = false;
	std::function<void()> on_timeout;

};




#endif // !_TIMER_H_

