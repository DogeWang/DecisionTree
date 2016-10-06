/*
���ȷ�Χ:32λ�з�������
maxlh:maximum likelihood
con:conditional
tempMutual:temporal mutual information
*/

#include <iostream>
#include <list>
#include <cmath>

#include "Member.h"
#include "TreeNode.h"
using namespace std;

list<Member> trainList;

int featureNumber = 0;
map<string, int> featureIDMap; // ��ÿһ����������һ����ֵӳ��
map<int, map<string, int> > featureTypeIDMap; // �������е�ÿһ������һ����ֵӳ��
int featureTrainNumber = 0;

int typeNumber = 0;
map<string, int> typeIDMap; // ��ÿһ��������һ����ֵӳ��
map<int, double> typeProbilityMap;

int parameterE = 0.5; // ��ֵ���ж��Ƿ������֧

void type_maxlh_probability(bool con, int featureID, int featureTypeID, list<Member> subTrainList)
{
    auto typeIter = typeIDMap.begin();

    while(typeIter != typeIDMap.end())
    {
        typeProbilityMap[typeIter->second] = 0;

        ++typeIter;
    }

    if(con)
    {
        featureTrainNumber = 0;

        auto subTrainIter = subTrainList.begin();

        while(subTrainIter != subTrainList.end())
        {
            if((*subTrainIter).get_featureValueMap()[featureID] == featureTypeID)
            {
                typeProbilityMap[(*subTrainIter).get_typeID()]++;
                ++featureTrainNumber;
            }

            ++subTrainIter;
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
        auto subTrainIter = subTrainList.begin();

        while(subTrainIter != subTrainList.end())
        {
            typeProbilityMap[(*subTrainIter).get_typeID()]++;

            ++subTrainIter;
        }

        auto typeProIter = typeProbilityMap.begin();

        while(typeProIter != typeProbilityMap.end())
        {
            typeProIter->second = typeProIter->second / (double)subTrainList.size();

            ++typeProIter;
        }
    }
}

double empirical_entropy(list<Member> subTrainList)
{
    double entropy = 0;

    type_maxlh_probability(false, 0, 0, subTrainList);

    auto typeProIter = typeProbilityMap.begin();

    while(typeProIter != typeProbilityMap.end())
    {
        entropy -= typeProIter->second * log(typeProIter->second);

        ++typeProIter;
    }

    return entropy;
}

double empirical_conditional_entropy(int featureID, list<Member> subTrainList)
{
    double entropy = 0;

    auto featureTypeIter = featureTypeIDMap[featureID].begin();

    while(featureTypeIter != featureTypeIDMap[featureID].end())
    {
        type_maxlh_probability(true, featureID, featureTypeIter->second, subTrainList);

        auto typeProIter = typeProbilityMap.begin();
        double featureEntropy = 0;

        while(typeProIter != typeProbilityMap.end())
        {
            featureEntropy -= typeProIter->second * log(typeProIter->second);

            ++typeProIter;
        }

        entropy += ((double)featureTrainNumber / (double)subTrainList.size()) * featureEntropy;

        ++featureTypeIter;
    }

    return entropy;
}

double feature_entropy(int featureID, list<Member> subTrainList)
{
    double entropy = 0;

    auto featureTypeIter = featureTypeIDMap[featureID].begin();

    while(featureTypeIter != featureTypeIDMap[featureID].end())
    {
        double tempNumber = 0;

        auto subTrainIter = subTrainList.begin();

        while(subTrainIter != subTrainList.end())
        {
            if((*subTrainIter).get_featureValueMap()[featureID] == featureTypeIter->second)
            {
                ++tempNumber;
            }

            ++subTrainIter;
        }

        tempNumber /=  (double)subTrainList.size();
        entropy -= tempNumber * log(tempNumber);

        ++featureTypeIter;
    }

    return entropy;
}

int max_type(list<Member> subTrainList)
{
    map<int, int> typeNumberMap;

    auto typeIter = typeIDMap.begin();

    while(typeIter != typeIDMap.end())
    {
        typeNumberMap[typeIter->second] = 0;

        ++typeIter;
    }

    auto subTrainIter = subTrainList.begin();

    while(subTrainIter != subTrainList.end())
    {
        typeNumberMap[(*subTrainIter).get_typeID()]++;

        ++subTrainIter;
    }

    int maxNumber = 0;
    int maxTypeID = -1;
    auto typeNumIter = typeNumberMap.begin();

    while(typeNumIter != typeNumberMap.end())
    {
        if(typeNumIter->second > maxNumber)
        {
            maxNumber = typeNumIter->second;
            maxTypeID = typeNumIter->first;
        }

        ++typeNumIter;
    }

    return maxTypeID;
}

TreeNode ID3_C45(bool ic,list<Member> subTrainList)
{
    auto featureIDIter = featureIDMap.begin();
    double maxFeatureValue = 0;
    int maxFeatureID = -1;

    if(ic)
    {
        while(featureIDIter != featureIDMap.end())
        {
            double tempMutual = empirical_entropy(subTrainList) - empirical_conditional_entropy(featureIDIter->second, subTrainList);

            if(tempMutual > maxFeatureValue)
            {
                maxFeatureValue = tempMutual;
                maxFeatureID = featureIDIter->second;
            }
        }
    }
    else
    {
        while(featureIDIter != featureIDMap.end())
        {
            double tempMutual = empirical_entropy(subTrainList) - empirical_conditional_entropy(featureIDIter->second, subTrainList);

            tempMutual /= feature_entropy(featureIDIter->second, subTrainList);

            if(tempMutual > maxFeatureValue)
            {
                maxFeatureValue = tempMutual;
                maxFeatureID = featureIDIter->second;
            }
        }
    }



    if(maxFeatureValue >= parameterE)
    {
        TreeNode treeNode(true);
        treeNode.set_featureID(maxFeatureID);

        auto featureTypeIter = featureTypeIDMap[maxFeatureID].begin();

        while(featureTypeIter != featureTypeIDMap[maxFeatureID].end())
        {
            list<Member> tempTrainList;

            auto subTrainIter = subTrainList.begin();

            while(subTrainIter != subTrainList.end())
            {
                if((*subTrainIter).get_featureValueMap()[maxFeatureID] == featureTypeIter->second)
                {
                    tempTrainList.push_back((*subTrainIter));
                }

                ++subTrainIter;
            }

            treeNode.get_treeNodeMap()[featureTypeIter->second] = ID3_C45(ic, tempTrainList);

            ++featureTypeIter;
        }

        return treeNode;
    }
    else
    {
        TreeNode treeNode(false);
        treeNode.set_typeID(max_type(subTrainList));

        return treeNode;
    }
}

int main()
{

    return 0;
}
