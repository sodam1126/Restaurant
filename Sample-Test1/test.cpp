#include "pch.h"
#include "../Restaurant/BookingScheduler.cpp"
#include "TestableSmsSender.cpp"
#include "TestableMailSender.cpp"

class BookingItem : public testing::Test
{
protected:
	void SetUp() override
	{
		NOT_ON_THE_HOUR = getTime(2024, 5, 20, 9, 5);
		ON_THE_HOUR = getTime(2024, 5, 20, 9, 0);
		bookingScheduler.setSmsSender(&testableSmsSender);
		bookingScheduler.setMailSender(&testableMailSender);
	}
public:
	tm getTime(int year,int mon, int day, int hour, int min)
	{
		tm result = { 0,min,hour,day,mon - 1, year - 1990,0,0,-1 };
		mktime(&result);
		return result;
	}
	tm plusHour(tm base, int hour)
	{
		base.tm_hour += hour;
		mktime(&base);
		return base;
	}
	tm NOT_ON_THE_HOUR;
	tm ON_THE_HOUR;
	Customer CUSTOMER{ "fake name", "010-1234-5678" };
	Customer CUSTOMER_WITH_MAIL{ "fake name", "010-1234-5678","test@test.com" };
	const int UNDER_CAPACITY = 1;
	const int CAPACITY_PER_HOUR= 3;
	BookingScheduler bookingScheduler{ CAPACITY_PER_HOUR };
	TestableSmsSender testableSmsSender;
	TestableMailSender testableMailSender;
};


TEST_F(BookingItem, 예약은_정시에만_가능하다_예외발생) {
	Schedule* schedule = new Schedule{ NOT_ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };
	
	EXPECT_THROW(bookingScheduler.addSchedule(schedule), std::runtime_error);
}

TEST_F(BookingItem, 예약은_정시에만_가능하다_예약성공) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };

	bookingScheduler.addSchedule(schedule);
	EXPECT_EQ(true,bookingScheduler.hasSchedule(schedule));
}

TEST_F(BookingItem, 시간대별_인원제한_인원초과) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER };

	bookingScheduler.addSchedule(schedule);
	try
	{
		Schedule* newSchedule = new Schedule{ ON_THE_HOUR,UNDER_CAPACITY,CUSTOMER };
		bookingScheduler.addSchedule(newSchedule);
		FAIL();
	}
	catch(std::runtime_error&e)
	{
		EXPECT_EQ(string{ e.what() }, string{ "Number of people is over restaurant capacity per hour" });
	}
	
}

TEST_F(BookingItem, 시간대별_인원제한_다른시간) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER };

	bookingScheduler.addSchedule(schedule);

	tm diffHour = plusHour(ON_THE_HOUR, 2);

	Schedule* newSchedule = new Schedule{ diffHour,UNDER_CAPACITY,CUSTOMER };
	bookingScheduler.addSchedule(newSchedule);

	EXPECT_EQ(true, bookingScheduler.hasSchedule(schedule));
	EXPECT_EQ(true, bookingScheduler.hasSchedule(newSchedule));
}

TEST_F(BookingItem, 예약완료시_SMS는_무조건_발송) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER };

	bookingScheduler.addSchedule(schedule);

	EXPECT_EQ(true, testableSmsSender.isSendMethodIsCalled());
}

TEST_F(BookingItem, 이메일이_없는_경우_메일_미발송) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER };

	bookingScheduler.addSchedule(schedule);

	EXPECT_EQ(0, testableMailSender.getCountSendMailMethodIsCalled());
}

TEST_F(BookingItem, 이메일이_있는_경우_메일_발송) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER_WITH_MAIL };

	bookingScheduler.addSchedule(schedule);

	EXPECT_EQ(1, testableMailSender.getCountSendMailMethodIsCalled());
}