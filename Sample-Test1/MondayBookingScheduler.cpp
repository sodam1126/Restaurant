#include "pch.h"
#include "../Restaurant/BookingScheduler.cpp"

class MondayBookingScheduler : public BookingScheduler
{
public:
	MondayBookingScheduler(int capacityPerHour) :
		BookingScheduler(capacityPerHour) {};
	tm getNow() override
	{
		return getTime(2024, 5, 20, 9, 0);
	}

private:
	tm getTime(int year, int mon, int day, int hour, int min)
	{
		tm result = { 0,min,hour,day,mon - 1, year - 1990,1,0,-1 };
		return result;
	}
};