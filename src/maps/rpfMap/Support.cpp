#include "openeaagles/maps/rpfMap/Support.h"
#include <ctype.h>
#include <string.h>

namespace Eaagles {
namespace Maps {
namespace Rpf {

// -----------------------------------------------------------
// swap() - Simple static function to swap bytes.
// -----------------------------------------------------------
void swap(unsigned char *ptr, int count)
{
    unsigned char   *end, temp;
    
    end = &ptr[count - 1];
    while (end > ptr)
    {
        temp = *end;
        *end = *ptr;
        *ptr = temp;
    
        ptr++, end--;
    }
}

// ----------------------------------------------------------------
// stringToLowercase() - Takes in a string, and makes it lowercase.
// ----------------------------------------------------------------
char* stringToLowercase(char* str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) {
        if (isascii(str[i]) && isalpha(str[i])) str[i] = tolower(str[i]);
    }

    return str;
}

// -------------------------------------------------------------------------
// parseLocations() - At this point we are at the end of the header section 
// and now we need to start reading the location data. - this function 
// takes in a file, and finds the proper locations for our component types.
// -------------------------------------------------------------------------
void parseLocations(std::ifstream& fin, Location* locs, int count)
{
    // We are at the location section portion of the file, which holds the 
    // following information (MIL-STD-2411, page 45)
    // Name                                 Type    Size (Bytes)    
    //  <location section length>            ushort   2             - Size of the location section
    //  <component location table offset>    uint     4             - Position of component table
    //  <num component location records>     ushort   2             - Number of records we have
    //  <component location record length>   ushort   2             - Length of each component record
    //  <component aggregate length>         ushort   2             - Length of all component records
    //  <component location table>           
    //      <component location record>  (2... many)
    //          <component id>               ushort   2             - Specific id
    //          <component length>           uint     4             - Length of the component (named by the id)
    //          <component location>         uint     4             - Physical address of the component

	ushort numRecords;
	ushort id;
	uint physicalIdx;
    // Easy way to skip length uint (4) and ushort (2)
	uint skipLong;
    ushort skip;


	// Initialize indices so we can later tell if they weren't found 
	for (int j = 0; j < count; j++)
	{
		// DKS: want physical index: offset from start of file 
		locs[j].physicalIdx = uint(~0);
	}

	// Skip location section length 
	fin.read((char *) &skip, sizeof(skip));

	// Skip component location table offset 
	fin.read((char *) &skipLong, sizeof(skipLong));

	// How many sections: # of section location records 
	fin.read((char *) &numRecords, sizeof(numRecords));
	swap((unsigned char *) &numRecords, sizeof(numRecords));

	// Skip location record length 
	fin.read((char *) &skip, sizeof(skip));

	// Skip component aggregate length 
	fin.read((char *) &skipLong, sizeof(skipLong));

	// Now go find the ones we want 
	for (int i = 0; i < numRecords; i++) {
        // Get the component id
		fin.read((char *) &id, sizeof(id));
		// Skip section length 
		fin.read((char *) &skipLong, sizeof(skipLong));
        // Read our physical offset 
		fin.read((char *) &physicalIdx, sizeof(physicalIdx));

        // Swap our bytes
		swap((unsigned char *) &id, sizeof(id));
		swap((unsigned char *) &physicalIdx, sizeof(physicalIdx));

		for (int j = 0; j < count; j++) {
			if (locs[j].componentId == id) {
				// Get our physical offset for this component!
				locs[j].physicalIdx = physicalIdx;
			}
		}
	}
}

};  // End Rpf namespace
};  // End Maps namespace
};  // End Eaagles namespace
