//------------------------------------------------------------------------------
// Class:	Message
// enum:    AckCodes
// Base class: Basic::Object -> Message
//
// Description: 'Generic' Eaagles Message. Used as a base class for other generic messages
//
// Slots:
//      none
// Events:
//      none
////get functions
//  Eaagles::Basic::String* getSenderName()                       { return senderName;   }
//      Description: returns the message senders name
//  Eaagles::Basic::List* getDestNames()                          { return destNames;     }
//      Description: returns the list of destination name strings
//  double getTimeStamp()                   { return timeStamp;     }
//      Description: returns the time at which this message was sent (if on receive end)
//  double getLifeSpan()                    { return lifeSpan;      }
//      Description: returns the length of time in seconds which this message is valid
//  AckCodes getAckCode()                   { return ack;           }
//      Description: returns the ack code for this message
//
////set functions
//  void setSenderName(Eaagles::Basic::String* sName)   
//      Description: sets the sender name of this message to the value passed in as a parameter
//  void setSenderName(const char* const sName)           
//      Description: sets to sender name of this message to the value passed in as a parameter
//      Pre: sName should be null terminated
//  void setTimeStamp(double ts)            { timeStamp = ts;       }
//      Description: sets the time stamp to the value passed in as a parameter
//  void setLifeSpan(double ls)             { lifeSpan = ls;        }
//      Description: sets the life span of to the value passed in as a parameter
//  void setAckCode(AckCodes a)             { ack = a;              }
//      Description: sets the ack code to the value passed in as parameter
//
////misc functions
//  bool addDestName(Eaagles::Basic::String* name);
//      Description: adds a destination name to the list of intended recipients of this message
//  void removeDestName(Eaagles::Basic::String* name);
//      Description: removes all destination names that fully match the value passed in as a parameter
//  void clearDestNames();
//      Description: clears the list of destination names
//----------------------------------------------------------------------------------

#ifndef __Eaagles_Simulation_Message_H__
#define __Eaagles_Simulation_Message_H__

#include "openeaagles/basic/Object.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/List.h"

namespace Eaagles {
namespace Simulation {

class Message : public Eaagles::Basic::Object
{
    DECLARE_SUBCLASS(Message,Eaagles::Basic::Object)

public:
    //left as enum for future use
    enum AckCodes { ACK_REQUESTED, ACK };

    Message();

    //get functions
    Eaagles::Basic::String* getSenderName()                         { return senderName;    }
    Eaagles::Basic::List* getDestNames()                            { return destNames;     }
    virtual double getTimeStamp()                                   { return timeStamp;     }
    virtual double getLifeSpan()                                    { return lifeSpan;      }
    AckCodes getAckCode()                                           { return ack;           }

    //set functions
    virtual void setSenderName(Eaagles::Basic::String* sName);
    virtual void setSenderName(const char* const sName);
    virtual void setTimeStamp(double ts)                            { timeStamp = ts;       }
    virtual void setLifeSpan(double ls)                             { lifeSpan = ls;        }
    virtual void setAckCode(AckCodes a)                             { ack = a;              }

    //misc functions
    virtual bool addDestName(Eaagles::Basic::String* name);
    virtual void removeDestName(Eaagles::Basic::String* name);
    virtual void clearDestNames();

private:
    void initData();

    Eaagles::Basic::String* senderName;    // name of sending player
    Eaagles::Basic::List* destNames;       // player names to whome this message is intended for
    double timeStamp;                      // time at which this message was sent
    double lifeSpan;                       // seconds
    AckCodes ack;                          // ack code
};
} // End simulation
} // End Eaagles Namespace

#endif
