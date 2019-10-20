/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>
#include <base/Date.h>
#include <base/Literal.h>

using namespace com::azure::dev::base;

class CalendarApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  static const unsigned int FIELD_WIDTH = 4;
public:
  
  CalendarApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application(
        "calendar",
        numberOfArguments,
        arguments,
        environment) {
  }
  
  inline int getSucceedingDayOfWeek(int day) throw() {
    int temp = day + 1;
    return (temp > Date::SATURDAY) ? Date::SUNDAY : temp;
  }
  
  void main() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2003-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;
    
    Date now = Date::getNow();
    unsigned int month = now.getMonth();
    Date::DateTime decomposed;
    now.split(decomposed, true);
    
    // TAG: use locale
    static const Literal MONTH_NAMES[] = {
      Literal("January"),
      Literal("February"),
      Literal("March"),
      Literal("April"),
      Literal("May"),
      Literal("June"),
      Literal("July"),
      Literal("August"),
      Literal("September"),
      Literal("October"),
      Literal("November"),
      Literal("December")
    };

    static const Literal DAY_NAMES[] = {
      Literal("Sunday"),
      Literal("Monday"),
      Literal("Tuesday"),
      Literal("Wednesday"),
      Literal("Thursday"),
      Literal("Friday"),
      Literal("Saturday")
    };
    
    static const Literal DAY_NAMES_MEDIUM[] = {
      Literal("Sun"),
      Literal("Mon"),
      Literal("Tue"),
      Literal("Wed"),
      Literal("Thu"),
      Literal("Fri"),
      Literal("Sat")
    };
    
    static const Literal DAY_NAMES_SHORT[] = {
      Literal("Su"),
      Literal("M"),
      Literal("Tu"),
      Literal("W"),
      Literal("Th"),
      Literal("F"),
      Literal("Sa")
    };

    const int firstDayOfWeekOfLocale = Date::MONDAY; // TAG: locale specific
    
    {
      StringOutputStream stream;
      stream << MONTH_NAMES[month] << SP
             << decomposed.year << FLUSH;
      String header = stream.getString();
      int headerIndent =
        (6 + FIELD_WIDTH * Date::DAYS_PER_WEEK - header.getLength())/2;
      fout << indent(maximum(0, headerIndent)) << header << ENDL;
      int dayOfWeek = firstDayOfWeekOfLocale;
      fout << LEFT << setWidth(6) << "week";
      for (
        int day = 0;
        day < Date::DAYS_PER_WEEK;
        ++day, dayOfWeek = getSucceedingDayOfWeek(dayOfWeek)
      ) {
        fout << RIGHT << setWidth(FIELD_WIDTH)
             << DAY_NAMES_MEDIUM[dayOfWeek];
      }
      fout << ENDL;
    }

    int firstWeek = Date::getWeek(0, decomposed.month, decomposed.year);
    
    int firstDayOfWeek =
      Date::getDayOfWeek(0, decomposed.month, decomposed.year);
    
    fout << LEFT << setWidth(6) << firstWeek;
    int dayOfWeek = firstDayOfWeekOfLocale;
    while (dayOfWeek != firstDayOfWeek) {
      dayOfWeek = getSucceedingDayOfWeek(dayOfWeek);
      fout << indent(FIELD_WIDTH);
    }
    
    const unsigned int daysOfMonth =
      Date::getDaysOfMonth(decomposed.month, decomposed.year);

    // TAG: 3 to 6 weeks per month
    for (int day = 0; day < static_cast<int>(daysOfMonth);) {
      fout << RIGHT << setWidth(FIELD_WIDTH) << (day + 1);
      dayOfWeek = getSucceedingDayOfWeek(dayOfWeek);
      ++day;
      if (dayOfWeek == firstDayOfWeekOfLocale) {
        fout << EOL;
        fout << LEFT << setWidth(6)
             << Date::getWeek(day, decomposed.month, decomposed.year);
      }
    }
    fout << ENDL;
  }
};

APPLICATION_STUB(CalendarApplication);
