// ------------------------------------------------------------
// Class: VMAP0FeatureClass
//
// Description: Defines relationships between feature tables and
// primitives.
// ------------------------------------------------------------
#ifndef __Eaagles_Maps_Vpf_VMAP0FeatureClass_H__
#define __Eaagles_Maps_Vpf_VMAP0FeatureClass_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Maps {
namespace Vpf {

class VPFTable;

class VMAP0FeatureClass : public Basic::Object
{
    DECLARE_SUBCLASS(VMAP0FeatureClass, Basic::Object)

public:
    VMAP0FeatureClass();

    const char* getName()   { return name; }
    virtual void setName(const char* x);

    struct Relation {
        char table1[255];
        char table2[255];
        char table1key[255];
        char table2key[255];
        int table1Type;
        int table2Type;
    };

    virtual void addRelation(const char* table1, const char* table2, const char* table1key, const char* table2key);
    int determineTableType(const char* x);

private:
    static const int MAX_RELATIONS = 50;
    char name[150];     // Name of our feature class
    Relation* relations[MAX_RELATIONS];
    int numRelations;
};

} // End Vpf namespace
} // End Maps namespace
} // End Eaagles namespace

#endif

