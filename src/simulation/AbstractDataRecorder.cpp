
#include "openeaagles/simulation/AbstractDataRecorder.hpp"

#include "openeaagles/simulation/SimExec.hpp"
#include "openeaagles/simulation/Station.hpp"

#include "openeaagles/base/List.hpp"
#include <iostream>

namespace oe {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(AbstractDataRecorder, "AbstractDataRecorder")
EMPTY_SLOTTABLE(AbstractDataRecorder)
EMPTY_SERIALIZER(AbstractDataRecorder)

AbstractDataRecorder::AbstractDataRecorder()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void AbstractDataRecorder::initData()
{
   sta = nullptr;
   sim = nullptr;
}

void AbstractDataRecorder::copyData(const AbstractDataRecorder& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   sta = nullptr;
   sim = nullptr;
}

void AbstractDataRecorder::deleteData()
{
   sta = nullptr;
   sim = nullptr;
}

//------------------------------------------------------------------------------
// Background thread processing of the data records
//------------------------------------------------------------------------------
void AbstractDataRecorder::processRecords()
{
   // nothing to do at this level
}

// Our parent Station
Station* AbstractDataRecorder::getStation()
{
   if (sta == nullptr) {
      getStationImp();
   }
   return sta;
}

// Our parent Station (const version)
const Station* AbstractDataRecorder::getStation() const
{
   if (sta == nullptr) {
      (const_cast<AbstractDataRecorder*>(this))->getStationImp();
   }
   return sta;
}

// Find our parent Station
Station* AbstractDataRecorder::getStationImp()
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
SimExec* AbstractDataRecorder::getSimulation()
{
   if (sim == nullptr) {
      getSimulationImp();
   }
   return sim;
}

const SimExec* AbstractDataRecorder::getSimulation() const
{
   if (sim == nullptr) {
      (const_cast<AbstractDataRecorder*>(this))->getSimulationImp();
   }
   return sim;
}

// The simulation
SimExec* AbstractDataRecorder::getSimulationImp()
{
   if (sim == nullptr) {
      Station* p = getStation();
      if (p != nullptr) sim = p->getSimulation();
   }
   return sim;
}


bool AbstractDataRecorder::recordDataImp(const unsigned int id, const base::Object* pObjects[4], const double values[4])
{
   return true;
}

}
}
