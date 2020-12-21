#pragma once
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>

namespace timeutils
{
	using namespace std;
	std::string now()
	{
		auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
		return ctime(&timenow);
	}

	std::string addDaysToDate(std::string date, int days)
	{
		std::ostringstream result;
		int d = stoi(date.substr(0, 2));
		int m = stoi(date.substr(3, 2));
		int y = stoi(date.substr(6, 4));

		int m2[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
		int i, k = 0, p, a = 0;
		for (i = 0; i < m; i++)
			k = k + m2[i];
		k = k + d + days;

		if (k <= 365)
		{
			for (i = 0; i < 13; i++)
			{
				p = k - m2[i];
				if (p <= m2[i + 1])
				{
					a = i + 1;
					break;
				}
				else
					k = p;
			}
		}
		else
		{
			k = k - 365;
			for (i = 0; i < 13; i++)
			{
				p = k - m2[i];
				if (p <= m2[i + 1])
				{
					a = i + 1;
					break;
				}
				else
					k = p;
			}
			y++;
		}
		if (p < 10)
			result << "0";
		result << p << "-";
		if (a < 10)
			result << "0";
		result << a << "-" << y;
		return result.str();
	}
}