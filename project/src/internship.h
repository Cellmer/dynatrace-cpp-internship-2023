#pragma once
#include <string>
#include <vector>

namespace internship
{

    struct OperatingSystem
    {
        std::string name;
        std::string cycle;
        int supportPeriod;

        OperatingSystem(std::string name, std::string cycle, int supportPeriod) : 
            name(name), cycle(cycle), supportPeriod(supportPeriod)
        {
        }
    };

    // returns duration in days, releaseDate and eol must be dates in format YYYY-mm-dd
    int calculateSupportPeriod(const std::string &releaseDate, const std::string &eol);

    // populates vector with values about os from json file
    void getOperatingSystemsFromJson(std::vector<OperatingSystem> &operatingSystems, const std::string &fileName);

    void solution(const std::string &jsonFileName, int elementsCount);

}