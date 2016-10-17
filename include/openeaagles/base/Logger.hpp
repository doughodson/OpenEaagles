
#ifndef __oe_base_Logger_H__
#define __oe_base_Logger_H__

#include "openeaagles/base/Component.hpp"
#include <fstream>

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class: Logger
// Description: General Message/Event Logger
//
// Factory name: Logger
// Slots:
//     file       <String>     ! Log file name (default: empty string)
//     path       <String>     ! Path to log directory (optional). (default: empty string)
//     topLine    <String>     ! Optional top (first) line of file. (default: 0)
//
// Public member functions:
//
//      bool isOpen()
//      bool isFailed()
//          Get the state of the log file.
//
//      log(char* msg)
//      log(LogEvent* event)
//          Add an entry to the log file.
//
//      updateTC(double dt)
//          Update time critical part of this component (empty)
//
//      updateData(double dt)
//          Update background part of this component (tries to open the logfile)
//------------------------------------------------------------------------------
class Logger : public Component
{
    DECLARE_SUBCLASS(Logger, Component)

public:
    // -------------------------------------------------
    // Class: LogEvent
    // Base class:  Object -> LogEvent
    // Description: Abstract class for all logger events
    // -------------------------------------------------
    class LogEvent : public Object
    {
        DECLARE_SUBCLASS(LogEvent, Object)
    public:
        LogEvent();
        virtual const char* getDescription() = 0;
    };

public:
    Logger();

    bool isOpen() const                { return opened; }
    bool isFailed() const              { return failed; }

    const String* getFilename() const  { return filename; }
    const String* getPathname() const  { return pathname; }
    const String* getTopLine() const   { return topLine; }

    virtual bool setSlotFilename(const String* const msg);
    virtual bool setSlotPathName(const String* const msg);
    virtual bool setSlotTopLine(const String* const msg);

    virtual void log(const char* const msg);
    virtual void log(LogEvent* const event);

    virtual void updateTC(const double dt = 0.0) override;
    virtual void updateData(const double dt = 0.0) override;

protected:
    virtual bool openFile();

    void setOpen(const bool val)        { opened = val; }
    void setFailed(const bool val)      { failed = val; }

    std::ofstream*   lout;       // Output stream

private:
    String*        filename;     // Log file name
    String*        pathname;     // Path to log file directory
    const String*  topLine;      // Optional top (first) line of output
    bool           opened;       // File opened
    bool           failed;       // Open or write failed
};

}
}

#endif
