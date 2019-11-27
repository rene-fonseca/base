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
  
  DateApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[])
    : Application("Date", numberOfArguments, arguments, environment)
  {
  }
  
  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;

    Date date = Date::getNow();
    
    Date::DateTime dateTime = {
      4000, // year
      13, // month
      31, // day of month
      123456789, // day of week - dont care
      123456789, // day of year - dont care
      123456789, // week of year - dont care
      24, // hour
      61, // minute
      61, // second
      999 // millisecond
    };
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
         << "Date::getUTCSecond(): " << date.getUTCSecond() << EOL
         << "Date::getUTCMinute(): " << date.getUTCMinute() << EOL
         << "Date::getUTCHour(): " << date.getUTCHour() << EOL
         << "Date::getUTCDay(): " << date.getUTCDay() << EOL
         << "Date::getUTCDayOfWeek(): " << date.getUTCDayOfWeek() << EOL
         << "Date::getUTCDayOfYear(): " << date.getUTCDayOfYear() << EOL
         << "Date::getUTCMonth(): " << date.getUTCMonth() << EOL
         << "Date::getUTCYear(): " << date.getUTCYear() << EOL
         << "Date::getJulianDay(): " << date.getJulianDay() << EOL
         << "Date::isLeapYear(1999): " << Date::isLeapYear(1999) << EOL
         << "Date::isLeapYear(2000): " << Date::isLeapYear(2000) << EOL
         << "Date::Date(DateTime): " << Date(dateTime) << EOL
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

    // Sunday
    fout << "Date::getDayOfWeek(2-1, Date::AUGUST, 1953): "
         << DAY_NAMES[Date::getDayOfWeek(2-1, Date::AUGUST, 1953)] << ENDL;

    // Thursday
    fout << "Date::getDayOfWeek(13-1, Date::FEBRUARY, 2053): "
         << DAY_NAMES[Date::getDayOfWeek(13-1, Date::FEBRUARY, 2053)] << ENDL;

    // Saturday
    fout << "Date::getDayOfWeek(20-1, Date::NOVEMBER, 2055): "
         << DAY_NAMES[Date::getDayOfWeek(20-1, Date::NOVEMBER, 2055)] << ENDL;
  }

  ~DateApplication() {
  }
};

APPLICATION_STUB(DateApplication);
