#pragma once
#include <string>
#include "ITimeUnit.h"

namespace AG {
	class Time
	{
	public:
		Time();

		Time* update();
		std::string to_string();
		int round(int n);

		/* Overloads */
		Time& operator++();
		Time& operator--();

		// postfix
		Time operator--(int)
		{
			Time tmp(*this);
			--(*this);
			return tmp;
		}
		Time operator++(int)
		{
			Time tmp(*this);
			++(*this);
			return tmp;
		}

		~Time();

	private:
		ITimeUnit month_;
		ITimeUnit year_;
		ITimeUnit day_;
		ITimeUnit hour_;
		ITimeUnit min_;

	};
}

