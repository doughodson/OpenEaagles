
#ifndef __oe_behaviors_PlaneAction_H__
#define __oe_behaviors_PlaneAction_H__

#include "openeaagles/base/ubf/Action.h"

namespace oe {

namespace simulation { class Player; }

namespace behaviors {

//------------------------------------------------------------------------------
// Class: PlaneAction
//------------------------------------------------------------------------------
class PlaneAction : public base::ubf::Action
{
   DECLARE_SUBCLASS(PlaneAction, base::ubf::Action)

public:
   PlaneAction();

   virtual bool execute(base::Component* actor) override;

   // get/set methods
   void setPitch(const double);
   double getPitch() const                  { return pitch;              }
   bool isPitchChanged() const              { return pitchChanged;       }
   void setPitchChanged(const bool x)       { pitchChanged = x;          }

   void setRoll(const double);
   double getRoll() const                   { return roll;               }
   bool isRollChanged() const               { return rollChanged;        }
   void setRollChanged(const bool x)        { rollChanged = x;           }

   void setHeading(const double);
   double getHeading() const                { return heading;            }
   bool isHeadingChanged() const            { return headingChanged;     }
   void setHeadingChanged(const bool x)     { headingChanged = x;        }

   void setYaw(const double);
   double getYaw() const                    { return yaw;                }
   bool isYawChanged() const                { return yawChanged;         }
   void setYawChanged(const bool x)         { yawChanged = x;            }

   void setThrottle(const double);
   double getThrottle() const               { return throttle;           }
   bool isThrottleChanged() const           { return throttleChanged;    }
   void setThrottleChanged(const bool x)    { throttleChanged = x;       }

   void setSpeed(const double);
   double getSpeed() const                  { return speed;              }
   bool isSpeedChanged() const              { return speedChanged;       }
   void setSpeedChanged(const bool x)       { speedChanged = x;          }

   void setFireMissile(const bool);
   bool getFireMissile() const              { return fireMissile;        }
   bool isFireMissileChanged() const        { return fireMissileChanged; }
   void setFireMissileChanged(const bool x) { fireMissileChanged = x;    }

   void setPitchTrim(const double);
   double getPitchTrim() const              { return pitchTrim;          }
   bool isPitchTrimChanged() const          { return pitchTrimChanged;   }
   void setPitchTrimChanged(const bool x)   { pitchTrimChanged = x;      }

private:

   double pitch;
   bool pitchChanged;

   double roll;
   bool rollChanged;

   double heading;
   bool headingChanged;

   double yaw;
   bool yawChanged;

   double throttle;
   bool throttleChanged;

   double speed;
   bool speedChanged;

   double pitchTrim;
   bool pitchTrimChanged;

   bool fireMissile;
   bool fireMissileChanged;

};

}
}

#endif
