/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/Date.h>
#include <base/string/FormatOutputStream.h>
#include <base/Literal.h>

using namespace com::azure::dev::base;

class DateApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  
  Array<String> hosts;
public:
  
  DateApplication()
    : Application("Date")
  {
  }
  
  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;

    Date date = Date::getNow();
    
    Date::DateTime dateTime;
    dateTime.year = 4000;
    dateTime.month = Date::DECEMBER;
    dateTime.day = 31;
    dateTime.hour = 24;
    dateTime.minute = 1;
    dateTime.second = 1;
    dateTime.millisecond = 999;
    Date::normalize(dateTime);
    
//     fout << dateTime.year << EOL
//          << dateTime.month << EOL
//          << dateTime.day << EOL
//          << dateTime.weekday << EOL
//          << dateTime.dayOfYear << EOL
//          << dateTime.week << EOL
//          << dateTime.hour << EOL
//          << dateTime.minute << EOL
//          << dateTime.second << EOL
//          << dateTime.millisecond << EOL
//          << ENDL;
    
    fout << "Date::getNow(): " << date << EOL
         << "Date::getBias(): " << Date::getBias() << EOL
         << "Date::getValue(): " << date.getValue() << EOL
         << "Date::getSecond(): " << date.getSecond() << EOL
         << "Date::getMinute(): " << date.getMinute() << EOL
         << "Date::getHour(): " << date.getHour() << EOL
         << "Date::getDay(): " << date.getDay() << EOL
         << "Date::getDayOfWeek(): " << date.getDayOfWeek() << EOL
         << "Date::getDayOfYear(): " << date.getDayOfYear() << EOL
         << "Date::getMonth(): " << date.getMonth() << EOL
         << "Date::getYear(): " << date.getYear() << EOL
         << "Date::getJulianDay(): " << date.getJulianDay() << EOL
         << "Date::isLeapYear(1999): " << Date::isLeapYear(1999) << EOL
         << "Date::isLeapYear(2000): " << Date::isLeapYear(2000) << EOL
         << "Date::Date(DateTime): " << Date::makeDate(dateTime) << EOL
         << ENDL;

    static const Literal DAY_NAMES[] = {
      Literal("Sunday"),
      Literal("Monday"),
      Literal("Tuesday"),
      Literal("Wednesday"),
      Literal("Thursday"),
      Literal("Friday"),
      Literal("Saturday")
    };
  }

  ~DateApplication()
  {
  }
};

APPLICATION_STUB(DateApplication);
