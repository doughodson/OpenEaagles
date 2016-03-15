
#include "openeaagles/graphics/SymbolLoader.h"
#include "openeaagles/graphics/Readouts.h"
#include "openeaagles/graphics/Polygon.h"
#include "openeaagles/graphics/Display.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/units/Angles.h"
#include "openeaagles/base/units/Distances.h"
#include "openeaagles/base/util/str_utils.h"

#include <cstring>

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace oe {
namespace graphics {

IMPLEMENT_SUBCLASS(SymbolLoader, "SymbolLoader")
EMPTY_SERIALIZER(SymbolLoader)

BEGIN_SLOTTABLE(SymbolLoader)
   "templates",         // 1) List of templates to use for navaids
   "showOnlyInRange",   // 2) only show symbols within map range
   "interconnect",      // 3) Interconnect the symbols
END_SLOTTABLE(SymbolLoader)

// Map slot table to handles
BEGIN_SLOT_MAP(SymbolLoader)
   ON_SLOT(1,setSlotTemplates,base::PairStream)
   ON_SLOT(2,setSlotShowInRangeOnly,base::Number)
   ON_SLOT(3,setSlotInterconnect,base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SymbolLoader::SymbolLoader()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void SymbolLoader::initData()
{
   templates = nullptr;
   for (int i = 0; i < MAX_SYMBOLS; i++) {
      symbols[i] = nullptr;
   }
   showInRangeOnly = true;
   interconnect = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void SymbolLoader::copyData(const SymbolLoader& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   // Clear the symbols; the user will need to
   // create these with the new template list.
   clearLoader();

   {
      base::PairStream* copy = nullptr;
      if (org.templates != nullptr) {
         copy = org.templates->clone();
      }
      setSlotTemplates(copy);
      if (copy != nullptr) copy->unref();
   }

   showInRangeOnly = org.showInRangeOnly;
   interconnect = org.interconnect;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void SymbolLoader::deleteData()
{
   if (templates != nullptr) templates->unref();
   templates = nullptr;

   // go through our whole array and 0 everyone out
   for (int i = 0; i < MAX_SYMBOLS; i++) {
      if (symbols[i] != nullptr) {
         symbols[i]->setSymbolPair(nullptr);
         symbols[i]->setValue(nullptr);
         symbols[i]->unref();
      }
   }
}


//------------------------------------------------------------------------------
// getNumberOfActiveSymbols() - returns the count of active symbols
//------------------------------------------------------------------------------
int SymbolLoader::getNumberOfActiveSymbols() const
{
   int num = 0;
   for (int i = 0; i < MAX_SYMBOLS; i++) {
      if(symbols[i] != nullptr) num++;
   }
   return num;
}

//------------------------------------------------------------------------------
// getSymbolType() - return the symbol's type code
//------------------------------------------------------------------------------
int SymbolLoader::getSymbolType(const int idx) const
{
   int result = 0;

   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if(symbols[i] != nullptr){
         result = symbols[i]->getType();
      }
   }

   return result;
}

//------------------------------------------------------------------------------
// getSymbolIndex() - returns the symbol index for a particular object, if
// it exists in our list of symbols.  If it doesn't exist, we return 0.
//------------------------------------------------------------------------------
int SymbolLoader::getSymbolIndex(const graphics::Graphic* const mySymbol) const
{
   int index = 0;
   for (int i = 0; i < MAX_SYMBOLS; i++) {
      if (symbols[i] != nullptr) {
         base::Pair* p = symbols[i]->getSymbolPair();
         graphics::Graphic* graph = static_cast<graphics::Graphic*>(p->object());
         if (mySymbol == graph) index = (i + 1);
      }
   }
   return index;
}

//------------------------------------------------------------------------------
// getSymbol() - gets a symbol based on the pixel x,y (from center) position specified
//------------------------------------------------------------------------------
SlSymbol* SymbolLoader::getSymbol(const int xPixel, const int yPixel)
{
   SlSymbol* sym = nullptr;
   if (getDisplay() != nullptr) {

      // we have to do a little math and figure our pixel to inches value
      GLdouble l = 0, r = 0, b = 0, t = 0, n = 0, f = 0;
      GLsizei w = 0, h = 0;
      getDisplay()->getOrtho(l, r, b, t, n, f);
      getDisplay()->getViewportSize(&w, &h);
      const double inchPerPixWidth = (r * 2) / w;
      const double inchPerPixHeight = (t * 2) / h;

      // we assume our xPixel and yPixel are from the center
      const double inchX = xPixel * inchPerPixWidth;
      const double inchY = yPixel * inchPerPixHeight;

      // index of the closest symbol
      int id = -1;

      // our "snapping" cursor distance is basically 1 pixel in the y direction
      const double cursorDist = 1 * inchPerPixHeight;

      double symX = 0;
      double symY = 0;
      double distX = 0;
      double distY = 0;
      double dist = 0;
      double lastDist = 500000;

      // now search our symbols for the closest symbol
      for (int i = 0; i < MAX_SYMBOLS; i++) {
         if (symbols[i] != nullptr) {
            symX = symbols[i]->getScreenXPos();
            symY = symbols[i]->getScreenYPos();
            distX = symX - inchX;
            distY = symY - inchY;
            dist = (distX * distX) + (distY * distY);
            if (dist < cursorDist) {
               if (dist < lastDist) {
                  lastDist = dist;
                  id = i;
               }
            }
         }
      }

      // If we found one then get a pointer to the symbol
      if (id != -1) sym = symbols[id];

   }
   return sym;
}

//------------------------------------------------------------------------------
// addSymbol() - adds a symbol to our array list;
// -- return the symbol's index; range [ 1 .. getMaxSymbols() ]
//    or zero if not added.
//------------------------------------------------------------------------------
int SymbolLoader::addSymbol(const int nType, const char* const id, int specName)
{
   int idx = 0;

   if (templates != nullptr) {

      // Find the graphic template for this type symbol, and make
      // sure that the template is a graphics::Graphic, since it
      // will be used as the symbol's graphical component.
      base::Pair* tpair = templates->getPosition(nType);
      if (tpair != nullptr) {
         graphics::Graphic* tg = dynamic_cast<graphics::Graphic*>(tpair->object());
         if (tg != nullptr) {

            // Find an empty symbol slot in our master symbol table
            for (int i = 0; i < MAX_SYMBOLS && idx == 0; i++) {
               if ( symbols[i] == nullptr ) {

                  // Create a new SlSymbol object to manage this symbol.
                  symbols[i] = symbolFactory();

                  // Clone the graphic template and set it as the
                  // symbol's graphical component.
                  base::Pair* newPair = tpair->clone();
                  graphics::Graphic* newGraph = static_cast<graphics::Graphic*>(newPair->object());

                  // Set the new graphical component's select name
                  GLuint mySelName = 0;
                  if (specName > 0) mySelName = specName;
                  else mySelName = graphics::Graphic::getNewSelectName();
                  newGraph->setSelectName(mySelName);

                  // Add the symbol's graphical component to our component list.
                  {
                     base::PairStream* comp = getComponents();
                     base::Component::processComponents(comp, typeid(graphics::Graphic), newPair);
                     if (comp != nullptr) comp->unref();
                  }

                  // Set the symbol's graphical component pointer
                  symbols[i]->setSymbolPair( newPair );
                  newPair->unref(); // symbol[i] now owns it.

                  // Set the symbol's type and ID.
                  symbols[i]->setType( nType );
                  symbols[i]->setId( id );

                  // And this is the new symbol's index
                  idx = (i + 1);
               }
            }
         }
      }
   }

   return idx;
}

//------------------------------------------------------------------------------
// setSymbolType() - change an existing symbol type to another type
//------------------------------------------------------------------------------
bool SymbolLoader::setSymbolType(const int idx, const int nType)
{
   bool ok = false;

   // Find the symbol
   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if (symbols[i] != nullptr) {

         // Find the graphic template for this type symbol, and make
         // sure that the template is a graphics::Graphic, since it
         // will be use as the symbol's graphical component.
         if (templates != nullptr) {
            base::Pair* tpair = templates->getPosition(nType);
            if (tpair != nullptr) {
               graphics::Graphic* tg = dynamic_cast<graphics::Graphic*>(tpair->object());
               if (tg != nullptr) {

                  // Get the symbol's old graphical component
                  base::Pair* oldPair = static_cast<base::Pair*>(symbols[i]->getSymbolPair());
                  graphics::Graphic* oldG = static_cast<graphics::Graphic*>(oldPair->object());

                  // Clone the new graphical component from the template
                  base::Pair* newPair = tpair->clone();

                  // Set the new graphical component's select name using the old's
                  graphics::Graphic* newGraph = static_cast<graphics::Graphic*>(newPair->object());
                  GLuint mySelName = oldG->getSelectName();
                  newGraph->setSelectName(mySelName);

                  // Add the new and remove the old components from our subcomponent list
                  {
                     base::PairStream* comp = getComponents();
                     base::Component::processComponents(comp, typeid(graphics::Graphic), newPair, oldG);
                     if (comp != nullptr) comp->unref();
                  }

                  // Set the symbol's graphical component pointer
                  symbols[i]->setSymbolPair( newPair );
                  newPair->unref(); // symbol[i] now owns it.

                  // Set new type
                  symbols[i]->setType( nType );

                  ok = true;
               }

            }
         }
      }

   }
   return ok;
}

//------------------------------------------------------------------------------
// Removes a symbol from the master symbol table
//------------------------------------------------------------------------------
bool SymbolLoader::removeSymbol(const int idx)
{
   bool ok = false;

   // Find the symbol
   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if (symbols[i] != nullptr) {

         // ---
         // remove the symbol's graphical component from our subcomponent list
         // ---
         {
            // Get the symbol's graphical component
            base::Pair* pair = symbols[i]->getSymbolPair();
            graphics::Graphic* g = static_cast<graphics::Graphic*>(pair->object());

            base::PairStream* x = getComponents();
            base::Component::processComponents(x, typeid(graphics::Graphic), nullptr, g);
            x->unref();
         }

         // ---
         // and remove it from our master symbol table
         // ---
         symbols[i]->setSymbolPair(nullptr);
         symbols[i]->unref();
         symbols[i] = nullptr;

         ok = true;
      }

   }
   return ok;
}

//------------------------------------------------------------------------------
// Clears all symbols from our master symbol table
//------------------------------------------------------------------------------
bool SymbolLoader::clearLoader()
{
   bool ok = false;
   for (int idx = 1; idx <= MAX_SYMBOLS; idx++) {
      removeSymbol(idx);
   }
   return ok;
}

//------------------------------------------------------------------------------
   // Sets the show in-range symbols only flag
//------------------------------------------------------------------------------
bool SymbolLoader::setShowInRangeOnly(const bool x)
{
   showInRangeOnly = x;
   return true;
}

//------------------------------------------------------------------------------
// Update the symbol's position as Lat/Lon
//------------------------------------------------------------------------------
bool SymbolLoader::updateSymbolPositionLL(const int idx, const double nLat, const double nLon)
{
   bool ok = false;
   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if (symbols[i] != nullptr) {
         symbols[i]->setXPosition( nLat );
         symbols[i]->setYPosition( nLon );
         symbols[i]->setLatLonFlag(true);
         symbols[i]->setACCoordFlag(false);
         symbols[i]->setScreenFlag(false);
         ok = true;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Update the symbol's position as X/Y  [NED], from the center of the reference
// point (found in MapPage)
//------------------------------------------------------------------------------
bool SymbolLoader::updateSymbolPositionXY(const int idx, const double xPos, const double yPos)
{
   bool ok = false;
   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if (symbols[i] != nullptr) {
         symbols[i]->setXPosition( xPos );
         symbols[i]->setYPosition( yPos );
         symbols[i]->setLatLonFlag(false);
         symbols[i]->setACCoordFlag(false);
         symbols[i]->setScreenFlag(false);
         ok = true;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Update the symbol's position XY, in reference from the ownship nose and wing
//------------------------------------------------------------------------------
bool SymbolLoader::updateSymbolPositionXYAircraft(const int idx, const double xPos, const double yPos)
{
   bool ok = false;
   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if (symbols[i] != nullptr) {
         symbols[i]->setXPosition( xPos );
         symbols[i]->setYPosition( yPos );
         symbols[i]->setLatLonFlag(false);
         symbols[i]->setACCoordFlag(true);
         symbols[i]->setScreenFlag(false);
         ok = true;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Update the symbol's position as X/Y in the screen,
// This is used when we want to manually position our symbols without using
// any math (or we do the math ourselves)
//------------------------------------------------------------------------------
bool SymbolLoader::updateSymbolPositionXYScreen(const int idx, const double xPos, const double yPos)
{
   bool ok = false;
   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if (symbols[i] != nullptr) {
         symbols[i]->setXScreenPos( xPos );
         symbols[i]->setYScreenPos( yPos );
         symbols[i]->setLatLonFlag(false);
         symbols[i]->setACCoordFlag(false);
         symbols[i]->setScreenFlag(true);
         ok = true;
      }
   }
   return ok;
}


//------------------------------------------------------------------------------
// Update the symbol's heading
//------------------------------------------------------------------------------
bool SymbolLoader::updateSymbolHeading(const int idx, const double hdg)
{
   bool ok = false;
   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if (symbols[i] != nullptr) {
         symbols[i]->setHeadingDeg( hdg );
         ok = true;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Update the symbol's value
//------------------------------------------------------------------------------
bool SymbolLoader::updateSymbolValue(const int idx, base::Object* const value)
{
   bool ok = false;
   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if (symbols[i] != nullptr) {
         symbols[i]->setValue( value );
         ok = true;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Updates the text of the named AsciiText type subcomponent
//------------------------------------------------------------------------------
bool SymbolLoader::updateSymbolText(const int idx, const char* name, const char newString[])
{
   bool ok = false;

   // Find the symbol
   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if (symbols[i] != nullptr) {

         // Get its graphical component
         base::Pair* p = static_cast<base::Pair*>(symbols[i]->getSymbolPair());
         if (p != nullptr) {
            graphics::Graphic* g = static_cast<graphics::Graphic*>(p->object());
            if (g != nullptr) {

               // If we were passed a name then use it to find the subcomponent
               // and change 'g' to point to the subcomponent instead.
               if (name != nullptr) {
                  base::Pair* spair = g->findByName(name);
                  if (spair != nullptr) {
                     // subcomponent found by name
                     g = static_cast<graphics::Graphic*>(spair->object());
                  }
                  else {
                     // no subcomponent was found by that name
                     g = nullptr;
                  }
               }

               if (g != nullptr) {
                  // Have a graphic, but make sure it's an AsciiText
                  graphics::AsciiText* text = dynamic_cast<graphics::AsciiText*>(g);
                  if (text != nullptr) {
                     // It's an AsciiText, so change the its text string.
                     text->setText(newString);
                     ok = true;
                  }
               }

            }
         }

      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Updates the value of the named NumericReadout type subcomponent
//------------------------------------------------------------------------------
bool SymbolLoader::updateSymbolText(const int idx, const char* name, const double x)
{
   bool ok = false;

   // Find the symbol
   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if(symbols[i] != nullptr){

         // Get its graphical component
         base::Pair* p = static_cast<base::Pair*>(symbols[i]->getSymbolPair());
         if (p != nullptr) {
            graphics::Graphic* g = dynamic_cast<graphics::Graphic*>(p->object());
            if (g != nullptr) {

               // If we were passed a name then use it to find the subcomponent
               // and change 'g' to point to the subcomponent instead.
               if (name != nullptr) {
                  base::Pair* spair = g->findByName(name);
                  if (spair != nullptr) {
                     // subcomponent found by name
                     g = static_cast<graphics::Graphic*>(spair->object());
                  }
                  else {
                     // no subcomponent was found by that name
                     g = nullptr;
                  }
               }

               if (g != nullptr) {
                  // Have a graphic, but make sure it's a numeric readout
                  graphics::NumericReadout* text = dynamic_cast<graphics::NumericReadout*>(g);
                  if (text != nullptr) {
                     // It's a NumericReadout type, so update its value
                     text->setValue(x);
                     ok = true;
                  }
               }

            }
         }

      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSymbolVisible() - set symbol visible / invisible
//------------------------------------------------------------------------------
bool SymbolLoader:: setSymbolVisible(const int idx, const char* name, bool visibility)
{
   bool ok = false;

   // Find the symbol
   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if (symbols[i] != nullptr) {
         // if no name is passed, the symbol is invisible, otherwise just
         // parts are
         if (name == nullptr) symbols[i]->setVisible(visibility);

         // Get its graphical component
         base::Pair* p = symbols[i]->getSymbolPair();
         if (p != nullptr) {
            graphics::Graphic* g = static_cast<graphics::Graphic*>(p->object());
            if (g != nullptr) {

               // If we were passed a name then use it to find the subcomponent
               // and change 'g' to point to the subcomponent instead.
               if (name != nullptr) {
                  base::Pair* spair = g->findByName(name);
                  if (spair != nullptr) {
                     // subcomponent found by name
                     g = static_cast<graphics::Graphic*>(spair->object());
                  }
                  else {
                     // no subcomponent was found by that name
                     g = nullptr;
                  }
               }

               // Set the visibility (if we found one)
               if (g != nullptr) ok = g->setVisibility(visibility);

            }

         }
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSymbolFlashRate() - set symbol and child components flashing
//------------------------------------------------------------------------------
bool SymbolLoader::setSymbolFlashRate(const int idx, const char* name, const double flashRate )
{
   bool ok = false;

   // Find the symbol
   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if (symbols[i] != nullptr) {

         // Get its graphical component
         base::Pair* p = symbols[i]->getSymbolPair();
         if (p != nullptr) {
            graphics::Graphic* g = static_cast<graphics::Graphic*>(p->object());
            if (g != nullptr) {

               // If we were passed a name then use it to find the subcomponent
               // and change 'g' to point to the subcomponent instead.
               if (name != nullptr) {
                  base::Pair* spair = g->findByName(name);
                  if (spair != nullptr) {
                     // subcomponent found by name
                     g = static_cast<graphics::Graphic*>(spair->object());
                  }
                  else {
                     // no subcomponent was found by that name
                     g = nullptr;
                  }
               }

               // Set the flash rate (if we found one)
               if (g != nullptr) ok = g->setFlashRate(flashRate);

            }
         }
      }
   }
   return ok;
}


//------------------------------------------------------------------------------
// update the symbol's color
//------------------------------------------------------------------------------
bool SymbolLoader::setSymbolColor(const int idx, const char* name, const base::Color* cobj)
{
   bool ok = false;

   // Find the symbol
   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if(symbols[i] != nullptr) {

         // Get its graphical component
         base::Pair* p = static_cast<base::Pair*>(symbols[i]->getSymbolPair());
         if (p != nullptr) {
            graphics::Graphic* g = dynamic_cast<graphics::Graphic*>(p->object());
            if (g != nullptr) {

               // If we were passed a name then use it to find the subcomponent
               // and change 'g' to point to the subcomponent instead.
               if (name != nullptr) {
                  base::Pair* spair = g->findByName(name);
                  if (spair != nullptr) {
                     // subcomponent found by name
                     g = static_cast<graphics::Graphic*>(spair->object());
                  }
                  else {
                     // no subcomponent was found by that name
                     g = nullptr;
                  }
               }

               // Set the color (if we found one)
               if (g != nullptr) ok = g->setColor(cobj);
            }
         }
      }
   }

   return ok;
}


//------------------------------------------------------------------------------
// update the symbol's color based on Identifier
//------------------------------------------------------------------------------
bool SymbolLoader::setSymbolColor(const int idx, const char* name, const base::Identifier* cname)
{
   bool ok = false;

   // Find the symbol
   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if(symbols[i] != nullptr) {

         // Get its graphical component
         base::Pair* p = static_cast<base::Pair*>(symbols[i]->getSymbolPair());
         if (p != nullptr) {
            graphics::Graphic* g = dynamic_cast<graphics::Graphic*>(p->object());
            if (g != nullptr) {

               // If we were passed a name then use it to find the subcomponent
               // and change 'g' to point to the subcomponent instead.
               if (name != nullptr) {
                  base::Pair* spair = g->findByName(name);
                  if (spair != nullptr) {
                     // subcomponent found by name
                     g = static_cast<graphics::Graphic*>(spair->object());
                  }
                  else {
                     // no subcomponent was found by that name
                     g = nullptr;
                  }
               }

               // Set the color (if we found one)
               if (g != nullptr) ok = g->setColor(cname);
            }
         }
      }
   }

   return ok;
}

//------------------------------------------------------------------------------
// Update the symbol's select name
//------------------------------------------------------------------------------
bool SymbolLoader::updateSymbolSelectName(const int idx, const int newSN)
{
   bool ok = false;
   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if (symbols[i] != nullptr) {

         base::Pair* pair = static_cast<base::Pair*>(symbols[i]->getSymbolPair());
         if (pair != nullptr) {
            graphics::Graphic* graphic = static_cast<graphics::Graphic*>(pair->object());
            if (graphic != nullptr) graphic->setSelectName(newSN);
         }
         ok = true;

      }
   }
   return ok;
}


//------------------------------------------------------------------------------
// symbolFactory() - Creates symbols objects; derived classes can override this
//                   to create unique, special purpose SlSymbol objects
//------------------------------------------------------------------------------
SlSymbol* SymbolLoader::symbolFactory()
{
   return new SlSymbol();
}

//------------------------------------------------------------------------------
// updateTC() - update time-critical threads here
//------------------------------------------------------------------------------
void SymbolLoader::updateTC(const double)
{
   // Do nothing here!

   // Don't call our base class, which will call our subcomponents.
   // Only want the symbol graphics updated by the updateData() and draw()
   // functions.
}


//------------------------------------------------------------------------------
// drawFunc() - interconnects the symbols (if we are interconnecting)
//------------------------------------------------------------------------------
void SymbolLoader::drawFunc()
{
    if (interconnect) {
        glPushMatrix();
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < MAX_SYMBOLS; i++) {
            if (symbols[i] != nullptr)
                glVertex2f(static_cast<GLfloat>(symbols[i]->getScreenXPos()),
                           static_cast<GLfloat>(symbols[i]->getScreenYPos()));
        }
        glEnd();
        glPopMatrix();
    }
    BaseClass::drawFunc();
}


//------------------------------------------------------------------------------
// draw() - draw the objects in their proper place
//------------------------------------------------------------------------------
void SymbolLoader::draw()
{
   if (isVisible()) {

      // Y Displacement (ie, decentered)
      double displacement = 0;
      if (!getCentered()) displacement = getDisplacement();

      // Radius (ie., range)
      double radius = 0;
      if (!getCentered()) radius = getOuterRadiusDC();
      else radius = getOuterRadius();
      double radius2 = radius * radius;

      // ---
      // Setup the drawing parameters for all of our symbols ...
      // ---
      for (int i = 0; i < MAX_SYMBOLS; i++) {

         if (symbols[i] != nullptr) {

            // When the symbol visibility flag is true ...
            if (symbols[i]->isVisible()) {

               // Get the pointer to the symbol's graphical component
               base::Pair* p = symbols[i]->getSymbolPair();
               graphics::Graphic* g = static_cast<graphics::Graphic*>(p->object());

               // We need the symbol's position in screen coordinates (inches) ...
               double xScn = static_cast<double>(symbols[i]->getScreenXPos());
               double yScn = static_cast<double>(symbols[i]->getScreenYPos());

               if ( !(symbols[i]->isPositionScreen()) ) {

                  // But when we were not give screen coordinates,
                  // we'll need to compute them from A/C coordinates
                  double acX = 0.0;
                  double acY = 0.0;

                  // 1) when given A/C coordinates ...
                  if ( symbols[i]->isPositionAC() ) {
                     acX = static_cast<double>(symbols[i]->getXPosition());
                     acY = static_cast<double>(symbols[i]->getYPosition());
                  }

                  // 2) when given NED or L/L coordinates ..
                  else {
                     double north = 0;
                     double east  = 0;

                     if (symbols[i]->isPositionLL()) {
                        // 2a) we were give L/L so convert to NED coordinates
                        const double lat = symbols[i]->getXPosition();
                        const double lon = symbols[i]->getYPosition();
                        latLon2Earth(lat, lon, &north, &east);
                     }
                     else {
                        // 2b) we were give NED coordinates
                        north = static_cast<double>(symbols[i]->getXPosition());
                        east  = static_cast<double>(symbols[i]->getYPosition());
                     }

                     // 2c) convert the NED coordinates to aircraft coordinates
                     earth2Aircraft(north, east, &acX, &acY);
                  }

                  // 3) Convert the aircraft coordinates to screen coordinates
                  aircraft2Screen(acX, acY, &xScn, &yScn);

                  // 4) Save the screen coordinates (inches)
                  symbols[i]->setXScreenPos(xScn);
                  symbols[i]->setYScreenPos(yScn);
               }

               // In range?  Do we care?
               bool inRange  = !showInRangeOnly || (((xScn * xScn) + (yScn * yScn)) <= radius2);

               if (inRange) {

                  // set symbol's visibility
                  g->setVisibility(true);

                  // and set the symbol's position
                  g->lcSaveMatrix();
                  g->lcTranslate(xScn, yScn + displacement);

                  // pass the argument value to the symbol (if needed)
                  if (symbols[i]->getValue() != nullptr) {
                     g->event(UPDATE_VALUE, symbols[i]->getValue());
                  }

                  // rotate the symbol's heading subcomponent (if needed)
                  // -- sending a 'Z' rotation event to a component named 'hdg'
                  if (symbols[i]->isHeadingValid()) {
                     graphics::Graphic* phdg = symbols[i]->getHdgGraphics();
                     if (phdg == nullptr) {
                        base::Pair* hpair = static_cast<base::Pair*>(g->findByName("hdg"));
                        if (hpair != nullptr) {
                           phdg = dynamic_cast<Graphic*>(hpair->object());
                           symbols[i]->setHdgGraphics(phdg);
                        }
                     }
                     if (phdg != nullptr) {
                        base::Degrees* angObj = symbols[i]->getHdgAngleObj();
                        if (angObj == nullptr) {
                           angObj = new base::Degrees();
                           symbols[i]->setHdgAngleObj(angObj);
                        }
                        double relHeading = symbols[i]->getHeadingDeg() - getHeadingDeg();
                        angObj->set(-relHeading);
                        phdg->event(UPDATE_VALUE6, angObj);
                     }
                  }
               }
               else {
                  // out of range, so clear the graphical component's visibility flag
                  g->setVisibility(false);
               }
            }

            // When the symbol visibility flag is false ...
            else {
               base::Pair* p = symbols[i]->getSymbolPair();
               graphics::Graphic* g = static_cast<graphics::Graphic*>(p->object());
               g->setVisibility(false);
            }
         }
      }

      // ---
      // Let our base class handle the drawing
      // ---
      BaseClass::draw();

      // ---
      // now restore the matrices on all of our graphical components
      // ---
      for (int i = 0; i < MAX_SYMBOLS; i++) {
         if (symbols[i] != nullptr) {
            base::Pair* p = symbols[i]->getSymbolPair();
            graphics::Graphic* g = static_cast<graphics::Graphic*>(p->object());
            if (g->isVisible()) g->lcRestoreMatrix();
         }
      }

   }
}

//------------------------------------------------------------------------------
// Gets our list of symbols, and returns the number of symbols
//------------------------------------------------------------------------------
int SymbolLoader::getSymbols(base::safe_ptr<SlSymbol>* const newSyms, const int max)
{
   int numSymbols = 0;
   if (max > 0) {
      for(int i = 0; i < MAX_SYMBOLS; i++) {
         if (symbols[i] != nullptr) {
            newSyms[i] = symbols[i];
            numSymbols = i;
         }
      }
   }
   return numSymbols;
}


//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

// simply loads our template pairstream
bool SymbolLoader::setSlotTemplates(base::PairStream* msg)
{
   bool ok = false;
   if (msg != nullptr) {
      if (templates != nullptr) templates->unref();
      templates = msg;
      if (templates != nullptr) templates->ref();
      ok = true;
   }
   return ok;
}

// Show in range only flag
bool SymbolLoader::setSlotShowInRangeOnly(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) ok = setShowInRangeOnly(msg->getBoolean());
   return ok;
}

// Interconnect flag
bool SymbolLoader::setSlotInterconnect(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) ok = setInterconnect(msg->getBoolean());
   return ok;
}


//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* SymbolLoader::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}



//==============================================================================
// class SlSymbol
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SlSymbol, "SlSymbol")
EMPTY_SERIALIZER(SlSymbol)


SlSymbol::SlSymbol()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void SlSymbol::initData()
{
   visibility = true;
   llFlg = false;
   acFlg = false;
   scrnFlg = false;

   type = 0;
   id[0] = '\0';
   value = nullptr;
   pntr = nullptr;

   xPos = 0;
   yPos = 0;

   xScreenPos = 0;
   yScreenPos = 0;

   hdg = 0;
   hdgValid = false;
   hdgAng = nullptr;
   phdg = nullptr;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void SlSymbol::copyData(const SlSymbol& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   visibility = org.visibility;
   llFlg = org.llFlg;
   acFlg = org.acFlg;
   scrnFlg = org.scrnFlg;

   type = org.type;
   base::lcStrcpy(id, sizeof(id), org.id);

   xPos = org.xPos;
   yPos = org.yPos;

   xScreenPos = org.xScreenPos;
   yScreenPos = org.yScreenPos;

   hdg = org.hdg;
   hdgValid = org.hdgValid;
   setHdgGraphics(nullptr);
   setHdgAngleObj(nullptr);

   {
      base::Object* copy = nullptr;
      if (org.value != nullptr) copy = org.value->clone();
      setValue(copy);
      if (copy != nullptr) copy->unref();
   }

   {
      base::Pair* copy = nullptr;
      if (org.pntr != nullptr) copy = org.pntr->clone();
      setSymbolPair(copy);
      if (copy != nullptr) copy->unref();
   }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void SlSymbol::deleteData()
{
   setSymbolPair(nullptr);
   setValue(nullptr);
   setHdgAngleObj(nullptr);
   setHdgGraphics(nullptr);
}

//------------------------------------------------------------------------------
// Member functions
//------------------------------------------------------------------------------

void SlSymbol::setHeadingDeg(const double h)
{
   hdg = h;
   hdgValid = true;
}

void SlSymbol::setId(const char* const v)
{
   if (v != nullptr) {
      std::strncpy(id, v, MAX_ID_SIZE);
      id[MAX_ID_SIZE] = '\0';
   }
   else {
      id[0] = '\0';
   }
}

void SlSymbol::setValue(base::Object* const v)
{
   if (value != nullptr) value->unref();
   value = v;
   if (value != nullptr) value->ref();
}

void SlSymbol::setSymbolPair(base::Pair* const p)
{
   if (pntr != nullptr) pntr->unref();
   pntr = p;
   if (pntr != nullptr) pntr->ref();
}

void SlSymbol::setHdgAngleObj(base::Degrees* const v)
{
   hdgAng = v;
}

void SlSymbol::setHdgGraphics(Graphic* const v)
{
   phdg = v;
}

}  // end of graphics namespace
}  // end of oe namespace
