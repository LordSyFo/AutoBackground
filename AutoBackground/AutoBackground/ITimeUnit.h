#pragma once
#include <iostream>

class ITimeUnit
{
public:
	ITimeUnit(unsigned int max, unsigned int min = 0, unsigned int step = 1, unsigned int initval = 0)
		: max_(max)
		, min_(min)
		, value_(initval>=min&&initval<=max ? initval : min)
		, step_(step){};

	void setMax() { value_ = max_; }
	void setMin() { value_ = min_; }
	unsigned int getMax() { return max_; }
	unsigned int getMin() { return min_; }

	unsigned int get() { return value_; }
	void set(unsigned int val) { val <= max_ && val >= min_ ? value_ = val : value_ = value_; }

	std::string to_string()
	{
		std::string valueStr = std::to_string(value_);
		return valueStr.length() > 1 ? valueStr : valueStr.insert(0, "0");
	}

	// prefix
	ITimeUnit& operator--()
	{
		value_ = ((value_ + max_ - step_) % max_);
		value_ = value_ < min_ ? max_ : value_;		//TODO: Find a smarter and faster way to implement this
		return *this;
	}
	ITimeUnit& operator++()
	{
		value_ = ((value_ + step_) % max_);
		return *this;
	}

	// postfix
	ITimeUnit operator--(int) 
	{
		ITimeUnit tmp(*this);
		--(*this);
		return tmp;
	}
	ITimeUnit operator++(int)
	{
		ITimeUnit tmp(*this);
		++(*this);
		return tmp;
	}



	~ITimeUnit(){};
private:
	unsigned int value_;
	unsigned int max_;
	unsigned int min_;
	unsigned int step_;
	//maybe implement loop counter for counting how many times unit has looped
};

inline bool operator==(ITimeUnit& lhs, ITimeUnit& rhs) { return lhs.get() == rhs.get(); }
inline bool operator==(ITimeUnit& lhs, unsigned int rhs) { return lhs.get() == rhs; }
inline bool operator!=(ITimeUnit& lhs, ITimeUnit& rhs) { return lhs.get() != rhs.get(); }
inline bool operator< (ITimeUnit& lhs, ITimeUnit& rhs) { return lhs.get() < rhs.get();; }
inline bool operator> (ITimeUnit& lhs, ITimeUnit& rhs) { return lhs.get() > rhs.get();; }
inline bool operator<=(ITimeUnit& lhs, ITimeUnit& rhs) { return lhs.get() <= rhs.get();; }
inline bool operator>=(ITimeUnit& lhs, ITimeUnit& rhs) { return lhs.get() >= rhs.get();; }