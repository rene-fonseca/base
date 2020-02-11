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
#include <base/string/ANSIEscapeSequence.h>
#include <base/Date.h>
#include <base/Literal.h>

using namespace com::azure::dev::base;

class CalendarApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  static const unsigned int FIELD_WIDTH = 4;
public:
  
  CalendarApplication()
    : Application("calendar")
  {
  }
  
  inline int getSucceedingDayOfWeek(int day)
  {
    int temp = day + 1;
    return (temp > Date::SATURDAY) ? Date::SUNDAY : temp;
  }
  
  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;
    
    const Date now = Date::getNow();
    const Date::DateTime decomposedNow = now.getLocalTime().split();
    Date::DateTime decomposed = now.getLocalTime().split();
    unsigned int month = decomposed.month;
    
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
      stream << MONTH_NAMES[month] << SP << decomposed.year << FLUSH;
      String header = stream.getString();
      int headerIndent =
        (6 + FIELD_WIDTH * Date::DAYS_PER_WEEK - header.getLength())/2;
      
      fout << indent(maximum(0, headerIndent));
      if (fout.isANSITerminal()) {
        fout << bold() << header << normal() << ENDL;
      } else {
        fout << header << ENDL;
      }

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

    Date::DateTime firstDay = Date::getDate(1, decomposed.month, decomposed.year).split();
    int firstWeek = Date::getWeek(firstDay);
    int firstDayOfWeek = firstDay.weekday;
    
    fout << LEFT << setWidth(6) << firstWeek;
    int dayOfWeek = firstDayOfWeekOfLocale;
    while (dayOfWeek != firstDayOfWeek) {
      dayOfWeek = getSucceedingDayOfWeek(dayOfWeek);
      fout << indent(FIELD_WIDTH);
    }
    
    const unsigned int daysOfMonth = Date::getDaysOfMonth(decomposed.month, decomposed.year);
    int week = firstWeek;
    for (int day = 1; day <= static_cast<int>(daysOfMonth); ++day) {
      if (fout.isANSITerminal()) {
        bool today = (decomposedNow.day == day) &&
          (decomposedNow.month == decomposed.month) &&
        (decomposedNow.year == decomposed.year);
        if (today) {
          fout << bold();
        }
        fout << RIGHT << setForeground(today ? ANSIEscapeSequence::GREEN : ANSIEscapeSequence::BLUE)
             << setWidth(FIELD_WIDTH) << day << normal();
      } else {
        fout << RIGHT << setWidth(FIELD_WIDTH) << day;
      }
      dayOfWeek = getSucceedingDayOfWeek(dayOfWeek);
      if (dayOfWeek == firstDayOfWeekOfLocale) {
        ++week;
        fout << EOL << LEFT << setWidth(6) << week;
      }
    }
    fout << ENDL;
  }
};

APPLICATION_STUB(CalendarApplication);
