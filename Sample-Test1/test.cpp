#include "pch.h"
#include "../Restaurant/BookingScheduler.cpp"

TEST(Booking, 예약은_정시에만_가능하다_예외발생) {
	tm notOntheHour = { 0 };
	notOntheHour.tm_year = 2024 - 1900;
	notOntheHour.tm_mon = 5 - 1;
	notOntheHour.tm_mday = 20;
	notOntheHour.tm_hour = 9;
	notOntheHour.tm_min = 5;
	mktime(&notOntheHour);

	Customer customer{ "fake name", "010-1234-5678" };
	Schedule* schedule = new Schedule{ notOntheHour, 1, customer };
	BookingScheduler bookingScheduler{ 3 };

	EXPECT_THROW(bookingScheduler.addSchedule(schedule), std::runtime_error);
}

TEST(Booking, 예약은_정시에만_가능하다_예약성공) {
	tm ontheHour = { 0 };
	ontheHour.tm_year = 2024 - 1900;
	ontheHour.tm_mon = 5 - 1;
	ontheHour.tm_mday = 20;
	ontheHour.tm_hour = 9;
	ontheHour.tm_min = 0;
	mktime(&ontheHour);

	Customer customer{ "fake name", "010-1234-5678" };
	Schedule* schedule = new Schedule{ ontheHour, 1, customer };
	BookingScheduler bookingScheduler{ 3 };

	bookingScheduler.addSchedule(schedule);
	EXPECT_EQ(true,bookingScheduler.hasSchedule(schedule));
}