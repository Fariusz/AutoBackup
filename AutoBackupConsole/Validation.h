#pragma once
#include <string>
#include <stdexcept>

namespace validation {

	using std::string;

	bool validateDate(string date)
	{
		if (date.length() != 10)
			return false;
		if (!(date[2] == '-' && date[5] == '-'))
			return false;

		int iDay, iMonth, iYear;
		try
		{
			string sDay = date.substr(0, 2);
			string sMonth = date.substr(3, 5);
			string sYear = date.substr(6, 10);

			iDay = stoi(sDay);
			iMonth = stoi(sMonth);
			iYear = stoi(sYear);
		}
		catch (std::exception e)
		{
			return false;
		}

		if (!(1582 <= iYear))
			return false;
		if (!(1 <= iMonth && iMonth <= 12))
			return false;
		if (!(1 <= iDay && iDay <= 31))
			return false;
		if ((iDay == 31) && (iMonth == 2 || iMonth == 4 || iMonth == 6 || iMonth == 9 || iMonth == 11))
			return false;
		if ((iDay == 30) && (iMonth == 2))
			return false;
		if ((iMonth == 2) && (iDay == 29) && (iYear % 4 != 0))
			return false;
		if ((iMonth == 2) && (iDay == 29) && (iYear % 400 == 0))
			return true;
		if ((iMonth == 2) && (iDay == 29) && (iYear % 100 == 0))
			return false;
		if ((iMonth == 2) && (iDay == 29) && (iYear % 4 == 0))
			return true;

		return true;
	}

	bool validateTime(std::string time)
	{
		if (time.length() != 8)
			return false;
		if (!(time[2] == ':' && time[5] == ':'))
			return false;

		int iHour, iMin, iSec;
		try
		{
			string sHour = time.substr(0, 2);
			string sMin = time.substr(3, 5);
			string sSec = time.substr(6, 8);

			iHour = stoi(sHour);
			iMin = stoi(sMin);
			iSec = stoi(sSec);
		}
		catch (std::exception e)
		{
			return false;
		}

		if (iHour < 0 || iHour > 23)
			return false;
		if (iMin < 0 || iMin > 59)
			return false;
		if (iSec < 0 || iSec > 59)
			return false;

		return true;
	}

	bool validateInterval(std::string interval)
	{

		if (interval.length() != 8)
			return false;
		if (!(interval[2] == ' '
			&& interval[5] == ':'
			))
			return false;

		return true;
	}
}