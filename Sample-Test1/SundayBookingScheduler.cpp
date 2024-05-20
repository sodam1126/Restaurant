#include "pch.h"
#include "../Restaurant/BookingScheduler.cpp"

class SundayBookingScheduler : public BookingScheduler
{
public:
	SundayBookingScheduler(int capacityPerHour) :
		BookingScheduler(capacityPerHour) {};
	tm getNow() override
	{
		return getTime(2024, 5, 19, 9, 0);
	}

private:
	tm getTime(int year, int mon, int day, int hour, int min)
	{
		tm result = { 0,min,hour,day,mon - 1, year - 1990,0,0,-1 };
		return result;
	}
};