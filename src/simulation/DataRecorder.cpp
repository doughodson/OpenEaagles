//------------------------------------------------------------------------------
// Classes:
//    DataRecorder      -- Abstract data recorder class
//    RecorderComponent -- Base class for all data recorder components
//------------------------------------------------------------------------------

#include "openeaagles/simulation/DataRecorder.h"

#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Station.h"

#include "openeaagles/basic/List.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class DataRecorder
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(DataRecorder,"Abstract_DataRecorder")
EMPTY_SLOTTABLE(DataRecorder)
EMPTY_SERIALIZER(DataRecorder)

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
DataRecorder::DataRecorder()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void DataRecorder::initData()
{
   sta = 0;
   sim = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void DataRecorder::copyData(const DataRecorder& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   sta = 0;
   sim = 0;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void DataRecorder::deleteData()
{
   sta = 0;
   sim = 0;
}

//------------------------------------------------------------------------------
// Background thread processing of the data records
//------------------------------------------------------------------------------
void DataRecorder::processRecords()
{
   // nothing to do at this level
}

// Our parent Station
Station* DataRecorder::getStation()
{
   if (sta == 0) {
      sta = (Station*) findContainerByType(typeid(Station));
      if (sta == 0 && isMessageEnabled(MSG_ERROR)) {
         std::cerr << "DataRecorder::getStation(): ERROR, unable to locate the Station class!" << std::endl;
      }
   }
   return sta;
}

// Our parent Station (const version)
const Station* DataRecorder::getStation() const
{
   if (sta != 0) {
      return sta;
   }
   else {
      // Yes this is a "const cast-away", but it is the non-const version
      // that initially finds our Station class.
      DataRecorder* p = (DataRecorder*) this;
      Station* s = p->getStation();
      return (const Station*) s;
   }
}

// The simulation
Simulation* DataRecorder::getSimulation()
{
   if (sim == 0) {
      Station* p = getStation();
      if (p != 0) sim = p->getSimulation();
   }
   return sim;
}

const Simulation* DataRecorder::getSimulation() const
{
   if (sim != 0) {
      return sim;
   }
   else {
      // Yes this is a "const cast-away", but its the non-const version
      // that initially finds our Simulation class.
      DataRecorder* p = (DataRecorder*) this;
      Simulation* s = p->getSimulation();
      return (const Simulation*) s;
   }
}

bool DataRecorder::recordDataImp(const unsigned int id, const Basic::Object* pObjects[4], const double values[4])
{
   return true;
}

//==============================================================================
// Class RecorderComponent
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(RecorderComponent,"RecorderComponent")

// Slot table for this form type
BEGIN_SLOTTABLE(RecorderComponent)
    "enabledList",         // 1)  List of enabled data records
    "disabledList",        // 2)  List of disabled data records
END_SLOTTABLE(RecorderComponent)

// Map slot table to handles 
BEGIN_SLOT_MAP(RecorderComponent)
    ON_SLOT( 1, setSlotEnabledList,  Basic::List)
    ON_SLOT( 2, setSlotDisabledList, Basic::List)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
RecorderComponent::RecorderComponent()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void RecorderComponent::initData()
{
   enabledList = 0;
   numEnabled = 0;

   disabledList = 0;
   numDisabled = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void RecorderComponent::copyData(const RecorderComponent& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   setEnabledList(org.enabledList, org.numEnabled);
   setDisabledList(org.disabledList, org.numDisabled);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void RecorderComponent::deleteData()
{
   setEnabledList(0,0);
   setDisabledList(0,0);
}

//------------------------------------------------------------------------------
// Set a list of 'n' of data records enabled for processing
//------------------------------------------------------------------------------
bool RecorderComponent::setEnabledList(const unsigned int* const list, const unsigned int n)
{
   // Remove the old list
   if (enabledList != 0) {
      delete[] enabledList;
      enabledList = 0;
   }
   numEnabled = 0;

   // Create the new list
   if (list != 0 && n > 0) {
      enabledList = new unsigned int[n];
      for (unsigned int i = 0; i < n; i++) {
         enabledList[i] = list[i];
      }
      numEnabled = n;
   }

   return true;
}


//------------------------------------------------------------------------------
// Set a list of 'n' of data records disabled from being processed
//------------------------------------------------------------------------------
bool RecorderComponent::setDisabledList(const unsigned int* const list, const unsigned int n)
{
   // Remove the old list
   if (disabledList != 0) {
      delete[] disabledList;
      disabledList = 0;
   }
   numDisabled = 0;

   // Create the new list
   if (list != 0 && n > 0) {
      disabledList = new unsigned int[n];
      for (unsigned int i = 0; i < n; i++) {
         disabledList[i] = list[i];
      }
      numDisabled = n;
   }

   return true;
}


//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool RecorderComponent::setSlotEnabledList(const Basic::List* const list)
{
   unsigned int n = list->entries();

   // Items in the list ...
   if (n > 0) {
      // get the numbers from the list and set them as the filter list
      unsigned int* p = new unsigned int[n];
      unsigned int n2 = list->getNumberList((int*)p, n);
      setEnabledList(p, n2);
      delete[] p;
   }

   // Empty list -- 
   else {
      setEnabledList(0,0);
   }

   return true;
}

bool RecorderComponent::setSlotDisabledList(const Basic::List* const list)
{
   unsigned int n = list->entries();

   // Items in the list ...
   if (n > 0) {
      // get the numbers from the list and set them as the filter list
      unsigned int* p = new unsigned int[n];
      unsigned int n2 = list->getNumberList((int*)p, n);
      setDisabledList(p, n2);
      delete[] p;
   }

   // Empty list -- 
   else {
      setDisabledList(0,0);
   }

   return true;
}


//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
Basic::Object* RecorderComponent::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& RecorderComponent::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      //indent(sout,i);
      sout << "( " << getFormName() << std::endl;
      j = 4;
   }

   // Enabled list -- printing as numeric values only
   if (numEnabled > 0 && enabledList != 0) {
      indent(sout, i + j);
      sout << "enabledList: " << "[";
      for (unsigned int i = 0; i < numEnabled; i++) sout << " " << enabledList[i];
      sout << " ]" << std::endl;
   }

   // Disabled list -- printing as numeric values only
   if (numDisabled > 0 && disabledList != 0) {
      indent(sout, i + j);
      sout << "disabledList: " << "[";
      for (unsigned int i = 0; i < numDisabled; i++) sout << " " << disabledList[i];
      sout << " ]" << std::endl;
   }

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

} // End Simulation namespace
} // End Eaagles namespace

