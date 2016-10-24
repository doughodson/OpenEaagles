
#ifndef __oe_base_Function_H__
#define __oe_base_Function_H__

#include "openeaagles/base/Object.hpp"
#include "openeaagles/base/safe_ptr.hpp"

namespace oe {
namespace base {

class FStorage;
class List;
class Table;

//------------------------------------------------------------------------------
// Class: Function
//
// Description: Base for the derived, multi-dimensional function classes.
//
// Notes:
//    1) Use the storageFactory() function to create the FStorage object that
//       will maintain the previous function call values (i.e., integration).
//
//   virtual FStorage* storageFactory() const;
//       Data storage factory (pre-ref()'d)
//   virtual bool setSlotLfiTable(const Table* const msg);
//       Set Slot Tables
//
// Slots:
//    table    <Table>     ! Optional LFI table
//
//------------------------------------------------------------------------------
class Function : public Object
{
    DECLARE_SUBCLASS(Function, Object)

public:
   Function();

   virtual FStorage* storageFactory() const;    // Data storage factory (pre-ref()'d)
   virtual bool setSlotLfiTable(const Table* const msg);

protected:
   const Table* getTable() const {  return table; }

private:
   safe_ptr<const Table> table;   // Optional LFI Table
};

//------------------------------------------------------------------------------
// class FStorage
// Description: Generic storage class for past values of function calls.
//              Use Function::storageFactory() to generate pre-ref()'d
//              FStorage objects.
//------------------------------------------------------------------------------
class FStorage : public Object {
   DECLARE_SUBCLASS(FStorage, Object)
public:
   FStorage();
};

}
}

#endif
