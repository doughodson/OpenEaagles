//==============================================================================
// KeithleyDeviceImp -- MS Windows implementation
//
//  FOR REFERENCE THE DL_SERVICEREQUEST STRUCTURE IS GIVEN BELOW
//
//  typedef struct DL_ServiceRequest {
//    /* Control info for 32-bit DriverLINX */
//    DWORD      dwSize;                /* size of Service Request structure */
//    /* Request Group */
//    HWND       hWnd;                  /* window handle requesting service */
//    WORD       device;                /* logical device to use */
//    SubSystems subsystem;             /* requested subsystem */
//    Modes      mode;                  /* subsystem operation mode */
//    Ops        operation;             /* requested operation */
//    /* Results Group */
//    WORD       result;                /* output: result code for operation */
//    WORD       taskId;                /* output: task id for operation */
//    DLSTATUS   status;                /* status/read output for request */
//    /* Events Group */
//    DLEVENT    timing;                /* timing event */
//    DLEVENT    start;                 /* starting event for request */
//    DLEVENT    stop;                  /* stopping event for request */
//    /* Select Group */
//    WORD       taskFlags;             /* critical section flags */
//    HINSTANCE  hDLL;                  /* DriverLINX DLL handle, 0 if not used*/
//    LPBUFFLIST lpBuffers;             /* I/O buffers for request */
//    CHANLIST   channels;              /* channels to read or write */
//  } DL_SERVICEREQUEST;
//  
//==============================================================================

#include "./KeithleyDeviceImp.h"

// Keithley data acq card driver
#include "driverLINX/DrvLINX.h"  // DriverLINX API
#include "driverLINX/dlcodes.h"  // DriverLINX error codes and macros

namespace Eaagles {
namespace IoDevice {

IMPLEMENT_SUBCLASS(KeithleyDeviceImp,"KeithleyDevice")
EMPTY_SLOTTABLE(KeithleyDeviceImp)
EMPTY_SERIALIZER(KeithleyDeviceImp)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
KeithleyDeviceImp::KeithleyDeviceImp()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

//------------------------------------------------------------------------------
// Init our data
//------------------------------------------------------------------------------
void KeithleyDeviceImp::initData()
{
   driverInstance = 0;
   analogServReq = 0;
   digitalServReq = 0;
   analogValues = 0;
   digitalValues = 0;

   driverOpen = false;
   openFailed = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void KeithleyDeviceImp::copyData(const KeithleyDeviceImp& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   // No copy -- just delete our the old stuff
   deleteData(); 
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void KeithleyDeviceImp::deleteData()
{
   if (driverInstance != 0) {
      // stop all active tasks and unload the DriverLINUX driver
      CloseDriverLINX(driverInstance);
      driverInstance = 0;
   }

   if (analogServReq != 0) {

      // see if buffer list has been created
      if (analogServReq->lpBuffers != 0) {
         // if the buffer list exists, is there actually a pointer to a buffer?
         if (analogServReq->lpBuffers->BufferAddr[0] != 0) {
            // de-allocate the buffer, and clear its pointer
            BufFree(analogServReq->lpBuffers->BufferAddr[0]);
            analogServReq->lpBuffers->BufferAddr[0] = 0;
         }
         // delete the buffer list, we don't need it anymore
         delete(analogServReq->lpBuffers); 
         analogServReq->lpBuffers = 0;
      }

      delete analogServReq;
      analogServReq = 0;
   }

   if (analogValues != 0) {
      delete [] analogValues;
      analogValues = 0;
   }

   if (digitalServReq != 0) {

      // see if buffer list has been created
      if(digitalServReq->lpBuffers != 0) {
         // if the buffer list exists, is there actually a pointer to a buffer?
         if(digitalServReq->lpBuffers->BufferAddr[0] != 0) {
            // de-allocate the buffer, and clear its pointer
            BufFree(digitalServReq->lpBuffers->BufferAddr[0]);
            digitalServReq->lpBuffers->BufferAddr[0] = 0;
         }
         // delete the buffer list, we don't need it anymore
         delete(digitalServReq->lpBuffers); 
         digitalServReq->lpBuffers = 0;
      }
   
      delete digitalServReq;
      digitalServReq = 0;
   }

   digitalValues = 0;

   driverOpen = false;
   openFailed = false;
}

//------------------------------------------------------------------------------
// reset() -- Reset (open) device
//------------------------------------------------------------------------------
void KeithleyDeviceImp::reset()
{
   BaseClass::reset();

   // open driver for KPCI-3101/2/3/4 or KPCI-3110/3116, driver name is kpci3100
   if ( !(driverOpen || openFailed) ) {

      // get instance handle that application can use to identify the DriverLINX driver
      driverInstance = OpenDriverLINX(GetDesktopWindow(),"kpci3100");

      // create a service request packet for initializing the driver
      DL_ServiceRequest* initServReq = createServiceRequest();

      // define the service request structure for initialize
      defineInitServReq(initServReq);

      // execute init service
      if (DriverLINX(initServReq) == NoErr) {
         driverOpen = true;
      }
      else {
         showMessage(initServReq);  // display what went wrong
         openFailed = true;
      }

      delete initServReq;
      initServReq = 0;

      if (driverOpen) {
         // the driver opened the card ok, so...

         // set the number of buttons and axes device supports
         numDI = NUM_DIGITAL_CHANNELS;
         if (numDI > MAX_DI) numDI = MAX_DI;

         numAI = NUM_ANALOG_CHANNELS;
         if (numAI > MAX_AI) numAI = MAX_AI;

         // create a service request packet for analog
         analogServReq = createServiceRequest();
         defineAnalogServReq(analogServReq);
         analogValues = new float[NUM_ANALOG_CHANNELS];

         // create a service request packet for digital
         digitalServReq = createServiceRequest();
         defineDigitalServReq(digitalServReq);
      }
   }
}

//------------------------------------------------------------------------------
// Go get our AIs and DIs here
//------------------------------------------------------------------------------
void KeithleyDeviceImp::processInputs(const Eaagles::LCreal dt, Basic::IoData* const pInData)
{

   if (driverOpen) {

      // ---
      // DIGITAL OPERATIONS
      // ---
      if (numDI > 0 && digitalValues != 0) {

         // execute digital service request
         DriverLINX(digitalServReq);

         unsigned int idx = 0;
         unsigned int ibit = 0;
         unsigned int ibyte = 0;
         while (idx < numDI) {
            // Test the bit within a byte
            inBits[idx++] = (digitalValues[ibyte] & (1 << ibit)) == 0;

            // Inc bit and byte counts
            ibit++;
            if (ibit >= 8) {
               ibit = 0;
               ibyte++;
            }
         }
      }

      // ---
      // ANALOG OPERATIONS
      // ---
      if (numAI > 0 && analogValues != 0) {

         // execute analog service request (read all analog channels)
         cfgAnalogRead(analogServReq);
         DriverLINX(analogServReq);

         // configure analog service request to convert received data
         cfgAnalogConvert(analogServReq);
         DriverLINX(analogServReq);

         // store results
         for(unsigned int i = 0; i < numAI; i++) {
            inData[i] = (float)(analogValues[i]/10.0f);
         }
      }

   }

   // Update our base class, which will call our component DI handlers
   BaseClass::processInputs(dt,pInData);
}

//------------------------------------------------------------------------------
// data access functions
//------------------------------------------------------------------------------
void KeithleyDeviceImp::showMessage(DL_ServiceRequest *ServReq)
{
	ServReq->operation=MESSAGEBOX;
	DriverLINX(ServReq);
	return;
}

DL_ServiceRequest* KeithleyDeviceImp::createServiceRequest()
{
  // create a service request structure
  DL_ServiceRequest* ServReq = (DL_ServiceRequest*) new DL_ServiceRequest;

  // reset the members of the service request to defaults
  memset(ServReq,0,sizeof(DL_ServiceRequest));

  // need to set the service request size member
  DL_SetServiceRequestSize(*ServReq);

  return ServReq;
}

void KeithleyDeviceImp::defineInitServReq(DL_ServiceRequest* ServReq)
{
  // initialize device - this takes care of both digital and analog subsystems
  // set device number
  ServReq->device = 0;

  // need to init before we can use
  ServReq->operation = INITIALIZE;

  // init the "DEVICE" subsystem
  ServReq->subsystem = DEVICE;

  // init is not a polled, interrupt, or dma operation
  ServReq->mode = OTHER;

  // window handle of app, set to desktop
  ServReq->hWnd = GetDesktopWindow();

  // return service request configured to perform driver initialization
  return;
}

void KeithleyDeviceImp::defineAnalogServReq(DL_ServiceRequest* ServReq)
{
  //
  // setup a multi channel, polled AI task
  //
  // request group settings
  //
  ServReq->hWnd = GetDesktopWindow();
  ServReq->device = 0;
  ServReq->subsystem = AI;
  ServReq->mode = POLLED;
  ServReq->operation = START;

  //
  // results group settings
  // -none specified
  //
  // events group settings
  //
  // these are all DLEVENT structures
  ServReq->timing.typeEvent = NULLEVENT;
  ServReq->start.typeEvent = COMMAND;
  ServReq->stop.typeEvent = TCEVENT;

  //
  // select group setttings
  //
  // start and stop channel range
  ServReq->channels.nChannels = 2;
  ServReq->channels.numberFormat = tNATIVE;

  // chanGain specifies the starting and ending channels and gain
  // start on channel 0, stop on channel NUM_ANALOG_CHANNELS-1
  ServReq->channels.chanGain[0].channel = 0;
  ServReq->channels.chanGain[1].channel = NUM_ANALOG_CHANNELS-1;

  // setup channel gains using Gain2Code(device,subsystem,gain)
  // negative gain means bipolar gain, positive mean unipolar
  ServReq->channels.chanGain[0].gainOrRange = Gain2Code(0,AI,-1);
  ServReq->channels.chanGain[1].gainOrRange = Gain2Code(0,AI,-1);

  // need a buffer for data
  // polled mode tasks can scan the channels one time
  // the buffer MUST be a multiple of the channels
  // create a buffer list pointer for one buffer
  ServReq->lpBuffers = (DL_BUFFERLIST*) new BYTE[DL_BufferListBytes(1)];

  // set number of buffers in the list
  ServReq->lpBuffers->nBuffers = 1;
  // specify the size of each buffer in bytes
  // Sample2Bytes converts the given number of samples to bytes
  // Sample2Bytes(device,subsystem,channel,samples)
  ServReq->lpBuffers->bufferSize = Samples2Bytes(0,AI,0,NUM_ANALOG_CHANNELS);

  // specify the memory address of each of the nBuffers
  // allocate memory suitable for POLLED data acquisition tasks
  ServReq->lpBuffers->BufferAddr[0] = BufAlloc(GBUF_POLLED, ServReq->lpBuffers->bufferSize);

  //
  // fill out DATA_CONVERT structure
  //
  // specify index of the data-acquisition buffer in the Buffer List (DL_BUFFERLIST)
  ServReq->start.u.dataConvert.wBuffer = 0;

  // specify the index of the starting sample in the data-acquisition buffer to convert
  ServReq->start.u.dataConvert.startIndex = 0;

  // specify the number of samples in the data-acquisition buffer to convert
  ServReq->start.u.dataConvert.nSamples = NUM_ANALOG_CHANNELS;

  // point to the applications data buffer
  ServReq->start.u.dataConvert.lpBuffer = analogValues;

  // specifies the data format of the applications data buffer
  ServReq->start.u.dataConvert.numberFormat = tSINGLE;

  // specifies a factor by which every converted sample will be multiplied
  // 0.0 means that it is ignored
  ServReq->start.u.dataConvert.scaling = 0.0f;

  // specifies a term which will be added to every converted sample
  ServReq->start.u.dataConvert.offset = 0.0f;

  return;
}

void KeithleyDeviceImp::cfgAnalogRead(DL_ServiceRequest* ServReq)
{
  // given a service request already defined for analog operation
  // we specify exactly what kind of operation to do
  // in this case, we want to read inputs
  ServReq->mode = POLLED;
  ServReq->operation = START;
  ServReq->start.typeEvent = COMMAND;

  return;
}

void KeithleyDeviceImp::cfgAnalogConvert(DL_ServiceRequest* ServReq)
{
  // given a service request already defined for analog operation
  // we specify that we want to do a data conversion on inputs
  // already read
  // convert is not a polled, interrupt, or DMA operation
  ServReq->mode = OTHER;

  // use the convert operation to convert the raw counts in the buffer to voltages
  ServReq->operation = CONVERT;

  // set the start type to convert the data
  ServReq->start.typeEvent = DATACONVERT;

  return;
}

void KeithleyDeviceImp::defineDigitalServReq(DL_ServiceRequest* ServReq)
{
   //
   // read all digital channels
   //

   //
   // request group settings
   //
   ServReq->hWnd = GetDesktopWindow();
   ServReq->device = 0;
   ServReq->subsystem = DI;
   ServReq->mode = POLLED;
   ServReq->operation = START;

   //
   // results group setttings
   // -none specified
   //
   // events group settings
   //
   // these are all DLEVENT structures
   ServReq->timing.typeEvent = NULLEVENT;
   ServReq->start.typeEvent = COMMAND;
   ServReq->stop.typeEvent = TCEVENT;

   //
   // select group setttings
   //
   ServReq->channels.nChannels = 2;
   ServReq->channels.numberFormat = tNATIVE;
   ServReq->channels.chanGain[0].channel = 0;
   ServReq->channels.chanGain[1].channel = NUM_DIGITAL_CHANNELS-1;
   ServReq->lpBuffers = (DL_BUFFERLIST*) new BYTE[DL_BufferListBytes(1)];

   // create a list of buffers; in this case just one
   ServReq->lpBuffers->nBuffers = 1;
   ServReq->lpBuffers->bufferSize = Samples2Bytes(0,DI,0,NUM_DIGITAL_CHANNELS);

   // allocate enough memory in the buffer for the number of samples
   ServReq->lpBuffers->BufferAddr[0] = BufAlloc(GBUF_INT, ServReq->lpBuffers->bufferSize);

   // point at the data buffer so we can extract values
   digitalValues = (BYTE*)ServReq->lpBuffers->BufferAddr[0];

   return;
}


} // IoDevice namespace
} // end Eaagles namespace
