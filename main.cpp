/*
maxlh:maximum likelihood
con:conditional
*/

#include <iostream>
#include <list>
#include <cmath>

#include "Member.h"
using namespace std;

list<Member> trainMemberList;

int featureNumber;
map<string, int> featureIDMap;
map<int, map<string, int> > featureTypeIDMap;

int typeNumber;
map<string, int> typeIDMap;
map<int, double> typeProbilityMap;

void type_maxlh_probability(bool con, int featureID, int featureTypeID)
{
    auto typeIter = typeIDMap.begin();

    while(typeIter != typeIDMap.end())
    {
        typeProbilityMap[typeIter->second] = 0;
    }

    if(con)
    {
        long long int number = 0;

        auto trainIter = trainMemberList.begin();

        while(trainIter != trainMemberList.end())
        {
            if((*trainIter).get_featureValueMap()[featureID] == featureTypeID)
            {
                typeProbilityMap[(*trainIter).get_typeID()]++;
                ++number;
            }

            ++trainIter;
        }

        auto typeProIter = typeProbilityMap.begin();

        while(typeProIter != typeProbilityMap.end())
        {
            typeProbilityMap[typeProIter->first] = typeProIter->second / (double)number;

            ++typeProIter;
        }
    }
    else
    {
        auto trainIter = trainMemberList.begin();

        while(trainIter != trainMemberList.end())
        {
            typeProbilityMap[(*trainIter).get_typeID()]++;

            ++trainIter;
        }

        auto typeProIter = typeProbilityMap.begin();

        while(typeProIter != typeProbilityMap.end())
        {
            typeProbilityMap[typeProIter->first] = typeProIter->second / (double)trainMemberList.size();

            ++typeProIter;
        }
    }


}

double empirical_entropy()
{
    double entropy = 0;

    type_maxlh_probability(false, 0, 0);

    auto typeProIter = typeProbilityMap.begin();

    while(typeProIter != typeProbilityMap.end())
    {
        entropy -= typeProIter->second * log(typeProIter->second);
        ++typeProIter;
    }

    return entropy;
}

double conditional_entropy()
{
    double entropy = 0;

    return entropy;
}

int main()
{

    return 0;
}
