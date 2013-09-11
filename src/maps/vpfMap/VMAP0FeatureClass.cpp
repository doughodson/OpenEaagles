// ------------------------------------------------------------
// Class: VMAP0FeatureClass
// ------------------------------------------------------------

#include "openeaagles/maps/vpfMap/VMAP0FeatureClass.h"
#include "openeaagles/basic/String.h"

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace Maps {
namespace Vpf {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(VMAP0FeatureClass,"VMAP0FeatureClass")
EMPTY_SERIALIZER(VMAP0FeatureClass)

EMPTY_COPYDATA(VMAP0FeatureClass)

VMAP0FeatureClass::VMAP0FeatureClass()
{
    for (int i = 0; i < MAX_RELATIONS; i++) relations[i] = 0;
    //for (int i = 0; i < MAX_FEATURE_TABLES; i++) tables[i] = 0;
    numRelations = 0;
}

void VMAP0FeatureClass::deleteData()
{
    for (int i = 0; i < MAX_RELATIONS; i++) relations[i] = 0;
}

void VMAP0FeatureClass::setName(const char* x)
{
    //std::cout << "CREATING FEATURE CLASS = " << x << std::endl;
    std::sprintf(name, "%s", x);
}

void VMAP0FeatureClass::addRelation(const char* table1, const char* table2, const char *table1key, const char *table2key)
{
    char* tempTable1 = (char*)table1;
    char* tempTable2 = (char*)table2;
    char* tempTable1Key = (char*)table1key;
    char* tempTable2Key = (char*)table2key;

    // truncate our values
    size_t size = strlen(tempTable1);
    size_t count = 0;
    while (tempTable1[count] != ' ' && count < size) count++;
    tempTable1[count] = 0;

    size = strlen(tempTable2);
    count = 0;
    while (tempTable2[count] != ' ' && count < size) count++;
    tempTable2[count] = 0;

    size = strlen(tempTable1Key);
    count = 0;
    while (tempTable1Key[count] != ' ' && count < size) count++;
    tempTable1Key[count] = 0;

    size = strlen(tempTable2Key);
    count = 0;
    while (tempTable2Key[count] != ' ' && count < size) count++;
    tempTable2Key[count] = 0;

    relations[numRelations] = new Relation();

    strcpy(relations[numRelations]->table1, tempTable1);
    //std::sprintf(relations[numRelations]->table1, "%s", tempTable1);
    //lcStrcpy(relations[numRelations]->table1, sizeof(relations[numRelations]->table1), tempTable1);
    strcpy(relations[numRelations]->table2, tempTable2);
    //std::sprintf(relations[numRelations]->table2, "%s", tempTable2);
    //lcStrcpy(relations[numRelations]->table2, sizeof(relations[numRelations]->table2), tempTable2);
    strcpy(relations[numRelations]->table1key, tempTable1Key);
    //std::sprintf(relations[numRelations]->table1key, "%s", tempTable1Key);
    //lcStrcpy(relations[numRelations]->table1key, sizeof(relations[numRelations]->table1key), tempTable1Key);
    strcpy(relations[numRelations]->table2key, tempTable2Key);
    //std::sprintf(relations[numRelations]->table2key, "%s", tempTable2Key);
    //lcStrcpy(relations[numRelations]->table2key, sizeof(relations[numRelations]->table2key), tempTable2Key);
    relations[numRelations]->table1Type = -1;
    relations[numRelations]->table2Type = -1;
#if defined DEBUG_OUTPUT
    std::cout << "RELATION #" << numRelations+1 << " = " << relations[numRelations]->table1 << ", " << relations[numRelations]->table1key \
        << ", " << relations[numRelations]->table2 << ", " << relations[numRelations]->table2key << std::endl;
#endif
    numRelations++;
}

} // end Vpf namespace
} // end Maps namespace
} // end Eaagles namespace

