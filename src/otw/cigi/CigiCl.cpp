
#include "openeaagles/otw/cigi/CigiCl.hpp"

#include "cigicl/CigiHostSession.h"

namespace oe {
namespace otw {

IMPLEMENT_ABSTRACT_SUBCLASS(CigiCl, "CigiClAbstract")
EMPTY_SLOTTABLE(CigiCl)
EMPTY_SERIALIZER(CigiCl)

//------------------------------------------------------------------------------
// Parameters
//------------------------------------------------------------------------------
static const int MAX_BUF_SIZE = 1472;
static const double LOS_REQ_TIMEOUT = 2.0;     // one second timeout

bool CigiCl::cigiInitialized = false;    // CIGI has been initialized
bool CigiCl::cigiInitFailed  = false;    // CIGI initialization has failed

CigiCl::CigiCl()
{
   STANDARD_CONSTRUCTOR()

   session = nullptr;
   otwCigi = nullptr;
}

void CigiCl::copyData(const CigiCl& org, const bool)
{
   BaseClass::copyData(org);

   session = nullptr;
   otwCigi = nullptr;  // don't unref();
}

void CigiCl::deleteData()
{
   if (session != nullptr) {
      delete session;
   }
   session = nullptr;
   otwCigi = nullptr;  // don't unref();
}

//------------------------------------------------------------------------------
// initialize() -- Initialize this CIGI session
//------------------------------------------------------------------------------
bool CigiCl::initialize(OtwCigiCl* const p)
{
   // Our parent OTW CIGI handler
   // (no need to ref(), it owns us and it's not going to be deleted before us)
   otwCigi = p;

   // ---
   // Init the static CIGI system (only once for all instances)
   // ---
   if (!cigiInitialized && !cigiInitFailed) {
      cigiInitialized = initCigi();
      cigiInitFailed = !cigiInitialized;
   }

   // ---
   // Create the session
   // ---
   if (cigiInitialized) {
      session = new CigiHostSession(4, MAX_BUF_SIZE, 4, MAX_BUF_SIZE);
      //session->SetCigiVersion(2);     // CGB only force this if necessary
      // CGBCGB ??? session->SetSynchronous(p->isSyncMode());
   }

   return session != nullptr;
}

//------------------------------------------------------------------------------
// True if we've been successfully initialized
//------------------------------------------------------------------------------
bool CigiCl::isInitialized()
{
   return cigiInitialized && (session != nullptr);
}

//------------------------------------------------------------------------------
// initCigi() -- Initialize the static CIGI system
//------------------------------------------------------------------------------
bool CigiCl::initCigi()
{
   std::cout << "CigiCl::initCigi(): CIGI initialized!" << std::endl;
   return true;
}

//------------------------------------------------------------------------------
// R/T frame sync -- called from OtwCigiCl::frameSync() in the R/T thread
//------------------------------------------------------------------------------
void CigiCl::frameSync()
{
}

//------------------------------------------------------------------------------
// Background frame sync -- called from OtwCigiCl::updateData() in the background
//------------------------------------------------------------------------------
void CigiCl::frameBg()
{
}

}
}
