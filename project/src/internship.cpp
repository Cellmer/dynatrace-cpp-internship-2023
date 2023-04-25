#include <fstream>
#include <iostream>
#include <algorithm>

#include <date/date.h>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include "internship.h"

namespace internship
{

    std::ostream &operator<<(std::ostream &stream, const OperatingSystem &os)
    {
        stream << os.name << " " << os.cycle << " " << os.supportPeriod;
        return stream;
    }

    // returns duration in days, releaseDate and eol must be dates in format YYYY-mm-dd
    int calculateSupportPeriod(const std::string &releaseDate, const std::string &eol)
    {
        // get objects describing both dates
        std::istringstream iss(releaseDate);
        date::sys_days releaseDateDays;
        iss >> date::parse("%F", releaseDateDays);

        iss.str(eol);
        date::sys_days eolDays;
        iss >> date::parse("%F", eolDays);

        // contains both starting and ending day
        int duration_in_days = (eolDays - releaseDateDays).count() + 1;

        return duration_in_days;
    }

    // populates vector with values about os from json file
    void getOperatingSystemsFromJson(std::vector<OperatingSystem> &operatingSystems, const std::string &fileName)
    {
        std::ifstream ifs(fileName);
        rapidjson::IStreamWrapper isw(ifs);
        rapidjson::Document products;
        products.ParseStream(isw);

        for (const auto &product : products.GetArray())
        {
            if (product["os"].GetBool())
            {
                // reserve place for all versions to avoid copying in every iteration
                operatingSystems.reserve(operatingSystems.size() + product["versions"].GetArray().Capacity());

                // add all os versions to vector
                for (const auto &os : product["versions"].GetArray())
                {
                    operatingSystems.emplace_back(product["name"].GetString(),
                                                  os["cycle"].GetString(),
                                                  calculateSupportPeriod(os["releaseDate"].GetString(), os["eol"].GetString()));
                }
            }
        }
    }

    void solution(const std::string &jsonFileName, int elementsCount)
    {
        std::vector<OperatingSystem> operatingSystems;
        getOperatingSystemsFromJson(operatingSystems, jsonFileName);

        // sort vector in order to extract n max values
        std::partial_sort(operatingSystems.begin(), operatingSystems.begin() + elementsCount, operatingSystems.end(),
                          [](OperatingSystem a, OperatingSystem b)
                          { return a.supportPeriod > b.supportPeriod; });

        for (int i = 0; i < elementsCount; i++)
        {
            std::cout << operatingSystems[i] << "\n";
        }
    }

}