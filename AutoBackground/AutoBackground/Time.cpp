#include "Time.h"
#include "TimeDefs.h"

#include <ctime>
#include <sstream>

using namespace AG;

Time::Time() 
	: min_(60,0,10)
	, hour_(18,7,1)
	, day_(31,1,1)
	, month_(12,1,1)
	, year_(2100,1990,1)
{
	/* Get current time */
	update();
}

Time * Time::update()
{
	// current date/time based on current system
	time_t now = time(0);
	tm *ltm = new tm;
	localtime_s(ltm, &now);

	// update timeunits
	min_.set(round(ltm->tm_min));
	hour_.set(ltm->tm_hour);
	day_.set(ltm->tm_mday);
	month_.set(ltm->tm_mon+1);
	year_.set(ltm->tm_year + 1900);

	return this;
}

// function to round the number 
int Time::round(int n)
{
	// Return of smaller multiple
	return (n / 10) * 10;
}

std::string Time::to_string()
{
	// https://panodata4.panomax.com/cams/23/2019/11/04/12-20-00_default.jpg

	std::stringstream sstr;
	sstr << year_.to_string() << "/" << month_.to_string() << "/" << day_.to_string() << "/" << hour_.to_string() << "-" << min_.to_string() << "-00";

	return sstr.str();
}

Time& Time::operator++()
{
	if (min_++.get() == min_.getMax())
		if (hour_++.get() == hour_.getMax())
			if (day_++.get() == day_.getMax())
				if (month_++.get() == month_.getMax())
					year_++;
	return *this;
}

Time& Time::operator--()
{
	if (min_--.get() == min_.getMin())
		if (hour_--.get() == hour_.getMin())
			if (day_--.get() == day_.getMin())
				if (month_--.get() == month_.getMin())
					year_--;
	return *this;
}

Time::~Time()
{
}
