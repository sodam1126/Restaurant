#include "pch.h"
#include <iostream>
#include "../Restaurant/MailSender.cpp"

using namespace std;

class TestableMailSender : public MailSender
{
public:
	void sendMail(Schedule* schedule) override
	{
		cout << "테스트용 MailSender class의 send 메서드 실행됨" << endl;
		countsendMailMethodIsCalled ++;
	}
	bool getCountSendMailMethodIsCalled()
	{
		return countsendMailMethodIsCalled;
	}

private:
	int countsendMailMethodIsCalled = 0;
};