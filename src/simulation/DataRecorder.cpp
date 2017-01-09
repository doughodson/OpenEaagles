
#include "openeaagles/simulation/DataRecorder.hpp"

#include "openeaagles/simulation/SimExec.hpp"
#include "openeaagles/simulation/Station.hpp"

#include "openeaagles/base/List.hpp"
#include <iostream>

namespace oe {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(DataRecorder, "Abstract_DataRecorder")
EMPTY_SLOTTABLE(DataRecorder)
EMPTY_SERIALIZER(DataRecorder)

DataRecorder::DataRecorder()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void DataRecorder::initData()
{
   sta = nullptr;
   sim = nullptr;
}

void DataRecorder::copyData(const DataRecorder& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   sta = nullptr;
   sim = nullptr;
}

void DataRecorder::deleteData()
{
   sta = nullptr;
   sim = nullptr;
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
   if (sta == nullptr) {
      getStationImp();
   }
   return sta;
}

// Our parent Station (const version)
const Station* DataRecorder::getStation() const
{
   if (sta == nullptr) {
      (const_cast<DataRecorder*>(this))->getStationImp();
   }
   return sta;
}

// Find our parent Station
Station* DataRecorder::getStationImp()
{
   if (sta == nullptr) {
      sta = static_cast<Station*>(findContainerByType(typeid(Station)));
      if (sta == nullptr && isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Datarecorder::getStationImp(): ERROR, unable to locate the Station class!" << std::endl;
      }
   }
   return sta;
}

// The simulation
SimExec* DataRecorder::getSimulation()
{
   if (sim == nullptr) {
      getSimulationImp();
   }
   return sim;
}

const SimExec* DataRecorder::getSimulation() const
{
   if (sim == nullptr) {
      (const_cast<DataRecorder*>(this))->getSimulationImp();
   }
   return sim;
}

// The simulation
SimExec* DataRecorder::getSimulationImp()
{
   if (sim == nullptr) {
      Station* p = getStation();
      if (p != nullptr) sim = p->getSimulation();
   }
   return sim;
}


bool DataRecorder::recordDataImp(const unsigned int id, const base::Object* pObjects[4], const double values[4])
{
   return true;
}

}
}
