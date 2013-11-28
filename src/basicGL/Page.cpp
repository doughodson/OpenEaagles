//
// Class: Page

#include "openeaagles/basicGL/Page.h"
#include "openeaagles/basicGL/Display.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"

namespace Eaagles {
namespace BasicGL {

IMPLEMENT_SUBCLASS(Page,"Page")

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Page)
    "page",                // 1: Initial subpage
    "pages",               // 2: Subpages
    "pagingEvent",         // 3: Page change event
    "subpagesFirst",       // 4: Draw subpages first (default: draw our page graphics first)
    "focusSlavedToSubpage", // 5: Slave the focus to the subpage (default: true)
END_SLOTTABLE(Page)


//------------------------------------------------------------------------------
//  Map slot table to handles 
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(Page)
    ON_SLOT(1,setPage,Basic::Identifier)
    ON_SLOT(2,setSubpageStream,Basic::PairStream)             
    ON_SLOT(2,setSubpageSingle,Page)
    ON_SLOT(3,setPagingEvent,Basic::PairStream)
    ON_SLOT(4,drawSubpageFirst,Basic::Number)
    ON_SLOT(5,setSlotFocusSlavedToSubpage,Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Event handler 
//------------------------------------------------------------------------------                                             
BEGIN_EVENT_HANDLER(Page)
    ON_EVENT(ON_ENTRY,onEntry)
    ON_EVENT(ON_EXIT,onExit)
    ON_EVENT_OBJ(BUTTON_HIT,onButtonHit,Basic::String)
    ON_ANYKEY(onKeyHit)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Page::Page() : cpName()
{
   STANDARD_CONSTRUCTOR()
   cp = 0;
   np = 0;
   subpages = 0;
   pageChgEvents = 0;
   pageArg = 0;
   caller = 0;
   postDraw1 = false;
   focusSlavedToSubpage = true;
   subpageSP = SUBPAGE_STACK_SIZE;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Page::copyData(const Page& org, const bool cc)
{
    BaseClass::copyData(org);

    // Copy subpages
    if (!cc && subpages != 0) { subpages->unref(); }
    if (org.subpages != 0) {
        subpages = org.subpages->clone();
        processSubpages();
    }
    else subpages = 0;

    // Copy events
    if (!cc && pageChgEvents != 0) { pageChgEvents->unref(); }
    if (org.pageChgEvents != 0) {
        pageChgEvents = org.pageChgEvents->clone();
    }
    else pageChgEvents = 0;
        
    // select the same pages
    cpName = org.cpName;
    cp = 0;
    np = 0;

    // Clear the subpage stack
    clearSubpageStack();

    // Post draw flag
    postDraw1 = org.postDraw1;

   focusSlavedToSubpage = org.focusSlavedToSubpage;

    // At this time we are not copying the paging arguments.
    pageArg = 0;
    caller = 0;
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Page::deleteData()
{
    // De-select the current pages
    cp = 0;
    cpName.empty();

    // Delete list of subpages
    if (subpages != 0) subpages->unref();
    subpages = 0;

    // Delete list of events
    if (pageChgEvents != 0) pageChgEvents->unref();
    pageChgEvents = 0;
}


//------------------------------------------------------------------------------
// updateTC() -- Update time critical stuff here
//------------------------------------------------------------------------------
void Page::updateTC(const LCreal dt)
{
   // Update our subpage and base class
   if (cp != 0) cp->tcFrame(dt);
   BaseClass::updateTC(dt);
}

//------------------------------------------------------------------------------
// updateData() --
//------------------------------------------------------------------------------
void Page::updateData(const LCreal dt)
{
   // ---
   // Prime the pump -- when we don't have a current subpage (cp) and we do
   // have a subpage name (cpName) then make it our current subpage.
   // ---
   if (cp == 0 && !cpName.isEmpty()) {
      Basic::Pair* p = 0;
      if (subpages != 0) p = subpages->findByName(cpName);
      if (p != 0) {
         np = (Page*) p->object();
         np->caller  = 0;
         np->pageArg = 0;
      }
   }

   // ---
   // Check for page changes
   // ---
   if (np != 0) {
      if ( np != cp ) {
         // When we have a new page ...
         //  1) inform the old page of EXIT and remove focus
         //  2) set the current page pointer
         //  3) inform the new page of the ENTRY and set focus
         if (cp != 0) {
            cp->event(ON_EXIT);
            if (isFocusSlavedToSubpage()) getDisplay()->focus(this);
         }
         cp = np;
         if (cp != 0) {
            if (isFocusSlavedToSubpage()) getDisplay()->focus(cp);
            cp->event(ON_ENTRY);
         }
      }
   }
   
   // Update our subpage and base class
   if (cp != 0) cp->updateData(dt);
   BaseClass::updateData(dt);
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Page::reset()
{
    BaseClass::reset();
    if (subpages != 0) {
        // Reset all of our sub-pages
        Basic::List::Item* item = subpages->getFirstItem();
        while (item != 0) {
            Basic::Pair* pair = (Basic::Pair*)(item->getValue());
            Component* obj = (Component*)( pair->object() );
            if (obj != 0) obj->reset();
            item = item->getNext();
        }
    }
}

//------------------------------------------------------------------------------
// findBySelectName() -- find one of our components by its GL Select (pick) name
//                    (our children first then grandchildren)
//------------------------------------------------------------------------------
Basic::Pair* Page::findBySelectName(const GLuint name)
{
    // Use our base class's functions to check normal components
    Basic::Pair* q = Graphic::findBySelectName(name);

    // If still not found, check our subpage ...
    if (q == 0 && cp != 0) {
        q = cp->findBySelectName(name);
    }
    return q;
}

//------------------------------------------------------------------------------
// newSubpage() -- change subpages
//------------------------------------------------------------------------------
bool Page::newSubpage(Page* const np1, Page* theCaller, Basic::Object* theArg)
{
    bool ok = false;
    if (np1 != 0) {
        cpName.empty();       // Unknown name
        np = np1;
        np->pageArg = theArg;
        np->caller  = theCaller;
        clearSubpageStack();
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// newSubpage() -- change subpages by name; returns true of page was found
//------------------------------------------------------------------------------
bool Page::newSubpage(const char* const name, Page* theCaller, Basic::Object* theArg)
{
    bool ok = false;
    Basic::Pair* p = 0;
    if (subpages != 0) p = subpages->findByName(name);
    if (p != 0) {
        cpName = name;            // It's our page
        np = (Page*) p->object();
        np->pageArg = theArg;
        np->caller  = theCaller;
        clearSubpageStack();
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// clearSubpageStack() -- Clear the subpage stack
//------------------------------------------------------------------------------
bool Page::clearSubpageStack()
{
   subpageSP = SUBPAGE_STACK_SIZE;
   return true;
}

//------------------------------------------------------------------------------
// pushSubpage() -- push the current subpage and change to new subpage 'name'.
//------------------------------------------------------------------------------
bool Page::pushSubpage(const char* const name, Page* theCaller, Basic::Object* theArg)
{
    bool ok = false;
    if (subpageSP > 0) {
        Page* currPage = cp;
        Basic::Pair* p = 0;
        if (subpages != 0) p = subpages->findByName(name);
        if (p != 0) {
           cpName = name;            // It's our page
           np = (Page*) p->object();
           np->pageArg = theArg;
           np->caller  = theCaller;
           ok = true;
        }
        //ok = newSubpage(name,theCaller,theArg);
        if (ok) subpageStack[--subpageSP] = currPage;
    }
    return ok;
}

//------------------------------------------------------------------------------
// popSubpage() -- pop to the previous subpage on the stack
//------------------------------------------------------------------------------
bool Page::popSubpage(Page* theCaller, Basic::Object* theArg)
{
   bool ok = false;
   if (subpageSP < SUBPAGE_STACK_SIZE) {
      //ok = newSubpage(subpageStack[subpageSP++],theCaller,theArg);
      cpName.empty();               // Unknown name
      np = subpageStack[subpageSP++];
      np->pageArg = theArg;
      np->caller  = theCaller;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Functions that ask our container to change pages (we're outa here)
//------------------------------------------------------------------------------

// New page by pointer
bool Page::newPage(Page* const newPage, Page* theCaller, Basic::Object* theArg)
{
    bool ok = false;
    Page* cc = dynamic_cast<Page*>(container());
    if (cc != 0) ok = cc->newSubpage(newPage,theCaller,theArg);
    return ok;
}

// New page by name
bool Page::newPage(const char* const name, Page* theCaller, Basic::Object* theArg)
{
    bool ok = false;
    Page* cc = dynamic_cast<Page*>(container());
    if (cc != 0) ok = cc->newSubpage(name,theCaller,theArg);
    return ok;
}

// Push new page
bool Page::pushPage(const char* const name, Page* theCaller, Basic::Object* theArg)
{
    bool ok = false;
    Page* cc = dynamic_cast<Page*>(container());
    if (cc != 0) ok = cc->pushSubpage(name,theCaller,theArg);
    return ok;
}

// Pop back to previous page
bool Page::popPage(Page* theCaller, Basic::Object* theArg)
{
    bool ok = false;
    Page* cc = dynamic_cast<Page*>(container());
    if (cc != 0) ok = cc->popSubpage(theCaller,theArg);
    return ok;
}

//------------------------------------------------------------------------------
// findSubpageByName(), findSubpageByType() -- find a member
//------------------------------------------------------------------------------
Basic::Pair* Page::findSubpageByName(const char* const slotname)
{
    Basic::Pair* p = 0;
    if (subpages != 0) p = subpages->findByName(slotname);
    return p;
}


Basic::Pair* Page::findSubpageByType(const std::type_info& type)
{
    Basic::Pair* p = 0;
    if (subpages != 0) p = subpages->findByType(type);
    return p;
}


//------------------------------------------------------------------------------
// onButtonHit() -- handle button hits (with button name) as page change requests
//------------------------------------------------------------------------------
bool Page::onButtonHit(const Basic::String* const obhobj)
{
    bool used = false;
    if (obhobj != 0 && pageChgEvents != 0) {
        used = true;
        Basic::Pair* pageEvent = pageChgEvents->findByName(*obhobj);
        if (pageEvent != 0) {
            Basic::Identifier* id = dynamic_cast<Basic::Identifier*>(pageEvent->object());
            if (id != 0) {
                // Find our container and the new page ID, then push
                // current page and go to new page
                newPage(*id,this);
            }
        }
    }
    return used;
}


//------------------------------------------------------------------------------
// onKeyHit() -- handle keyboard inputs as page change requests
//------------------------------------------------------------------------------
bool Page::onKeyHit(const int key)
{
    bool used = false;
    if (pageChgEvents != 0) {
        char keyName[2];
        keyName[0] = char(key);
        keyName[1] = '\0';

        // search for a page change event
        Basic::Pair*pageEvent = pageChgEvents->findByName(keyName);
        if (pageEvent != 0) {
            Basic::Identifier* id = dynamic_cast<Basic::Identifier*>(pageEvent->object());
            if (id != 0) {
                // Find our container and the new page ID, then push
                // current page and go to new page
                newPage(*id,this);
                used = true;
            }
        }
    }
    return used;
}

//------------------------------------------------------------------------------
// onEntry() -- 
//------------------------------------------------------------------------------
bool Page::onEntry()
{
   // send our focus to our subpage, if we have one.
   if (subpage() != 0) {
      getDisplay()->focus( subpage() );
      subpage()->event(ON_ENTRY);
   }
   return true;
}

//------------------------------------------------------------------------------
// onExit() -- 
//------------------------------------------------------------------------------
bool Page::onExit()
{
   if (subpage() != 0) {
      subpage()->event(ON_EXIT);
   }
   return true;
}


//------------------------------------------------------------------------------
// draw -- draw this object and its children
//------------------------------------------------------------------------------
void Page::draw()
{
   // Only when this object is visible ...
   if ( !isVisible() ) return;

    if (postDraw1) {
        // Draw the current subpage and ...
        if (cp != 0) cp->draw();
        // then draw our own page components
        BaseClass::draw();
    }
    else {
        // Draw our own page components and ...
        BaseClass::draw();
        // then draw the current subpage
        if (cp != 0) cp->draw();
    }

}

//------------------------------------------------------------------------------
// processSubpages() -- process our subpages; make sure they are all of
// type Page (or derived from it)and tell them that we are their
// container.
//------------------------------------------------------------------------------
bool Page::processSubpages()
{
    bool ok = true;
    if (subpages != 0) {
        // Make sure we have only Pages and tell all of the pages
        // that we are their container.
        const Basic::List::Item* item = subpages->getFirstItem();
        while (ok && item != 0) {
            Basic::Pair* p = (Basic::Pair*) item->getValue();
            item = item->getNext();
            Page* g = dynamic_cast<Page*>(p->object());
            if (g != 0) {
                // It MUST be of type Page
                g->container(this);
            }
            else {
                // Delete components that are not of Page type
                if (isMessageEnabled(MSG_ERROR)) {
                    std::cerr << "Page::processSubpages(): " << *p->slot() << " is not a Page!" << std::endl;
                }
                ok = false;
            }
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// setPage() -- sets the initial page
//------------------------------------------------------------------------------
bool Page::setPage(const Basic::Identifier* const pobj)
{
    if (pobj != 0) cpName =  *pobj;    
    return true;
}

//------------------------------------------------------------------------------
//  setSubpageStream() -- it takes a pair stream
//------------------------------------------------------------------------------
bool Page::setSubpageStream (Basic::PairStream* const psobj)
{
    bool ok = false;
    if (psobj != 0) {
        // When a PairStream (i.e., more than one, a list) of pages
        if (subpages != 0) subpages->unref();                               
        subpages = psobj;
        subpages->ref();

        ok = processSubpages();
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSubpageSingle() -- it takes an Page
//------------------------------------------------------------------------------
bool Page::setSubpageSingle(Page* const pobj)
{
    bool ok = false;
    if (pobj != 0) {
        if (subpages != 0) subpages->unref();
        
        subpages = new Basic::PairStream();
        subpages->put( new Basic::Pair("1",pobj) );
        ok = processSubpages();
    }
    return ok;
}

//------------------------------------------------------------------------------
// setPagingEvent() -- sets the page change event
//------------------------------------------------------------------------------
bool Page::setPagingEvent(Basic::PairStream* const peobj)
{
    if (peobj != 0) {
        if (pageChgEvents != 0) pageChgEvents->unref();
        pageChgEvents = peobj;
        pageChgEvents->ref();
    }
    return true;
}

//------------------------------------------------------------------------------
// drawSubpageFirst() -- Draw subpages first (default: draw our page graphics first)
 //------------------------------------------------------------------------------
bool Page::drawSubpageFirst(const Basic::Number* const spfobj)
{
    if (spfobj != 0) postDraw1 = spfobj->getBoolean();
    return true;
}

//------------------------------------------------------------------------------
// drawSubpageFirst() -- Draw subpages first (default: draw our page graphics first)
//------------------------------------------------------------------------------
bool Page::setSlotFocusSlavedToSubpage(const Basic::Number* const msg)
{
    if (msg != 0) setFocusSlavedToSubpage( msg->getBoolean() );
    return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Page
//------------------------------------------------------------------------------
Basic::Object* Page::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Page::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        //indent(sout,i);
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    if (cpName.len() > 0) {
        indent(sout,i+j);
        sout << "page: " << cpName << std::endl;
    }

    BaseClass::serialize(sout,i+j,true);

    if (subpages != 0) {
        indent(sout,i+j);
        sout << "pages: {" << std::endl;
        subpages->serialize(sout,i+j+4,slotsOnly);
        indent(sout,i+j);
        sout << "}" << std::endl;
    }

    if (pageChgEvents != 0) {
        indent(sout,i+j);
        sout << "pagingEvent: {" << std::endl;
        pageChgEvents->serialize(sout,i+j+4,slotsOnly);
        indent(sout,i+j);
        sout << "}" << std::endl;
    }

    if (postDraw1) {
        indent(sout,i+j);
        sout << "subpagesFirst: 1" << std::endl;
    }

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End BasicGL namespace
} // End Eaagles namespace
