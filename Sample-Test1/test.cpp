#include "pch.h"
#include "../Restaurant/BookingScheduler.cpp"


class BookingItem : public testing::Test
{
protected:
	void SetUp() override
	{
		NOT_ON_THE_HOUR = getTime(2024, 5, 20, 9, 5);
		ON_THE_HOUR = getTime(2024, 5, 20, 9, 0);
	}
public:
	tm getTime(int year,int mon, int day, int hour, int min)
	{
		tm result = { 0,min,hour,day,mon - 1, year - 1990,0,0,-1 };
		mktime(&result);
		return result;
	}
	tm NOT_ON_THE_HOUR;
	tm ON_THE_HOUR;
	Customer CUSTOMER{ "fake name", "010-1234-5678" };
	const int UNDER_CAPACITY = 1;
	const int CAPACITY_PER_HOUR= 3;
	BookingScheduler bookingScheduler{ CAPACITY_PER_HOUR };

};
TEST_F(BookingItem, 예약은_정시에만_가능하다_예외발생) {
	Customer customer{ "fake name", "010-1234-5678" };
	Schedule* schedule = new Schedule{ NOT_ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };
	
	EXPECT_THROW(bookingScheduler.addSchedule(schedule), std::runtime_error);
}

TEST_F(BookingItem, 예약은_정시에만_가능하다_예약성공) {

	Schedule* schedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };
	

	bookingScheduler.addSchedule(schedule);
	EXPECT_EQ(true,bookingScheduler.hasSchedule(schedule));
}