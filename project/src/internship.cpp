#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cstdio>

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

        // date was in bad formatting
        if (!iss)
            throw std::invalid_argument("Unrecognised date format!");

        iss.str(eol);
        date::sys_days eolDays;
        iss >> date::parse("%F", eolDays);

        // date was in bad formatting
        if (!iss)
            throw std::invalid_argument("Unrecognised date format!");

        // release date is after eol
        if (releaseDateDays > eolDays)
            throw std::invalid_argument("Invalid dates!");

        // contains both starting and ending day
        int duration_in_days = (eolDays - releaseDateDays).count() + 1;

        return duration_in_days;
    }

    // populates vector with values about os from json file, omits bad formatted objects
    void getOperatingSystemsFromJson(std::vector<OperatingSystem> &operatingSystems, const std::string &fileName)
    {
        std::ifstream ifs(fileName);
        rapidjson::IStreamWrapper isw(ifs);
        rapidjson::Document products;
        products.ParseStream(isw);

        // iterate through products and push to vector all operating systems, omit badly formatted input
        for (const auto &product : products.GetArray())
        {
            if (product.HasMember("os") && product["os"].IsBool() && product["os"].GetBool())
            {
                if (!product.HasMember("versions") || !product["versions"].IsArray())
                    continue;

                // reserve place for all versions to avoid copying in every iteration
                operatingSystems.reserve(operatingSystems.size() + product["versions"].GetArray().Capacity());

                std::string name;
                std::string cycle;
                int supportPeriod;
                // add all os versions to vector
                for (const auto &os : product["versions"].GetArray())
                {
                    try
                    {
                        if (!product.HasMember("name") || !product["name"].IsString() ||
                            !os.HasMember("cycle") || !os["cycle"].IsString() ||
                            !os.HasMember("releaseDate") || !os["releaseDate"].IsString() ||
                            !os.HasMember("eol") || !os["eol"].IsString())
                            continue;

                        name = product["name"].GetString();
                        cycle = os["cycle"].GetString();
                        supportPeriod = calculateSupportPeriod(os["releaseDate"].GetString(), os["eol"].GetString());
                    }
                    catch (std::invalid_argument const &ex)
                    {
                        continue;
                    }

                    operatingSystems.emplace_back(name, cycle, supportPeriod);
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