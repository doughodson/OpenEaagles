//------------------------------------------------------------------------------
// Classes: SymbolLoader, SlSymbol
//------------------------------------------------------------------------------
#ifndef __oe_graphics_SymbolLoader_H__
#define __oe_graphics_SymbolLoader_H__

#include "openeaagles/graphics/MapPage.h"

namespace oe {
namespace graphics {

class SlSymbol;

//------------------------------------------------------------------------------
// Class: SymbolLoader
// Description: Generic symbol management class.
//
//    Symbols are described by their type codes. The loader will clone the
//    appropriate graphics, based on the symbol's type code, from a list of
//    graphical templates that are set using the 'templates' slot.
//
//    As symbol index is returned to the user when symbols are added.  This
//    index is used by the user code to manipulate and/or remove the symbol.
//
//    A helper class, SlSymbol, is used by the loader to track the state of
//    each symbol.  The user can access the SlSymbol object for any active
//    symbol using the getSymbol() function with the symbol's index.
//
//    The symbol location (e.g., lat/lon, NED, player or screen) are transformed
//    to screen coordinates using the MapPage base class.
//
//
// Notes:
//    1) All symbol index values are one-based; range: [ 1 ... getMaxSymbols() ]
//
//    2) The real-time thread, updateTC(), is not passed to our base class
//    or our component symbols.
//
//    3) Based on the MapPage, the map attributes of multiple symbol loaders
//    can be controlled by one MapPage container.
//
//    4) The symbol templates, which are used to draw the various types of
//    symbols, may contain any number of graphical subcomponents, which can
//    be named.  The updateSymbolText(), setSymbolVisible(), setSymbolFlashRate()
//    and setSymbolColor() functions can modify these named subcomponents
//    in the symbol's (cloned) copy of the template graphics.
//
//    For updateSymbolText(), setSymbolFlashRate() and setSymbolColor(), if
//    a zero pointer is passed for the 'name' then the top level graphical
//    component is modified.
//
//    5) The symbol's graphical component is positioned by SymbolLoader's
//    draw() function by applying lcTranslate() to the root graphical object.
//    Therefore, the root graphical object in each template must not have any
//    transformations previously applied to it, otherwise the final position
//    of the symbol will be effected.  Any of the root object's subcomponents
//    may have transformations applied.
//
//    6) The symbol's own heading is set by updateSymbolHeading().  To counter
//    rotate the symbol, or a portion of the symbol, from the map's rotation
//    back to the symbol's own heading, the symbol's graphical component must
//    contain a rotational subcomponent (e.g., Rotators) named "hdg".  The
//    counter rotation angle (in degrees) is sent via an UPDATE_VALUE6 event
//    to the "hdg" subcomponent of the symbols graphical component.
//
//    7) SymbolLoader will interconnect lines between each symbol, if given the right
//    data.  Set the interconnect flag to true, and then give a line width and color
//    (if so desired), and the symbol loader will draw lines between the symbols (this
//    is an easy way to draw routes).
//
// Factory name: SymbolLoader
// Slots:
//     templates         <PairStream>   ! List of templates to use for symbols
//     showOnlyInRange   <Number>       ! only show symbols within range (default: true)
//
//------------------------------------------------------------------------------
class SymbolLoader : public MapPage {
   DECLARE_SUBCLASS(SymbolLoader,MapPage)

public:
   static const int MAX_SYMBOLS = 300;

public:
   SymbolLoader();

   // Returns the current number of active symbols
   int getNumberOfActiveSymbols() const;

   // Returns the maximum number of active symbols
   int getMaxSymbols() const;

   // Returns the symbol type code for the symbol at index, 'idx',
   // or zero for no symbol at 'idx'.
   int getSymbolType(const int idx) const;

   // Returns the symbol index for 'mySymbol', or zero if not found.
   int getSymbolIndex(const graphics::Graphic* const mySymbol) const;

   // Returns the symbol at index 'idx', or zero if not found
   SlSymbol* getSymbol(const int idx);

   // Returns the 'const' symbol at index 'idx', or zero if not found
   const SlSymbol* getSymbol(const int idx) const;

   // Returns the symbol closest to the x,y position given (pixels),
   // or zero if none
   SlSymbol* getSymbol(const int xPixels, const int yPixels);

   // Adds a new symbol of type 'nType'; returns the symbol index or zero if unable to add
   virtual int addSymbol(const int nType, const char* const id, int specName = -1);

   // Change a symbol's type
   virtual bool setSymbolType(const int idx, const int nType);

   // Remove a symbol
   virtual bool removeSymbol(const int idx);

   // Clears all symbols from our master symbol table
   virtual bool clearLoader();

   // Sets the show in-range symbols only flag
   virtual bool setShowInRangeOnly(const bool flg);

   // Updates the position of a symbol using latitude and longitude (degs)
   virtual bool updateSymbolPositionLL(const int idx, const double lat, const double lon);

   // Updates the position of a symbol using distances north (X) and east (Y) from
   // the center of the MapPage reference point (see MapPage.h)
   virtual bool updateSymbolPositionXY(const int idx, const double xPos, const double yPos);

   // Updates the position of a symbol using distances X (nose) and Y (right wing) from our
   // ownship and rotated to the ownship's true heading.
   virtual bool updateSymbolPositionXYAircraft(const int idx, const double xPos, const double yPos);

   // Updates the position of the symbol using screen coordinates
   virtual bool updateSymbolPositionXYScreen(const int idx, const double xPos, const double yPos);

   // Updates the symbols true heading
   virtual bool updateSymbolHeading(const int idx, const double hdg);

   // Updates the symbol's value
   virtual bool updateSymbolValue(const int idx, base::Object* const value);

   // Updates the text of the named AsciiText type subcomponent
   virtual bool updateSymbolText(const int idx, const char* name, const char newString[]);

   // Updates the value of the named NumericReadout type subcomponent
   virtual bool updateSymbolText(const int idx, const char* name, const double newVal);

   // Sets the visibility flag for a symbol's subcomponent
   virtual bool setSymbolVisible(const int idx, const char* name, bool visibility);

   // Sets the flash rate (hz) for a symbol's subcomponent
   virtual bool setSymbolFlashRate(const int idx, const char* name, const double flashRate);

   // Change the color of a symbol (if 'name' == 0) or its subcomponent
   virtual bool setSymbolColor(const int idx, const char* name, const base::Color* cobj);

   // Change the color of a symbol (if 'name' == 0) or its subcomponent
   virtual bool setSymbolColor(const int idx, const char* name, const base::Identifier* cobj);

   // Update the symbol's "select name", which is used for pick() operations
   virtual bool updateSymbolSelectName(const int idx, const int newSN);

   // Interconnect settings
   virtual bool setInterconnect(const bool x);
   bool isInterconnected()  { return interconnect; };

   void draw() override;
   void drawFunc() override;

   void updateTC(const double dt = 0.0) override;

protected:
   bool setSlotTemplates(base::PairStream* myTemps);
   bool setSlotShowInRangeOnly(const base::Number* const x);
   bool setSlotInterconnect(const base::Number* const x);

   virtual SlSymbol* symbolFactory();  // Creates symbols objects

   int getSymbols(base::safe_ptr<SlSymbol>* const newSyms, const int max);

private:
   void initData();

   base::PairStream* templates;    // holds our pairstream of templates
   SlSymbol* symbols[MAX_SYMBOLS];  // holds our array of symbols
   bool showInRangeOnly;            // only show the symbols within our range, else draw all the symbols if false
   bool interconnect;               // Connect our symbols with a line?
};


//------------------------------------------------------------------------------
// Class: SlSymbol
// Description: General symbol used by SymbolLoader
//------------------------------------------------------------------------------
class SlSymbol : public base::Object {
    DECLARE_SUBCLASS(SlSymbol,base::Object)

public:
    // Max size of the symbol's ID
    static const int MAX_ID_SIZE = 8;

public:
   SlSymbol();

   bool isVisible() const;                   // Visibility
   int getType() const;                      // Type code
   const char* getId() const;                // ID sting
   base::Pair* getSymbolPair() const;       // Graphical component
   base::Object* getValue() const;          // Returns user defined data item

   bool isPositionLL() const;                // Returns true if using at lat/lon position
   bool isPositionAC() const;                // Returns true if using at aircraft nose/wing position
   bool isPositionXY() const;                // Returns true if using X/Y position [+X->North, +Y->East}
   bool isPositionScreen() const;            // Returns true if using screen position
   bool isHeadingValid() const;              // Returns true if heading is being used

   double getXPosition() const;              // X position ( NM or degs; based on flags )
   double getYPosition() const;              // Y position { NM or degs; based on flags )

   double getScreenXPos() const;             // X screen position (inches)
   double getScreenYPos() const;             // Y screen position (inches)

   double getHeadingDeg() const;             // Returns heading (degs)
   double getHeadingRad() const;             // heading (rads)
   base::Degrees* getHdgAngleObj() const;   // base::Angle object that holds the heading value
   Graphic* getHdgGraphics() const;          // Graphic object named 'hdg' to handle heading rotation

   void setVisible(const bool x);            // set our visibility
   void setLatLonFlag(const bool flg);       // Sets the lat/lon flag (if true we're using lat/lon, else XY)
   void setACCoordFlag(const bool flg);      // Sets the aircraft nose/wing flag (when using this instead of earth coordinates from aircraft)
   void setScreenFlag(const bool flg);       // set the manual screen position override flag
   void setType(const int t);                // Sets the user defined type (must match templates)
   void setId(const char* const v);          // Sets the ID string
   void setValue(base::Object* const v);    // Sets the user defined value, which is set to the graphical component

   void setXPosition(const double v);        // Sets the X position ( latitude or NM north/south )
   void setYPosition(const double v);        // Sets the Y position { longitude or NM east/west )
   void setXScreenPos(const double v);       // Sets the X screen pos (inches)
   void setYScreenPos(const double v);       // Sets the Y screen pos (inches) (does not include displacement)

   void setSymbolPair(base::Pair* const p);     // Sets the graphical component
   void setHeadingDeg(const double h);           // Sets the (optional) heading (degrees)
   void setHdgAngleObj(base::Degrees* const p); // Sets the base::Angle object that holds the heading value
   void setHdgGraphics(Graphic* const p);        // Sets the graphic object named 'hdg' to handle heading rotation

private:
   void initData();

   bool visibility;        // our symbol's visibility
   bool llFlg;             // Position is Lat/lon (not X/Y)
   bool acFlg;             // aircraft nose/wing coordinate flag
   bool scrnFlg;           // using screen coordinates only

   int type;               // numeric type (for looking up in slottable)
   char id[MAX_ID_SIZE+1]; // ID (or name) sent to the '
   base::Object* value;   // optional value (sent to the symbol as an UPDATE_VALUE event)
   base::Pair* pntr;      // The graphical component

   double xPos;            // X position ( latitude or NM north/south )
   double yPos;            // Y position { longitude or NM east/west )

   double xScreenPos;      // x position: Screen
   double yScreenPos;      // y position: Screen

   double hdg;             // symbol heading (degrees)
   bool hdgValid;          // Heading valid flag
   Graphic* phdg;          // Object named 'hdg' to handle heading rotation
   base::Degrees* hdgAng; // Value sent to the heading 'hdg' object
};

// -------------------------------------------------------------------------------
// Inline functions for SymbolLoader and SlSymbol
// -------------------------------------------------------------------------------

inline int SymbolLoader::getMaxSymbols() const { return MAX_SYMBOLS; }
inline bool SymbolLoader::setInterconnect(const bool flg) { interconnect = flg; return true; }

inline SlSymbol* SymbolLoader::getSymbol(const int idx)
{
   SlSymbol* p = 0;
   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if (symbols[i] != 0) p = symbols[i];
   }
   return p;
}

inline const SlSymbol* SymbolLoader::getSymbol(const int idx) const
{
   const SlSymbol* p = 0;
   if (idx >= 1 && idx <= MAX_SYMBOLS) {
      const int i = (idx - 1);
      if (symbols[i] != 0) p = symbols[i];
   }
   return p;
}

inline bool SlSymbol::isVisible() const                  { return visibility; }
inline bool SlSymbol::isHeadingValid() const             { return hdgValid; }
inline bool SlSymbol::isPositionLL() const               { return llFlg; }
inline bool SlSymbol::isPositionAC() const               { return acFlg; }
inline bool SlSymbol::isPositionXY() const               { return !llFlg || !scrnFlg; }
inline bool SlSymbol::isPositionScreen() const           { return scrnFlg; }

inline int SlSymbol::getType() const                     { return type; }
inline const char* SlSymbol::getId() const               { return id; }
inline base::Object* SlSymbol::getValue() const         { return value; }
inline base::Pair* SlSymbol::getSymbolPair() const      { return pntr; }

inline double SlSymbol::getXPosition() const             { return xPos; }
inline double SlSymbol::getYPosition() const             { return yPos; }

inline double SlSymbol::getScreenXPos() const            { return xScreenPos; }
inline double SlSymbol::getScreenYPos() const            { return yScreenPos; }

inline double SlSymbol::getHeadingDeg() const            { return hdg; }
inline double SlSymbol::getHeadingRad() const            { return static_cast<double>(hdg * base::Angle::D2RCC); }
inline base::Degrees* SlSymbol::getHdgAngleObj() const  { return hdgAng; }
inline Graphic* SlSymbol::getHdgGraphics() const         { return phdg; }

inline void SlSymbol::setXPosition(const double v)       { xPos = v; }
inline void SlSymbol::setYPosition(const double v)       { yPos = v; }
inline void SlSymbol::setXScreenPos(const double v)      { xScreenPos = v; }
inline void SlSymbol::setYScreenPos(const double v)      { yScreenPos = v; }
inline void SlSymbol::setVisible(const bool x)           { visibility = x; }
inline void SlSymbol::setType(const int t)               { type = t; }
inline void SlSymbol::setLatLonFlag(const bool flg)      { llFlg = flg; }
inline void SlSymbol::setACCoordFlag(const bool flg)     { acFlg = flg; }
inline void SlSymbol::setScreenFlag(const bool flg)      { scrnFlg = flg; }

}  // end of graphics namespace
}  // end of oe namespace

#endif
