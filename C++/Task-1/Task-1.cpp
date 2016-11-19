#include <assert.h>
#include <string>
#include <exception>

const int kDaysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const std::string kWeekDaysNames[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
const int kDaysInYear = 365;
const int kDaysInWeek = 7;
const int kMonthsInYear = 12;

struct Date
{
    int day_;
    int month_;
    int year_;
    Date(int day, int month, int year)
    {
        day_ = day;
        month_ = month;
        year_ = year;
    }
};

const Date kSynchronizationDate = Date(9, 9, 2014);
const int kSynchronizationDateIndex = 3;

bool IsLeapYear(int year)
{
    return ((year % 400) == 0) || ((year % 4) == 0 && (year % 100) != 0);
}

int GetNumberDaysInYear(int year)
{
    return IsLeapYear(year) ? kDaysInYear + 1 : kDaysInYear;
}

int GetNumberDaysInMonth(int month, int year)
{
    return (IsLeapYear(year) && month == 1) ? kDaysInMonth[month] + 1 : kDaysInMonth[month];
}

int GetDaysNumberFromZeroYear(const Date &date)
{
    int days_number = date.day_;
    for (int month = 0; month < date.month_ ; ++month)
    {
        days_number += GetNumberDaysInMonth(month, date.year_);
    } 
    int years_number = date.year_ - 1;
    days_number += years_number * kDaysInYear + years_number / 400 + years_number / 4 - years_number / 1000;
    return days_number;
}

int GetDaysNumberBetweenDates(const Date &first_date, const Date &second_date)
{
    return GetDaysNumberFromZeroYear(second_date) - GetDaysNumberFromZeroYear(first_date);
}

bool IsValidDate(const Date &date)
{
    if (date.day_ > 0 &&
        date.month_ >= 0 &&
        date.month_ < kMonthsInYear && 
        date.day_ <= GetNumberDaysInMonth(date.month_, date.year_))
    {
        return true;
    }
    return false;
}

const std::string &GetDayOfWeek(const Date &date)
{   
    if (!IsValidDate(date))
    {
        throw std::exception("Incorrect date");
    }
    int last_days_number =  GetDaysNumberBetweenDates(kSynchronizationDate, date);
    int week_day_name_index = (kDaysInWeek + kSynchronizationDateIndex + last_days_number % kDaysInWeek) % kDaysInWeek;
    return kWeekDaysNames[week_day_name_index];
}

void LeapYearTest()
{
    assert(IsLeapYear(2000));
    assert(IsLeapYear(2008));
    assert(!IsLeapYear(1900));
    assert(!IsLeapYear(2014));
}

void GetDayOfWeekTest()
{
    Date date_before_synchronization_date = Date(19, 0, 1993);
    assert(GetDayOfWeek(date_before_synchronization_date) == kWeekDaysNames[1]);

    Date date_after_synchronization_date = Date(29, 1, 2044);
    assert(GetDayOfWeek(date_after_synchronization_date) == kWeekDaysNames[0]);
}

int main()
{
    LeapYearTest();
    GetDayOfWeekTest();
    return 0;
}