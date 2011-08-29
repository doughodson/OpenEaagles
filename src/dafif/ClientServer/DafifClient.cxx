#include "DafifClient.h"

// Parameters used by query()
static const int maxRetry = 4;
static const float waitTime = 0.5f;


//------------------------------------------------------------------------------
// msgHandler() -- empty message handler for ASYNC
//------------------------------------------------------------------------------
void DsDafifClient::msgHandler()
{
}


//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
DsDafifClient::~DsDafifClient()
{
   delete[] respMsgBuffer;
}



//------------------------------------------------------------------------------
// query() -- sends the request to the server and waits for the reply
//------------------------------------------------------------------------------
int DsDafifClient::query()
{
   // default to bad response message
   nql = 0;
   dcRespMsgOk = 0;

   // send the request message to the server
   dsSendRequest( &reqMsg );

   int wait = true;
   int retry = 0;
   while (wait && retry < maxRetry) {

      // wait for a response message from the server
      dsSetTimer(waitTime);
      pause();
      dsSetTimer(0.0f);

      // get the response message
      int n = dsRecvResponse(respMsg, respMsgBuffSize);

      if (n <= 0) {
	 // retry when there was no response message
         dsSendRequest( &reqMsg );	// Timeout; resend
	 retry++;
      }
      else {
	 // check response message to make sure its the right one
         while (n > 0 && respMsg->seq != seq) {
            n = dsRecvResponse(respMsg, respMsgBuffSize);
         }
         if (n > 0 && respMsg->seq == seq) wait = 0; // we have it!
      }
   }

   if (!wait) {
      // When we did received a response message ...
      nql = respMsg->nrec;
      dcRespMsgOk = true;
   }

   // increment the sequence number for the next message
   seq++;

   return dcRespMsgOk;
}

