
#ifndef __oe_base_ClassMetadata_H__
#define __oe_base_ClassMetadata_H__

namespace oe {
namespace base {
class SlotTable;

//------------------------------------------------------------------------------
// Class: ClassMetadata
//------------------------------------------------------------------------------
class ClassMetadata
{
public:
   ClassMetadata(const unsigned int, const char* const, const char* const, const SlotTable* const, const ClassMetadata* const);

   const unsigned int classIndex;     // Registered class index
   const char* const cname;           // class name from 'type_info'
   const char* const fname;           // class factory name
   const SlotTable* const st;         // Pointer to the SlotTable
   const ClassMetadata* const bstatic;   // Pointer to the base class MetaObject object
   int count;                         // Current number of object instances
   int mc;                            // Max number of instances
   int tc;                            // Total number of instances

private:
   ClassMetadata& operator=(const ClassMetadata&);   // hide assignment operator, do nothing useful

};

}
}

#endif

