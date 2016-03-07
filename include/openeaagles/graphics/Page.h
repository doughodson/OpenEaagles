//------------------------------------------------------------------------------
// Class: Page
//------------------------------------------------------------------------------
#ifndef __oe_graphics_Page_H__
#define __oe_graphics_Page_H__

#include "Graphic.h"
#include "openeaagles/base/Identifier.h"

namespace oe {
namespace graphics {

//------------------------------------------------------------------------------
// Class: Page
//
// Description: Page format. A list of graphic objects and methods to manage
//              them as a page, also contains subpages and a background page.
// Factory name: Page
//
// Slots:
//  page                 <Identifier>   ! Initial subpage (default: 0)
//  pages                <PairStream>   ! Subpages (default: 0)
//  pagingEvent          <Page>         ! Page change event (default: 0)
//  subpagesFirst        <PairStream>   ! Draw subpages first (default: draw our page graphics first)
//  focusSlavedToSubpage <Number>       ! Slave the focus to the subpage (default: true)
//
// Events
//  1. ON_ENTRY
//  2. ON_EXIT
//  3. BUTTON_HIT
//  4. ON_ANYKEY
//
// Public member functions:
//  char* subpageName()
//    Returns cpName.
//
//  Page* subpage()
//    Returns cp.
//
//  bool isPostDrawSubpage()
//    Returns postDraw1
//
//  Pair* findSubpageByName(char* slotname)
//    Returns a member from the slot name given, else 0.
//
//  Pair* findSubpageByType(std::type_info& type)
//    Returns a member of the given type, else 0.
//
//  bool isFocusSlavedToSubpage()
//    Returns true if the focus is slaved to a subpage.
//
//  void setFocusSlavedToSubpage(bool f)
//    Sets focusSlavedToSubpage to f.
//
// macro slottable functions
//  bool setPage(Identifier* const pobj)
//    Sets the page to pobj and returns true if successful.
//
//  bool setSubpageStream (PairStream* psobj)
//    Sets the subpage stream to psobj and returns true if successful.
//
//  bool setSubpageSingle (Page* pobj)
//    Sets the subpage single to pobj and returns true if successful.
//
//  bool setPagingEvent(PairStream* peobj)
//    Sets the paging event to peobj and returns true if successful.
//
//  bool drawSubpageFirst(Number* spfobj)
//    Set drawSubpageFirst (boolean) then return true if successful.
//
//  bool setSlotFocusSlavedToSubpage(Number* spfobj)
//    Set focusSlavedToSubpage (boolean) and return true if successful.
//
// macro event handler functions
//  bool onEntry()
//    Returns true. (Function Not Implemented.)
//
//  bool onExit()
//    Returns true. (Function Not Implemented.)
//
//  bool onButtonHit(String* obhobj)
//    Handles the button hit as a page change event.
//
//  bool onKeyHit(int key)
//    Handles the keyboard hit as a page change event.
//
// Call new sub-page
//  bool newSubpage(Page* newPage, Page* theCaller, Object* theArg)
//    Changes subpages by page. Returns true if page was found.
//
//  bool newSubpage(char* name, Page* theCaller, Object* theArg)
//    Changes subpages by name. Returns true if the page was found.
//------------------------------------------------------------------------------
class Page : public Graphic
{
   DECLARE_SUBCLASS(Page,Graphic)

public:
   Page();

   const char* subpageName() const              { return cpName; }
   Page* subpage() const                        { return cp; }

   bool isPostDrawSubpage() const               { return postDraw1; }

   virtual base::Pair* findSubpageByName(const char* const slotname);
   virtual base::Pair* findSubpageByType(const std::type_info& type);

   bool isFocusSlavedToSubpage() const          { return focusSlavedToSubpage; }
   void setFocusSlavedToSubpage(const bool f)   { focusSlavedToSubpage = f; }

   // Call new sub-page
   bool newSubpage(Page* const newPage, Page* theCaller, base::Object* theArg = 0);
   bool newSubpage(const char* const name, Page* theCaller, base::Object* theArg = 0);

   // event handler functions
   virtual bool onEntry();
   virtual bool onExit();
   virtual bool onButtonHit(const base::String* const obhobj);
   virtual bool onKeyHit(const int key);

   // slottable functions
   bool setPage(const base::Identifier* const pobj);
   bool setSubpageStream (base::PairStream* const psobj);
   bool setSubpageSingle (Page* const pobj);
   bool setPagingEvent(base::PairStream* const peobj);
   bool drawSubpageFirst(const base::Number* const spfobj);
   bool setSlotFocusSlavedToSubpage(const base::Number* const spfobj);

   void draw() override;
   base::Pair* findBySelectName(const GLuint name) override;
   bool event(const int event, base::Object* const obj = nullptr) override;

   void updateTC(const double dt = 0.0) override;
   void updateData(const double dt = 0.0) override;
   void reset() override;

protected:
   // Return our paging arguments
   base::Object* getArgument()               { return pageArg; }
   const Page* getCaller()                    { return caller; }

   // Return our subpages
   base::PairStream* subPages()              { return subpages; }

   // Manage our (sub)page stack
   bool clearSubpageStack();
   bool pushSubpage(const char* const name, Page* theCaller, base::Object* theArg = 0);
   bool popSubpage(Page* theCaller, base::Object* theArg = 0);

   // Call/push/pop major pages (our container's pages, which we are a member of)
   bool newPage(Page* const newPage, Page* theCaller, base::Object* theArg = 0);
   bool newPage(const char* const name, Page* theCaller, base::Object* theArg = 0);
   bool pushPage(const char* const name, Page* theCaller, base::Object* theArg = 0);
   bool popPage(Page* theCaller, base::Object* theArg = 0);

private:
   bool processSubpages();

   Page*       cp;                     // Current Subpage
   base::Identifier cpName;           // Current Subpage Name
   Page*       np;                     // New subpage (requesting a page change)

   base::PairStream* subpages;        // Subpages
   base::PairStream* pageChgEvents;   // Page change events

   bool  postDraw1;                    // Post draw component (child) graphic
   bool  focusSlavedToSubpage;         // Input event focus should follow subpage changes

   // Passed by calling page
   base::safe_ptr<base::Object> pageArg;   // Paging argument
   const Page* caller;                       // Calling page

   // Subpage Stack
   static const int SUBPAGE_STACK_SIZE = 50;
   Page* subpageStack[SUBPAGE_STACK_SIZE];
   int      subpageSP;                 // Stack pointer
};

} // End graphics namespace
} // End oe namespace

#endif
