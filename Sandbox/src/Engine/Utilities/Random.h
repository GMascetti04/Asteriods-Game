#pragma once
#include "xpch.h"


class Random {
public:
	inline static Random& get() { return s_Instance; }

	//between 0 and 1
	float Float() {
		return (float)m_distribution(m_generator) / (float)m_distribution.max();
	}

	float FloatRange(float min, float max) {
		return fabs(max - min) * Float() + min;
	}

	//both parameters are inclusive
	int IntRange(int min, int max) {
		return (int)(Float() * (fabs(max - min) + 1) + min);
	}

	//returns a random point on the unit circle
	glm::vec2 onUnitCircle()
	{
		float angle = glm::radians(FloatRange(0.0f, 360.0f));
		return { glm::cos(angle), glm::sin(angle) };
	}

private:
	static Random s_Instance;
	Random() {}
	~Random() {}

	//might change this to have a faster engine generator
	std::random_device m_generator;
	std::uniform_int_distribution<std::random_device::result_type> m_distribution;

};