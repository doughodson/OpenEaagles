//------------------------------------------------------------------------------
// Classes ---
//    Base function:                      Function
//    Polynomial function:                Polynomial
//    Generic multi-variable functions:   Func1, Func2, Func3, Func4 and Func5
//    Funtion storage:                    FStorage
//------------------------------------------------------------------------------

#include "openeaagles/basic/Functions.h"

#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Tables.h"

namespace Eaagles {
namespace Basic {

//==============================================================================
// class FStorage
//==============================================================================

IMPLEMENT_SUBCLASS(FStorage,"FStorage")
EMPTY_SLOTTABLE(FStorage)
EMPTY_CONSTRUCTOR(FStorage)
EMPTY_COPYDATA(FStorage)
EMPTY_DELETEDATA(FStorage)
EMPTY_SERIALIZER(FStorage)

//==============================================================================
// Class: Function
//==============================================================================
IMPLEMENT_SUBCLASS(Function, "Function")

BEGIN_SLOTTABLE(Function)
    "table",          // Optional LFI table
END_SLOTTABLE(Function)

BEGIN_SLOT_MAP(Function)
    ON_SLOT( 1, setSlotLfiTable, Table)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
Function::Function()
{
   STANDARD_CONSTRUCTOR()

   table = 0;
}

void Function::copyData(const Function& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) table = 0;

   {
      const Table* copy = 0;
      if (org.table != 0) {
         copy = (const Table*) org.table->clone();
      }
      setSlotLfiTable(copy);
      if (copy != 0) copy->unref();
   }
}

void Function::deleteData()
{
   setSlotLfiTable(0);
}

//------------------------------------------------------------------------------
// Storage factory
//------------------------------------------------------------------------------
FStorage* Function::storageFactory() const
{
   // Since no derived class handled this ...
   if (table != 0) {
      // then we're going to let the table handle it, if any 
      return table->storageFactory();
   }
   else {
      // or we're just going to return a standard FStorage object.
      return new FStorage();
   }
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool Function::setSlotLfiTable(const Table* const msg)
{
   table = msg;
   return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() 
//------------------------------------------------------------------------------
Object* Function::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Function::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if (!slotsOnly) {
      sout << "( " << getFormName() << std::endl;
      j = 4;
   }

   if (table != 0) {
      indent(sout, i + j);
      sout << "table: ";
      table->serialize(sout,(i+j+4));
   }

   BaseClass::serialize(sout, i + j, true);

   if (!slotsOnly) {
      indent(sout, i);
      sout << ")" << std::endl;
   }

   return sout;
}

//==============================================================================
// Class: Func1 
//==============================================================================
IMPLEMENT_SUBCLASS(Func1,"Func1")
EMPTY_SLOTTABLE(Func1)
EMPTY_CONSTRUCTOR(Func1)
EMPTY_COPYDATA(Func1)
EMPTY_DELETEDATA(Func1)
EMPTY_SERIALIZER(Func1)

double Func1::f(const double iv1, FStorage* const s) const
{
   double value = 0;

   // No derived class handled this ...

   const Table1* p = (const Table1*) getTable();
   if (p != 0) {
      // But we do have an optional table that'll handle it.
      value = p->lfi( LCreal(iv1),s);
   }

   return value;
}

bool Func1::setSlotLfiTable(const Table* const msg)
{
   bool ok = false;
   if (msg == 0) {
      ok = BaseClass::setSlotLfiTable(0);    // Setting the table to null.
   }
   else if ( msg->isClassType(typeid(Table1)) ) {
      ok = BaseClass::setSlotLfiTable(msg); // We have a 1-D table.
   }
   else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Func1::setSlotLfiTable(): ERROR -- table must use a Table1 or derived class" << std::endl;
   }
   return ok;
}

//==============================================================================
// Class: Func2 
//==============================================================================
IMPLEMENT_SUBCLASS(Func2,"Func2")
EMPTY_SLOTTABLE(Func2)
EMPTY_CONSTRUCTOR(Func2)
EMPTY_COPYDATA(Func2)
EMPTY_DELETEDATA(Func2)
EMPTY_SERIALIZER(Func2)

double Func2::f(const double iv1, const double iv2, FStorage* const s) const
{
   double value = 0;

   // No derived class handled this ...

   const Table2* p = (const Table2*) getTable();
   if (p != 0) {
      // But we do have an optional table that'll handle it.
      value = p->lfi(LCreal(iv1), LCreal(iv2), s);
   }

   return value;
}

bool Func2::setSlotLfiTable(const Table* const msg)
{
   bool ok = false;
   if (msg == 0) {
      ok = BaseClass::setSlotLfiTable(0);    // Setting the table to null.
   }
   else if ( msg->isClassType(typeid(Table2)) ) {
      ok = BaseClass::setSlotLfiTable(msg);  // We have a 2-D table.
   }
   else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Func2::setSlotLfiTable(): ERROR -- table must use a Table2 or derived class" << std::endl;
   }
   return ok;
}

//==============================================================================
// Class: Func3 
//==============================================================================
IMPLEMENT_SUBCLASS(Func3,"Func3")
EMPTY_SLOTTABLE(Func3)
EMPTY_CONSTRUCTOR(Func3)
EMPTY_COPYDATA(Func3)
EMPTY_DELETEDATA(Func3)
EMPTY_SERIALIZER(Func3)

double Func3::f(const double iv1, const double iv2, const double iv3, FStorage* const s) const
{
   double value = 0;

   // No derived class handled this ...

   const Table3* p = (const Table3*) getTable();
   if (p != 0) {
      // But we do have an optional table that'll handle it.
      value = p->lfi(LCreal(iv1), LCreal(iv2), LCreal(iv3), s);
   }

   return value;
}

bool Func3::setSlotLfiTable(const Table* const msg)
{
   bool ok = false;
   if (msg == 0) {
      ok = BaseClass::setSlotLfiTable(0);    // Setting the table to null.
   }
   else if ( msg->isClassType(typeid(Table3)) ) {
      ok = BaseClass::setSlotLfiTable(msg);  // We have a 3-D table.
   }
   else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Func3::setSlotLfiTable(): ERROR -- table must use a Table3 or derived class" << std::endl;
   }
   return ok;
}

//==============================================================================
// Class: Func4
//==============================================================================
IMPLEMENT_SUBCLASS(Func4,"Func4")
EMPTY_SLOTTABLE(Func4)
EMPTY_CONSTRUCTOR(Func4)
EMPTY_COPYDATA(Func4)
EMPTY_DELETEDATA(Func4)
EMPTY_SERIALIZER(Func4)

double Func4::f(const double iv1, const double iv2, const double iv3, const double iv4, FStorage* const s) const
{
   double value = 0;

   // No derived class handled this ...

   const Table4* p = (const Table4*) getTable();
   if (p != 0) {
      // But we do have an optional table that'll handle it.
      value = p->lfi(LCreal(iv1), LCreal(iv2), LCreal(iv3), LCreal(iv4), s);
   }

   return value;
}

bool Func4::setSlotLfiTable(const Table* const msg)
{
   bool ok = false;
   if (msg == 0) {
      ok = BaseClass::setSlotLfiTable(0);    // Setting the table to null.
   }
   else if ( msg->isClassType(typeid(Table4)) ) {
      ok = BaseClass::setSlotLfiTable(msg);  // We have a 4-D table.
   }
   else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Func4::setSlotLfiTable(): ERROR -- table must use a Table4 or derived class" << std::endl;
   }
   return ok;
}

//==============================================================================
// Class: Func5
//==============================================================================
IMPLEMENT_SUBCLASS(Func5,"Func5")
EMPTY_SLOTTABLE(Func5)
EMPTY_CONSTRUCTOR(Func5)
EMPTY_COPYDATA(Func5)
EMPTY_DELETEDATA(Func5)
EMPTY_SERIALIZER(Func5)

double Func5::f(const double iv1, const double iv2, const double iv3, const double iv4, const double iv5, FStorage* const s) const
{
   double value = 0;

   // No derived class handled this ...

   const Table5* p = (const Table5*) getTable();
   if (p != 0) {
      // But we do have an optional table that'll handle it.
      value = p->lfi(LCreal(iv1), LCreal(iv2), LCreal(iv3), LCreal(iv4), LCreal(iv5) ,s);
   }

   return value;
}

bool Func5::setSlotLfiTable(const Table* const msg)
{
   bool ok = false;
   if (msg == 0) {
      ok = BaseClass::setSlotLfiTable(0);    // Setting the table to null.
   }
   else if ( msg->isClassType(typeid(Table5)) ) {
      ok = BaseClass::setSlotLfiTable(msg);  // We have a 5-D table.
   }
   else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Func5::setSlotLfiTable(): ERROR -- table must use a Table5 or derived class" << std::endl;
   }
   return ok;
}

//==============================================================================
// Class: Polynomial
//==============================================================================
IMPLEMENT_SUBCLASS(Polynomial, "Polynomial")

BEGIN_SLOTTABLE(Polynomial)
    "coefficients"   // 1) Constant coefficients vector: [ a0 a1 a2 ... aN ]
END_SLOTTABLE(Polynomial)

BEGIN_SLOT_MAP(Polynomial)
    ON_SLOT( 1, setSlotCoefficients, List)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
Polynomial::Polynomial()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void Polynomial::initData()
{
   m = 0;
   for (unsigned int i = 0; i < MAX_COEFF; i++) {
      a[i] = 0;
   }
}

void Polynomial::copyData(const Polynomial& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   setCoefficients(org.a,org.m);
}

void Polynomial::deleteData()
{
   setCoefficients(0,0);
}

//------------------------------------------------------------------------------
// The polynomial function
//------------------------------------------------------------------------------
double Polynomial::f(const double x, FStorage* const) const
{
   double result = 0;
   if (a != 0 && m > 0) {
      unsigned int n = (m-1);
      double xx = 1.0;
      for (unsigned int i = 0; i <= n; i++) {
         result += (a[i] * xx);
         xx *= x;
      }
   }
   return result;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
bool Polynomial::setCoefficients(const double* const aa, const unsigned short mm)
{
   bool ok = false;

   // Clear the coefficients
   if (aa == 0 || mm == 0) {
      for (unsigned int i = 0; i < mm; i++) {
         a[i] = 0;
      }
      m = 0;
      ok = true;
   }

   // Copy the coefficients
   else if (mm <= MAX_COEFF) {
      for (unsigned int i = 0; i < mm; i++) {
         a[i] = aa[i];
      }
      m = mm;
      ok = true;
   }

   else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Polynomial::setSlotCoefficients(): ERROR; too many coefficients; max is " << (MAX_DEGREE+1) << std::endl;
   }

   return ok;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool Polynomial::setSlotCoefficients(const List* const msg)
{
   bool ok = false;

   if (msg != 0) {

      unsigned int entries = msg->entries();
      if ( entries <= MAX_COEFF ) {
         double  aa[MAX_COEFF];   // Constant coefficients vector
         int mm = msg->getNumberList(aa, MAX_COEFF);
         ok = setCoefficients( aa, (unsigned short) mm );
      }

      else if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Polynomial::setSlotCoefficients(): ERROR; too many coefficients; max is " << MAX_COEFF << std::endl;
      }

   }
   else {
      // Just remove the old ...
      ok = setCoefficients(0,0);
   }

   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() 
//------------------------------------------------------------------------------
Object* Polynomial::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Polynomial::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if (!slotsOnly) {
      sout << "( " << getFormName() << std::endl;
      j = 4;
   }

   int mm = getDegree() + 1;
   if (mm > 0) {
      const double* aa = getCoefficients();
      indent(sout,i+j);
      sout << "coefficients: [ ";
      for (int i = 0; i < mm; i++) {
         std::cout << aa[i] << " ";
      }
      sout << " ]" << std::endl;
   }

   BaseClass::serialize(sout, i + j, true);

   if (!slotsOnly) {
      indent(sout, i);
      sout << ")" << std::endl;
   }

   return sout;
}


} // End Basic namespace
} // End Eaagles namespace
