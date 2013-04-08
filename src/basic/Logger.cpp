
#include "openeaagles/basic/Logger.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Basic {

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

IMPLEMENT_SUBCLASS(Logger,"Logger")

// Slot table for this form type
BEGIN_SLOTTABLE(Logger)
    "file",         // 1) Log file name                               (String)
    "path",         // 2) Path to the log file directory (0ptional)   (String)
    "topLine",      // 3) Top (first) line of file       (0ptional)   (String)
END_SLOTTABLE(Logger)

// Map slot table to handles 
BEGIN_SLOT_MAP(Logger)
    ON_SLOT( 1, setSlotFilename, String)   
    ON_SLOT( 2, setSlotPathName, String)
    ON_SLOT( 3, setSlotTopLine,  String)
END_SLOT_MAP()


//==============================================================================
// Class Logger
//==============================================================================

// -----------------------------------------------------------------
// Constructor: 
// -----------------------------------------------------------------
Logger::Logger()
{
    STANDARD_CONSTRUCTOR()
    
    lout = 0;
    filename = new String();
    pathname = new String();
    topLine = 0;
    opened = false;
    failed = false;
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Logger::copyData(const Logger& org, const bool cc)
{
    BaseClass::copyData(org);
    
    if (cc) {
        filename = 0;
        pathname = 0;
        lout = 0;
        topLine = 0;
    }
    if (filename == 0) filename = new String();
    if (pathname == 0) pathname = new String();
    
    *filename = *org.filename;
    *pathname = *org.pathname;

    setSlotTopLine(org.topLine);

    opened = false;    
    failed = false;
}

void Logger::deleteData()
{
    if (filename != 0) filename->unref();
    filename = 0;
    
    if (pathname != 0) pathname->unref();
    pathname = 0;

    setSlotTopLine(0);

    if (lout != 0) {
        if (isOpen()) lout->close();
        delete lout;
    }
    lout = 0;
}

//------------------------------------------------------------------------------
// updateTC() -- 
//------------------------------------------------------------------------------
void Logger::updateTC(const LCreal dt)
{
    BaseClass::updateTC(dt);
}

//------------------------------------------------------------------------------
// updateData() -- Update the log file
//------------------------------------------------------------------------------
void Logger::updateData(const LCreal dt)
{
    BaseClass::updateData(dt);
    
    // Open the log file (if we haven't tried before)
    if (!isOpen() && !isFailed()) {
        openFile();
    }
}

//------------------------------------------------------------------------------
// updateData() -- Update the log file
//------------------------------------------------------------------------------
bool Logger::openFile()
{
    // local flags (default is success)
    bool tOpened = true;
    bool tFailed = false;

    // Need a file name
    if (filename->len() ==  0) {
        if (isMessageEnabled(MSG_ERROR)) {
           std::cerr << "Logger::openFile(): Unable to open logger file: no file name" << std::endl;
        }
        tOpened = false;
        tFailed = true;
    }
    else {

        //---
        // Allocate space for the full file name
        //---
        size_t len = pathname->len();   // start with the length of the path name
        len += 1;   // add a character for the slash
        len += filename->len(); // add the length of the file name
        len += 4;  // add characters for possible version number, "_V99"
        len += 1;  // Add one for the null(0) at the end of the string
		const size_t NAME_LENGTH = len;

        char* fullname = new char[NAME_LENGTH];
        fullname[0] = '\0';

        //---
        // Create the (initial) full file name
        //---
        if (pathname->len() > 0) {
            lcStrcat(fullname,NAME_LENGTH,*pathname);
            lcStrcat(fullname,NAME_LENGTH,"/");
        }
        lcStrcat(fullname,NAME_LENGTH,*filename);

        //---
        // Make sure that it doesn't already exist (we don't want to over write good data).
        //---
        bool validName = !doesFileExist(fullname);
        if ( !validName ) {
            // If the file already exists, try appending a version number "v99" ..

            char* origname = new char[NAME_LENGTH];
            lcStrcpy(origname, NAME_LENGTH, fullname);

            validName = false;
            for (unsigned int i = 1; i <= 99 && !validName; i++) {
                std::sprintf(fullname, "%s_v%02d", origname, i);
                validName = !doesFileExist(fullname);
            }

            if ( !validName ) {
                if (isMessageEnabled(MSG_ERROR)) {
                  std::cerr << "Logger::openFile(): All version of the logger file already exists: " << origname << std::endl;
                }
                tOpened = false;
                tFailed = true;
            }
            delete[] origname;
        }

        if ( validName ) {
            //---
            // Make sure we have an output stream
            //---
            if (lout == 0) lout = new std::ofstream();

            //---
            // Open the file
            //---
            if (isMessageEnabled(MSG_INFO)) {
               std::cout << "Logger::openFile() Opening log file = " << fullname << std::endl;
            }
            lout->open(fullname);
            if (lout->fail()) {
                if (isMessageEnabled(MSG_ERROR)) {
                  std::cerr << "Logger::openFile(): Failed to open log file: " << fullname << std::endl;
                }
                tOpened = false;
                tFailed = true;
            }
            else if (topLine != 0) {
                *lout << *topLine << std::endl;
            }
                
        }

        delete[] fullname;
    }

    opened = tOpened;
    failed = tFailed;
    return opened;
}

//------------------------------------------------------------------------------
// log() -- Default (null) log entry
//------------------------------------------------------------------------------
void Logger::log(const char* const msg)
{
    if (isOpen()) {
        *lout << msg << std::endl;
    }
}

//------------------------------------------------------------------------------
// log() -- Default (null) log entry
//------------------------------------------------------------------------------
void Logger::log(LogEvent* const event)
{
    if (isOpen() && event != 0) {
        *lout << event->getDescription() << std::endl;
    }
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool Logger::setSlotFilename(const String* const msg)
{
    if (msg != 0) {
        *filename = *msg;
    }
    return true;
}

bool Logger::setSlotPathName(const String* const msg)
{
    if (msg != 0) {
        *pathname = *msg;
    }
    return true;
}

bool Logger::setSlotTopLine(const String* const msg)
{
   if (topLine != 0) topLine->unref();
   topLine = msg;
   if (topLine != 0) topLine->ref();
   return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
Object* Logger::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Logger::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        //indent(sout,i);
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    // File name
    if (filename->len() > 0) {
        indent(sout,i+j);
        sout << "filename: \"" << *filename << "\""<< std::endl;
    }

    // Path name
    if (pathname->len() > 0) {
        indent(sout,i+j);
        sout << "pathname: \"" << *pathname << "\"" << std::endl;
    }

    // TopLine
    if (topLine != 0) {
       if (topLine->len() > 0) {
          indent(sout,i+j);
          sout << "topLine: \"" << *topLine << "\"" << std::endl;
       }
    }

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

//==============================================================================
// Class LogEvent
//==============================================================================
IMPLEMENT_PARTIAL_SUBCLASS(Logger::LogEvent,"LogEvent")
EMPTY_SLOTTABLE(Logger::LogEvent)
EMPTY_SERIALIZER(Logger::LogEvent)

// Constructor
Logger::LogEvent::LogEvent()
{
    STANDARD_CONSTRUCTOR()
}

// Copy Constructor
Logger::LogEvent::LogEvent(const LogEvent& org)
{ 
    STANDARD_CONSTRUCTOR()
    copyData(org,true);
}

// Destructor
Logger::LogEvent::~LogEvent()
{
   STANDARD_DESTRUCTOR()
}

// Copy operator
Logger::LogEvent& Logger::LogEvent::operator=(const LogEvent& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}

// Clone
Logger::LogEvent* Logger::LogEvent::clone() const
{
    return 0;
}

// Copy data function
void Logger::LogEvent::copyData(const LogEvent& org, const bool)
{
    BaseClass::copyData(org);
}

// Delete data function
void Logger::LogEvent::deleteData()
{
}

} // End Basic namespace
} // End Eaagles namespace
