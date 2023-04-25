#include <fstream>
#include <iostream>
#include <string>

#include <date/date.h>

#include "internship.h"


namespace internship {

    // releaseDate and eol must be dates in format YYYY-mm-dd
    int calculateSupportPeriod(const std::string& releaseDate, const std::string& eol) {
        // get objects describing both dates
        std::istringstream iss(releaseDate);
        date::sys_days releaseDateDays;
        iss >> date::parse("%F", releaseDateDays);

        iss.str(eol);
        date::sys_days eolDays;
        iss >> date::parse("%F", eolDays);

        // containing both starting and ending day
        int duration_in_days = (eolDays - releaseDateDays).count() + 1;

        return duration_in_days;
    }

    void solution(const std::string& jsonFileName, int elementsCount) {
        std::cout << calculateSupportPeriod("2023-01-03", "2024-08-04") << "\n";
    }
}