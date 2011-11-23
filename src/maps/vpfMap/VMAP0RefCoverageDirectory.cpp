#include "openeaagles/maps/vpfMap/VMAP0RefCoverageDirectory.h"
#include "openeaagles/maps/vpfMap/VMAP0ReferenceDirectory.h"
#include "openeaagles/maps/vpfMap/VPFTable.h"
#include "openeaagles/maps/vpfMap/VPFSpatialIndexTable.h"
#include "openeaagles/maps/vpfMap/VPFRecord.h"
#include "openeaagles/basic/String.h"

// disable all deprecation warnings for now, until we fix
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace Maps {
namespace Vpf {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(VMAP0RefCoverageDirectory,"VMAP0RefCoverageDirectory")
EMPTY_SERIALIZER(VMAP0RefCoverageDirectory)


VMAP0RefCoverageDirectory::VMAP0RefCoverageDirectory()
{
    STANDARD_CONSTRUCTOR()
    numFeatureClasses = 0;
    for (int i = 0; i < MAX_FEATURE_CLASSES; i++) featureClass[i] = 0;
    for (int i = 0; i < MAX_FEATURE_TABLES; i++) featureTables[i] = 0;
    for (int i = 0; i < MAX_SPATIAL_INDEX_TABLES; i++) spatialTables[i] = 0;
    for (int i = 0; i < 500; i++) spatialQueryPrimIds[i] = 0;
}

void VMAP0RefCoverageDirectory::copyData(const VMAP0RefCoverageDirectory& org, const bool)
{
    BaseClass::copyData(org);
}

void VMAP0RefCoverageDirectory::deleteData()
{
    for (int i = 0; i < MAX_FEATURE_CLASSES; i++) {
        if (featureClass[i] != 0) featureClass[i]->unref();
        featureClass[i] = 0;
    }
    for (int i = 0; i < MAX_FEATURE_TABLES; i++) {
        if (featureClass[i] != 0) featureTables[i]->unref();
        featureTables[i] = 0;
    }
    for (int i = 0; i < MAX_SPATIAL_INDEX_TABLES; i++) {
        if (featureClass[i] != 0) spatialTables[i]->unref();
        spatialTables[i] = 0;
    }
}

void VMAP0RefCoverageDirectory::loadTables()
{
    Basic::String* string = new Basic::String(getPath());

    createTable(VPFDirectory::FCS);
    VPFTable* fcs = getTable(VPFDirectory::FCS);
    if (fcs != 0 && !fcs->isLoaded()) fcs->loadTableFromFile(string->getString(), "fcs", VPFDirectory::FCS);


    // ok, let's build our feature class(es) from our feature class schema
    if (fcs->isLoaded()) buildFeatureClasses();

    // based on the produt specification for VMAP level 0, there are 4 reference coverage directories
    // libref, dbref, polbnd, and placenam.
    if (getType() == VMAP0ReferenceDirectory::CVG_PLACENAM) {
        // for the placenam directory, there is one simple primitive table and attribute table
        // they are placenam.pft and end
        // There is also a node spacial index (nsi) table for minimum boundary rectangle lookups
        if (featureTables[PLACENAM_DOT_PFT] == 0) featureTables[PLACENAM_DOT_PFT] = new VPFTable();
        if (!featureTables[PLACENAM_DOT_PFT]->isLoaded()) {
            // don't give it a type, because we are keeping track of the type
            featureTables[PLACENAM_DOT_PFT]->loadTableFromFile(string->getString(), "placenam.pft");
        }
        
        // ok, now our entity node primitive table
        if (featureTables[END] == 0) featureTables[END] = new VPFTable();
        if (!featureTables[END]->isLoaded()) {
            // don't give it a type, because we are keeping track of the type
            featureTables[END]->loadTableFromFile(string->getString(), "end");
        }

        // SPATIAL TABLES FOR QUICK REFERENCE!
        if (spatialTables[NSI] == 0) spatialTables[END] = new VPFSpatialIndexTable();
        if (!spatialTables[NSI]->isLoaded()) {
            // don't give it a type, because we are keeping track of the type
            spatialTables[NSI]->loadIndexTableFromFile(string->getString(), "nsi");
            // SLS test
            // now do a quick spatial query 
            //spatialTables[NSI]->findPrimitivesBySpatialQuery(32, -115);
        }
        

    }
    else if (getType() == VMAP0ReferenceDirectory::CVG_POLBND) {
        if (featureTables[CND] == 0) featureTables[CND] = new VPFTable();
        if (!featureTables[CND]->isLoaded()) {
            // don't give it a type, because we are keeping track of the type
            featureTables[CND]->loadTableFromFile(string->getString(), "cnd");
        }
        // we now have to open our edge primitive table, which will contain information
        // about how our connected nodes are put together
        if (featureTables[EDG] == 0) featureTables[EDG] = new VPFTable();
        if (!featureTables[EDG]->isLoaded()) {
            // don't give it a type, because we are keeping track of the type
            featureTables[EDG]->loadTableFromFile(string->getString(), "edg");
        }
        // now we have to open our edge bouding rectangle, because it contains information
        // to do quick queries on our edg file
        if (featureTables[EBR] == 0) featureTables[EBR] = new VPFTable();
        if (!featureTables[EBR]->isLoaded()) {
            // don't give it a type, because we are keeping track of the type
            featureTables[EBR]->loadTableFromFile(string->getString(), "ebr");
        }

    }
    
    string->unref();
}

int VMAP0RefCoverageDirectory::getPolBndCoordsByLatLon(const float maxLat, const float maxLon, const float minLat, const float minLon, const int idx, osg::Vec3 vec[], const int max) 
{
    int numCoords = 0;
    if (featureTables[EBR]->isLoaded() && featureTables[EDG]->isLoaded()) {
        VPFRecord* record = 0;
        int index = 1;
        record = featureTables[EBR]->getRecord(index);
        double xMin = 0, yMin = 0, xMax = 0, yMax = 0;
        // take the absolute value of xmin thru xmax and our lat coverage, so we don't have to worry about sign
        while (record != 0) {
            xMin = atof(record->getData(2));
            xMax = atof(record->getData(4));
            yMin = atof(record->getData(3));
            yMax = atof(record->getData(5));
            if ((yMax <= maxLat && yMax >= minLat) || (yMin >= minLat && yMin <= maxLat)) {
                // ok, we are in our lat range, now lets check our lon!
                if ((xMax <= maxLon && xMax >= minLon) || (xMin >= minLon && xMin <= maxLon)) {
                    //int temp = record->getCoordinate
                    VPFRecord* r = featureTables[EDG]->getRecord(index);
                    if (r != 0) {
                        int temp = r->getCoordinate(8, vec, idx, max);
                        numCoords += temp;
                    }
                }
            }                        
            index++;
            record = featureTables[EBR]->getRecord(index);
        }
#if defined DEBUG_OUTPUT
        std::cout << "NUM RECORDS = " << index << std::endl;
#endif
    }
    return numCoords;
}

int VMAP0RefCoverageDirectory::getPlacenameCoordsByRecord(const int r, osg::Vec3 vec[], const int idx, const int max)
{
    int numCoords = 0;
    // first, make sure our table exists, and then get the record
    if (featureTables[END] != 0 && featureTables[END]->isLoaded()) {
        VPFRecord* rec = featureTables[END]->getRecord(r);
        // our coordinate is column 3
        if (rec != 0) {
            numCoords = rec->getCoordinate(3, vec, idx, max);
        }
    }
    else std::cout << "NO ENTITY NODE PRIMITIVE TABLE FOUND, PATH = " << getPath() << std::endl;


    return numCoords;
}

int VMAP0RefCoverageDirectory::getPlacenameCoordsByRange(const float width, const float height, const float refLat, const float refLon, osg::Vec3 vec[], const int idx, const int max)
{
    int numCoords = idx;
    int tempIdx = idx;
    // get our node spatial index to do a query
    if (spatialTables[NSI] != 0) {
        int numPrims = spatialTables[NSI]->findPrimitivesBySpatialQuery(refLat, refLon, spatialQueryPrimIds, width, height);
        VPFRecord* v = 0;
        for (int i = 0; i < numPrims && (i < max-1); i++) {
            // now that we have the primitive ids of the values, we can query our entity node primitive table to obtain the coordinates
            if (featureTables[END] != 0) {
                v = featureTables[END]->getRecord(spatialQueryPrimIds[i]);
                if (v != 0 && numCoords < max) {
                    int temp = v->getCoordinate(3, vec, tempIdx, max);
                    numCoords += temp;
                    tempIdx += temp;
                }
            }
        }
    }
    return numCoords;
}

int VMAP0RefCoverageDirectory::getSpatialQueryPlacenamePrimID(const int idx)
{
    if (idx < 500) return spatialQueryPrimIds[idx-1];
    return -1;
}

bool VMAP0RefCoverageDirectory::getPlacenameByRecord(const int r, char* p)
{
    bool ok = false;
    // first, make sure our table exists, and then get the record
    if (featureTables[PLACENAM_DOT_PFT] != 0 && featureTables[PLACENAM_DOT_PFT]->isLoaded()) {
        VPFRecord* rec = featureTables[PLACENAM_DOT_PFT]->getRecord(r);
        // our placaname is column 2
        if (rec != 0) {
            ok = true;
            //lcStrcpy(p, sizeof(p), rec->getData(2));
            strcpy(p, rec->getData(2));
            //std::sprintf(p, "%s", rec->getData(2));
        }
    }
    else std::cout << "NO PLACENAME POINT FEATURE TABLE FOUND, PATH = " << getPath() << std::endl;
    return ok;
}

int VMAP0RefCoverageDirectory::getPolBndCoordsByRecord(const int r, osg::Vec3 vec[], const int idx, const int max)
{
    int numCoords = 0;
    // first, make sure our table exists, and then get the record
    if (featureTables[EDG] != 0 && featureTables[EDG]->isLoaded()) {
        VPFRecord* rec = featureTables[EDG]->getRecord(r);
        // our boundary coordinate is column 8
        if (rec != 0) numCoords = rec->getCoordinate(8, vec, idx, max);            
    }
    else std::cout << "NO EDGE TABLE FOUND, PATH = " << getPath() << std::endl;
    return numCoords;
}

void VMAP0RefCoverageDirectory::buildFeatureClasses()
{
    char* d;
    bool exists = false;
    int holder = 0;
    VPFTable* fcs = getTable(VPFDirectory::FCS);
    // first, build our classes, then add the relationships
    if (fcs != 0) {
        VPFRecord* record = 0;
        for (int i = 1; fcs->getRecord(i) != 0 && numFeatureClasses < MAX_FEATURE_CLASSES; i++) {
            record = fcs->getRecord(i);
            d = (char*)record->getData(2); 
            // now count our table name and truncate it
            size_t size = strlen(d);
            size_t count = 0;
            while (d[count] != ' ' && count < size) count++;
            d[count] = 0;            

            exists = false;
            holder = 0;
            if (d != 0) {
                if (numFeatureClasses > 0) {
                    for (int x = 0; x < numFeatureClasses; x++) {
                        if (strcmp(featureClass[x]->getName(), d) == 0) {
                            holder = x;
                            exists = true;
                        }
                    }
                    if (!exists) {
                        featureClass[numFeatureClasses] = new VMAP0FeatureClass();
                        featureClass[numFeatureClasses]->setName(d);
                        // add our relationship
                        featureClass[numFeatureClasses]->addRelation(record->getData(3), record->getData(5), \
                            record->getData(4), record->getData(6));
                        numFeatureClasses++;
                    }
                    else {
                        featureClass[holder]->addRelation(record->getData(3), record->getData(5), \
                            record->getData(4), record->getData(6));
                    }
                }
                else {
                    featureClass[numFeatureClasses] = new VMAP0FeatureClass();
                    featureClass[numFeatureClasses]->setName(d);
                    featureClass[numFeatureClasses]->addRelation(record->getData(3), record->getData(5), \
                        record->getData(4), record->getData(6));
                    numFeatureClasses++;
                }
            }
        }
    }
}

VMAP0FeatureClass* VMAP0RefCoverageDirectory::getFeatureClassByName(const char* name)
{
    for (int i = 0; i < numFeatureClasses; i++) {
        if (strcmp(featureClass[i]->getName(),name) == 0) return featureClass[i];
    }
    return 0;
}

}; // end Vpf namespace
}; // end Maps namespace 
}; // end Eaagles namespace
