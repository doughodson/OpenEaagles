// -------------------------------------------------------------------------------
// Class -> VMAP0RefCoverageDirectory
// Description: VMAP level 0 coverage directorie(s), which contain
// - fcs (feature class schema)
// - libref.lft 
// - libref.tft
// - primitive tables
// -------------------------------------------------------------------------------
#ifndef	__EAAGLES_MAPS_VPF_VMAP0REFCOVERAGEDIRECTORY_H__
#define __EAAGLES_MAPS_VPF_VMAP0REFCOVERAGEDIRECTORY_H__

#include "openeaagles/maps/vpfMap/VPFDirectory.h"
#include "openeaagles/maps/vpfMap/VMAP0FeatureClass.h"

namespace Eaagles {
namespace Maps {
namespace Vpf {

class VPFTable;
class VPFSpatialIndexTable;
class VMPA0FeatureClass;

class VMAP0RefCoverageDirectory : public VPFDirectory {
   DECLARE_SUBCLASS(VMAP0RefCoverageDirectory,VPFDirectory)

public:
    VMAP0RefCoverageDirectory();

    virtual void loadTables();

    virtual void buildFeatureClasses();

    VMAP0FeatureClass* getFeatureClassByName(const char* name);

    // Here are all the feature table file types (attribute and primitive)
    enum { END = 0, PLACENAM_DOT_PFT, CND, EDG, EBR, MAX_FEATURE_TABLES };

    // Creates all the needed tables based on our coverage type
    void createTables();

    int getPlacenameCoordsByRecord(const int r, osg::Vec3 vec[], const int idx, const int max);
    bool getPlacenameByRecord(const int r, char* x);
    
    int getPlacenameCoordsByRange(const float width, const float height, const float refLat, const float refLon, osg::Vec3 vec[], const int idx, const int max);
    

    int getPolBndCoordsByRecord(const int r, osg::Vec3 vec[], const int idx, const int max);

    // Get by lat lon bounding box
    int getPolBndCoordsByLatLon(const float maxLat, const float maxLon, const float minLat, const float minLon, const int idx, osg::Vec3 vec[], const int max);

    // Here are all the spacial index table file types
    enum { NSI = 0, MAX_SPATIAL_INDEX_TABLES };

    int getSpatialQueryPlacenamePrimID(const int idx);

private:
    static const int MAX_FEATURE_CLASSES = 5;
    // Our feature class schema coverage directory
    VMAP0FeatureClass* featureClass[MAX_FEATURE_CLASSES];
    int numFeatureClasses;                  // Number of feature classes we have
    int spatialQueryPrimIds[500];           // Holds the primitive IDs of the last spatial query for quick access

    VPFTable* featureTables[MAX_FEATURE_TABLES];
    VPFSpatialIndexTable* spatialTables[MAX_SPATIAL_INDEX_TABLES];
};

}; // End Vpf namespace
}; // End Maps namespace 
}; // End Eaagles namespace

#endif // __EAAGLES_MAPS_VPF_VMAP0REFCOVERAGEDIRECTORY_H__
