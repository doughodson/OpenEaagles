//------------------------------------------------------------------------------
// Class: PairStream
//------------------------------------------------------------------------------
#ifndef	__Eaagles_Basic_PairStream_H__
#define __Eaagles_Basic_PairStream_H__

#include "openeaagles/basic/List.h"

namespace Eaagles {
namespace Basic {

class Identifier;
class Pair;

//------------------------------------------------------------------------------
// Class: PairStream
//
// Description: A PairStream is a list of Pair objects. Pair objects consist of 
//              an object and the name of the object.
//
// Form name: PairStream
//
// Public methods:
//
//   Finds the first pair by object type
//     Pair* findByType(const std::type_info& type);              
//   
//   Finds the first pair by object type (const version)
//     const Pair* findByType(const std::type_info& type) const;   
//
//   Finds a pair by name
//     Pair* findByName(const char* const slotname);               
//
//   Finds a pair by name (const version)
//     const Pair* findByName(const char* const slotname) const;   
//
//   Finds the name associated with an object
//     const Identifier* findName(const Object* const obj) const;
//
//   Returns the n'th pair
//     Pair* getPosition(const unsigned int n)    
//
//   Returns the n'th pair (const version)
//     const Pair* getPosition(const unsigned int n) const 
//
//   Returns a pointer to the pair at the head of this stream and the pair IS REMOVED
//   from the stream.  Empty streams will return null(0).  Ownership of the Pair is passed
//   to the caller (i.e., this routine does not unref() the pair and the caller should
//   not ref() the pair).
//     Pair* get() 
//
//   Put 'pair' at the tail of the stream.  The Pair is referenced, ref(), by this routine.
//     void put(Pair* pair1) 
//
//   Removes 'pair' from this list and true is returned.  If 'pair' 'obj' is not found
//   then false is returned.  (Note: The Pair is unref() and therefore possible to delete)
//     bool remove(Pair* pair1) 
//
//------------------------------------------------------------------------------
class PairStream : public List {
    DECLARE_SUBCLASS(PairStream,List)

public:
   PairStream();

   bool operator==(const PairStream& stream) const;
   bool operator!=(const PairStream& stream) const;

   // Finds the first pair by object type
   Pair* findByType(const std::type_info& type);              

   // Finds the first pair by object type (const version)
   const Pair* findByType(const std::type_info& type) const;   

   // Finds a pair by name
   Pair* findByName(const char* const slotname);               

   // Finds a pair by name (const version)
   const Pair* findByName(const char* const slotname) const;   

   // Finds the name associated with an object
   const Identifier* findName(const Object* const obj) const;

   // Returns the n'th pair
   Pair* getPosition(const unsigned int n)    {
      return (Pair*) List::getPosition(n);
   }

   // Returns the n'th pair (const version)
   const Pair* getPosition(const unsigned int n) const {
      return (const Pair*) List::getPosition(n);
   }

   // Returns a pointer to the pair at the head of this stream and the pair IS REMOVED
   // from the stream.  Empty streams will return null(0).  Ownership of the Pair is passed
   // to the caller (i.e., this routine does not unref() the pair and the caller should
   // not ref() the pair).
   Pair* get() {
      return (Pair*) List::get();
   }

   // Put 'pair' at the tail of the stream.  The Pair is referenced, ref(), by this routine.
   void put(Pair* pair1) {
      List::put((Object*)pair1);
   }

   // Removes 'pair' from this list and true is returned.  If 'pair' 'obj' is not found
   // then false is returned.  (Note: The Pair is unref() and therfore possible to delete)
   bool remove(Pair* pair1) {
      return List::remove((Object*)pair1);
   }

};

} // End Basic namespace
} // End Eaagles namespace

#endif
