#ifndef MEMBER_H_INCLUDED
#define MEMBER_H_INCLUDED

#include<map>
using std::map;

class Member {
public:
    Member() {};
    Member(int ID, map<int, int> featureValueMap)
    {
        this->ID = ID;
        this->featureValueMap = featureValueMap;
    };
    Member(int ID, map<int, int> featureValueMap, int typeID)
    {
        this->ID = ID;
        this->featureValueMap = featureValueMap;
        this->typeID = typeID;
    };

    int get_ID() {return this->ID;};
    map<int, int> get_featureValueMap() {return this->featureValueMap;};

    void set_typeID(int typeID) { this->typeID = typeID;};
    int get_typeID() {return this->typeID;};
private:
    int ID;
    map<int, int> featureValueMap;
    int typeID;
};

#endif // MEMBER_H_INCLUDED
