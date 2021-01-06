#pragma once
#include <string>

namespace timeutils
{
	std::string now();
	std::string addDaysToDate(std::string date, int days);
}