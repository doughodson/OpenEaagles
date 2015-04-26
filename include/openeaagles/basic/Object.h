
#ifndef __Eaagles_Basic_Object_H__
#define __Eaagles_Basic_Object_H__

#include "openeaagles/basic/support.h"
#include "openeaagles/basic/macros.h"
#include "openeaagles/basic/SlotTable.h"
#include "openeaagles/basic/safe_ptr.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class: Object
//
// Description: Our 'system' or top level base class for all framework objects.
//
//       Provides a common base class for type checking and support for factory names,
//       slot tables, exceptions and reference counting.  Most of this needs to
//       be implemented by each derived class, and the macros in 'macros.h' provide
//       an easy way to do this.
//
//
// Macros
//
//    Macros are provided to generate much of the boilerplate code that is required
//    for each derived class (see macros.h).  The macro DECLARE_SUBCLASS() must be
//    in every derived class declaration, and the macro IMPLEMENT_SUBCLASS() must
//    be included with each class implementation.  Use IMPLEMENT_ABSTRACT_SUBCLASS()
//    for the implementation of abstract classes, and IMPLEMENT_PARTIAL_SUBCLASS()
//    if you need to write your own destructor, copy constructor, assignment operator
//    and clone function.
//
//
// Constructors and general class functions:
//
//    Each derived class must provide a standard constructor that requires no
//    parameters (e.g., Foo()).  This is required by the class factories (i.e., the
//    form functions) to constructor instances of derived classes.
//
//    The copy constructors, assignment operators (=), virtual destructors,
//    and clone functions are provided for all derived classes using the
//    macros and the user written functions, copyData and deleteData().
//
//    All constructors must include the STANDARD_CONSTRUCTOR() macro.
//
//    All newly constructed objects start with a reference count of one.
//
//       Foo::Foo()
//          Constructor; must be written for each derived class, or you can
//          use the EMPTY_CONSTRUCTOR() macro.
//
//       void copyData(const Foo& orig, const bool cc)
//
//          Declared in DECLARE_SUBCLASS(), and must be written for each derived
//          class, or use the EMPTY_COPYDATA() macro.
//
//          Copies data from the original object, 'orig', to our member variables
//          (e.g., copying the data, subcomponents, etc.).  Old, unused member
//          components need to be disposed of, as required.  The base class'
//          copyData() should be called BEFORE we copy any of our data.  The
//          copy constructor flag, 'cc', is true if we're being called by our
//          copy constructor, which indicates that all of our member data and
//          pointers are still undefined.  This flag should NOT be passed to
//          our base class's copyData().
//
//       void deleteData()
//
//          Declared in DECLARE_SUBCLASS(), and must be written for each derived
//          class, or use the EMPTY_COPYDATA() macro.
//
//          Disposes of our member data and components, as required.  Our
//          base class's deleteData() should be called AFTER we have disposed
//          of our data.  Warning: this function can  be called several times,
//          so data should be disposed of cleanly (e.g., test pointers for
//          zero before unreferencing objects and setting the pointers to zero
//          after the object has been unreferenced).
//
//       Foo::Foo(Foo& orig)
//          Copy constructor provided by the macros and uses copyData().
//
//       Foo::~Foo()
//          Class destructor provided by the macros and uses deleteData().
//
//       Object* clone()
//          Returns a copy of this object.  Provided by the macros and uses
//          the copy constructor.
//
//       "="
//          Assignment Operator is provided by the macros and requires copyData()
//
//       bool isClassType(const type_info& type)
//          Returns true if this object's class type is 'type' or if it is
//          derived from class 'type'. <defined by the macros>

//       const char* Foo::getClassName()
//          Static function that returns the full class name of class Foo.
//
//       bool isValid()
//          Returns true if this object is valid (i.e., all required slots/attributes
//          have been properly set).  This function is called by the parser to inform
//          the object that all calls to setSlotByName() have been completed.
//
//
// 'BaseClass' type
//
//    The DECLARE_SUBCLASS macro defines the type 'BaseClass', which is the class'
//    base class, and which should be used when calling base class member functions
//    instead of calling the functions with explicit class names.
//
//
// Reference counting:
//
//    Reference counting is a technique of counting the number of users of an
//    instantiated object.  It is used to delete objects that are no
//    longer referenced.  Two functions (ref(), unref() control the reference
//    count of an object.
//
//    Using ref() increments the reference count, which indicates another user
//    of the object.  When an object is created using 'new', a copy operator, copy
//    constructor or the clone() function, the reference count is initialized to
//    one (1) by the constructor (i.e., it's a pre-referenced object).
//
//    Using unref() decrements the reference count, which indicates one less
//    user.  If the number of references becomes zero, the object is deleted.
//    Beware - Do not use 'delete' to destroy an object; only use unref()!!!
//
//    Using getRefCount() returns the current value of the Object's reference count
//
//
// Factory classes and the factory names:
//
//    The user written factory classes (e.g., Factory) or functions will create an object
//    given its 'factory' name and will return a pointer to the new object or zero if the
//    object wasn't found.  These new objects have a reference count of one.  The
//    object's factory name, which is defined by a parameter to the IMPLEMENT_SUBCLASS()
//    macro, is usually the same or similar to the class name.
//
//    A static factory method or factory function is passed to the parser (see parser.y),
//    as a helper function to build an object tree.  The factory class or function is used
//    to construct objects using the factory names that were parsed from the input file.
//
//    Typically an application will build its own application level factory,
//    which in turn will call the various factory methods for the model libraries and
//    OpenEaagles libraries that the application requires.  By ordering these factory
//    calls at the application level, the factory can search a library containing
//    a few more specific models prior to searching a library of many generic models,
//    and if a more specific model exists for a given factory name then it is used,
//    otherwise the library of generic models is searched and its model, if found,
//    is used.
//
//    Factory name support functions --
//
//       bool isFactoryName(char* name)
//          Returns true if this object's factory name is 'name' or if it is
//          derived from a factory named 'name'.
//
//       const char* Foo::getFactoryName()
//          Static function that returns the factory name of class Foo.
//
//    Depreciated form name support functions (use factory support functions instead)
//
//       bool isFormName(char* name)
//          Returns true if this object's form name is 'name' or if it is
//          derived from a form named 'name'.
//
//       const char* Foo::getFormName()
//          Static function that returns the form name of class Foo.
//
//    (For examples of factory classes, see basic/Factory.cpp, basicGL/Factory.cpp,
//     simulation/Factory.cpp, as well as the various OpenEaagles examples)
//
//
// Slots and the Slot table:
//
//    Slot tables define the names of the slots (i.e., attributes) accepted by
//    the class of objects and maps these slot names to local slot index numbers.
//
//    Slot tables are defined by BEGIN_SLOTTABLE() and END_SLOTTABLE(),
//    or for an empty slot table, use EMPTY_SLOTTABLE().
//
//       // Class 'Foo' slot table
//       BEGIN_SLOTTABLE(Foo)
//          "slotA",             // index #1
//          "slotB",             // index #2
//          "slotC",             // index #3
//       END_SLOTTABLE(Foo)
//
//    The slot map, which is defined by the macros BEGIN_SLOT_MAP(), ON_SLOT()
//    and END_SLOT_MAP(), provides a mapping by local slot index number and
//    argument type to class member functions, 'set slot functions', that will
//    handle setting the class attribute.
//
//       // Class 'Foo' slot map
//       BEGIN_SLOT_MAP(Foo)
//           ON_SLOT( 1, setSlotA, List)     // setSlotA() handles "slotA" with a List type argument
//           ON_SLOT( 1, setSlotA, String)   // setSlotA() handles "slotA" with a String type argument
//           ON_SLOT( 2, setSlotB, Number)   // setSlotA() handles "slotB" with a Number type argument
//           ON_SLOT( 3, setSlotC, String)   // setSlotA() handles "slotC" with a String type argument
//       END_SLOT_MAP()
//
//    Each object instance has a slot table pointer that points to the class
//    slot table, and each class constructor must include the STANDARD_CONSTRUCTOR()
//    macro, which will set this slot table pointer.
//
//    Each slot table contains a pointer to its base class' slot table, and
//    slot names that are not found in a class slot table are passed up to
//    the base class' slot table.  Therefore, slot names are inherited by
//    derived classes and derived classes can override base class slot names.
//
//    Note: there are the SlotTable's index numbers and our local slot index
//    numbers.  As defined by the SlotTable class (see SlotTable.h), the first
//    slot of the first base class that contains a slot table is slot number one.
//    Whereas BEGIN_SLOT_MAP() remaps these to local slot numbers, which are
//    relative to our class (i.e. a local slot one), and these local number are
//    used by the ON_SLOT() macro.
//
//
// Object Serializer:
//
//    A serializer must be written for each derived class, or use EMPTY_SERIALIZER().
//
//    Serializes the object to 'sout' in the standard Eaagles Description
//    Language (EDL) format, which includes the object's factory name and slots.
//
//       serialize(ostream& sout)
//       serialize(ostream& sout, int indent)
//       serialize(ostream& sout, int indent, bool slotsOnly)
//          Serialize the contents of an object, as EDL, to the output stream, sout.
//          Each output line can be indented by 'indent' spaces.  If slotsOnly is true,
//          then only the slots are printed.
//
//       void indent(std::ostream& sout, const int ident)
//          Indents the output stream, 'sout, by 'ident' spaces.
//
//
// Standard message types (see definitions below):
//
//    By default, error and warning messages are enabled.
//
//    And by default, the other message types (Informational, Debug, Data,
//    User) are implicitly disabled (i.e., neither enabled or disabled
//    explicitly)(use std::cout).
//
//    Use isMessageEnabled() to test if a message type is enabled before
//    writing messages; except in 'slot' functions, which are usually
//    called by the parser and need to be written.
//
//       bool isMessageEnabled(const unsigned int msgTypeBit)
//          Returns true if this message type is enabled.
//
//       bool isMessageDisabled(const unsigned int msgTypeBit)
//          Returns true if this message type is disabled.
//
//       bool enableMessageTypes(const unsigned int msgTypeBits)
//          Enables these message types (bitwise OR of the message type bits).
//
//       bool disableMessageTypes(const unsigned int msgTypeBits)
//          Disables these message types (bitwise OR of the message type bits).
//          Note: the MSG_ERROR type message can not be disabled.
//
//
// Templates:
//
//    QQueue -- Quick Queue (see QQueue.h)
//       Use put() to add items and get() to remove items.  Use the constructor's
//       'qsize' parameter to set the size of the queue.
//
//    QStack -- Quick Stack (see QStack.h)
//       Use push() to add items and pop() to remove items.  Use the constructor's
//       'ssize' parameter to set the size of the stack.
//
//
// Exception:
//    Exception
//       Object's general exception class which returns a description.
//       The default description is "Unknown".
//
//
// Variables:
//    protected: const SlotTable* slotTable
//       Object slot table.  The Object constructor sets this variable to zero.
//       Class constructors must set this variable to their <class>::slottable.
//       (See macro STANDARD_CONSTRUCTOR())
//
//
// Table of known OpenEaagles classes and object counters
//
//    A list of 'known' OpenEaagles classes is maintained as a table of pointers
//    to the _Static structure, which is contained in each OpenEaagles class.  The
//    table is a private, static member variable.  The various IMPLEMENT_SUBCLASS
//    macros register the class using the registerClass() function; therefore all
//    OpenEaagles classes implemented in an application are known.
//
//    The STANDARD_CONSTRUCTOR() and STANDARD_DESTRUCTOR() macros increment and
//    decrement, respectively, a counter located in each class _Static structure.
//    This provides a count of the current number of instantiated objects for the
//    class.  The maximum number of this count, as well as the total number of
//    instantiated objects, is also maintained.
//
//       writeClassList(std::ostream& sout)
//          Writes the table of known OpenEaagles classes, which includes the
//          form name, object counters and full C++ class name for each class,
//          to the 'sout' output stream.
//
//------------------------------------------------------------------------------
class Object
{
   // -------------------------------------------------------------------------
   // Standard object stuff --
   //    derived classes will use the macro DECLARE_SUBCLASS(); see macros.h
   // -------------------------------------------------------------------------
   public: virtual ~Object();
   public: Object(const Object& org);
   public: Object& operator=(const Object& org);
   public: virtual Object* clone() const;
   protected: void copyData(const Object& org, const bool cc = false);
   protected: void deleteData();

   public: virtual bool isClassType(const std::type_info& type) const;
   public: virtual bool isFactoryName(const char name[]) const;
   public: virtual bool isFormName(const char name[]) const;               // depreciated
   public: static const char* getFactoryName();
   public: static const char* getFormName();                               // depreciated
   public: static const char* getClassName();
   public: virtual std::ostream& serialize(std::ostream& sout, const int i = 0, const bool slotsOnly = false) const;

   // Slot table
   protected: static const SlotTable slottable;    // class slot table
   private: static const char* slotnames[];        // slot names in this object's slot table
   private: static const int nslots;               // number of slots in this object's slot table

   // Slot table functions
   public: static const SlotTable& getSlotTable();
   protected: virtual bool setSlotByIndex(const int slotindex, Object* const obj);
   protected: virtual Object* getSlotByIndex(const int slotindex);
   public: bool setSlotByName(const char* const slotname, Object* const obj);
   public: Object* getSlotByName(const char* const slotname);
   public: const char* slotIndex2Name(const int slotindex) const;
   public: int slotName2Index(const char* const slotname) const;

   // Static member variables
   protected: struct _Static {
      const unsigned int classIndex;   // Registered class index
      const char* const cname;         // class name from 'type_info'
      const char* const fname;         // class form name
      const SlotTable* const st;       // Pointer to the SlotTable
      const _Static* const bstatic;    // Pointer to the base class _Static object
      int count;                       // NCurrent of instances
      int mc;                          // Max number of instances
      int tc;                          // Total number of instances
      _Static(const unsigned int, const char* const, const char* const, const SlotTable* const, const _Static* const);
      private:_Static& operator=(const _Static&);
   };
   private: static struct _Static _static;
   protected: static const _Static* getStatic(); //Get the _Static member

public:
   // Standard message types
   static const unsigned short MSG_ERROR   = 0x0001;  // Error messages; ALWAYS ENABLED (use std::cerr)
   static const unsigned short MSG_WARNING = 0x0002;  // Warning messages (use std::cerr)
   static const unsigned short MSG_INFO    = 0x0004;  // Informational messages (use std::cout)
   static const unsigned short MSG_DEBUG   = 0x0008;  // Debug/trace messages(use std::cout)
   static const unsigned short MSG_DATA    = 0x0010;  // Data messages(use std::cout)
   static const unsigned short MSG_USER    = 0x0020;  // User debug/trace messages(use std::cout)
   static const unsigned short MSG_STD_ALL = 0x00FF;  // Standard message types mask
   // Non-standard or user defined message are the high order bits (0xFF00)
   static const unsigned short MSG_ALL     = 0xFFFF;  // All message types mask

public:
   Object();

   virtual bool isValid() const;

   virtual bool isMessageEnabled(const unsigned short msgType) const;
   bool isMessageDisabled(const unsigned short msgType) const;
   bool enableMessageTypes(const unsigned short msgTypeBits);
   bool disableMessageTypes(const unsigned short msgTypeBits);

   // ref(), unref() and getRefCount()
   #include "openeaagles/basic/refCount.h"

   // Output the list of known Eaagles classes
   static void writeClassList(std::ostream& sout);

   // ---
   // General exception class
   // ---
   class Exception {
   public:
      Exception()                                { }
      virtual ~Exception()                       { }
      virtual const char* getDescription() const {
         return "Unknown";
      }
   };

   class ExpInvalidRefCount : public Exception {
   public:
      ExpInvalidRefCount() : Exception() {}
      const char* getDescription() const override {
         return "invalid reference count";
      }
   };

   class ExpInvalidRefCountDelete : public Exception {
   public:
      ExpInvalidRefCountDelete() : Exception() {}
      const char* getDescription() const override {
         return "deleting object with positive reference count";
      }
   };

protected:
   // Slot table for this object (set to the object's class slot table)
   const SlotTable* slotTable;

   // Indents the output steam by 'ident' spaces. (used with serialize())
   void indent(std::ostream& sout, const int ident) const;

   unsigned short getMessageEnableBits() const  { return enbMsgBits; }
   unsigned short getMessageDisableBits() const { return disMsgBits; }

   static unsigned int registerClass(const _Static* const a);

private:
   unsigned short enbMsgBits;       // Enabled message bits
   unsigned short disMsgBits;       // Disabled message bits
   mutable long semaphore;          // ref(), unref() semaphore
   mutable unsigned int refCount;   // reference count

   // Table of registered classes:
   // --- pointers to the static member structure, _Static
   static const unsigned int MAX_CLASSES = EAAGLES_CONFIG_MAX_CLASSES;
   static const _Static* classes[MAX_CLASSES];
   static unsigned int numClasses;
};

} // End Basic namespace
} // End Eaagles namespace

#endif

