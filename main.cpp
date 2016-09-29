/*
精度范围:32位有符号类型
maxlh:maximum likelihood
con:conditional
*/

#include <iostream>
#include <list>
#include <cmath>

#include "Member.h"
using namespace std;

list<Member> trainMemberList;

int featureNumber = 0;
map<string, int> featureIDMap;
map<int, map<string, int> > featureTypeIDMap;
int featureTrainNumber = 0;


int typeNumber = 0;
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
        featureTrainNumber = 0;

        auto trainIter = trainMemberList.begin();

        while(trainIter != trainMemberList.end())
        {
            if((*trainIter).get_featureValueMap()[featureID] == featureTypeID)
            {
                typeProbilityMap[(*trainIter).get_typeID()]++;
                ++featureTrainNumber;
            }

            ++trainIter;
        }

        auto typeProIter = typeProbilityMap.begin();

        while(typeProIter != typeProbilityMap.end())
        {
            typeProIter->second = typeProIter->second / (double)featureTrainNumber;

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
            typeProIter->second = typeProIter->second / (double)trainMemberList.size();

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

double empirical_conditional_entropy(int featureID)
{
    double entropy = 0;

    auto featureTypeIter = featureTypeIDMap[featureID].begin();

    while(featureTypeIter != featureTypeIDMap[featureID].end())
    {
        type_maxlh_probability(true, featureID, featureTypeIter->second);

        auto typeProIter = typeProbilityMap.begin();
        double featureEntropy = 0;

        while(typeProIter != typeProbilityMap.end())
        {
            featureEntropy -= typeProIter->second * log(typeProIter->second);

            ++typeProIter;
        }

        entropy += ((double)featureTrainNumber / (double)trainMemberList.size()) * featureEntropy;

        ++featureTypeIter;
    }

    return entropy;
}

int main()
{

    return 0;
}
