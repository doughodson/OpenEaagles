//------------------------------------------------------------------------------
// Classes: Point, Circle, Arc, LineLoop, Line
//------------------------------------------------------------------------------
#ifndef __oe_BasicGL_Shapes_H__
#define __oe_BasicGL_Shapes_H__

#include "Graphic.h"

namespace oe {

   namespace Basic {
      class Number;
   }

namespace BasicGL {

//------------------------------------------------------------------------------
// Class: Circle
//
// Factory name: Circle
// Slots:
//  radius    <Number>    ! Radius of the circle (default: 1 )
//  filled    <Number>    ! True if circle is filled (default: false)
//  slices    <Number>    ! Number of slices in the circle (i.e. number of sides)
//                        ! (default: 16)
//
//
// Public member functions:
//
//  bool setRadius(Number* srobj)
//      Sets the radius to srobj and returns true if successful.
//
//  bool setFilled(Number* sfobj)
//      Sets filled to sfobj and returns true if successful.
//
//  bool setSlices(Number* ssobj)
//      Sets slices to ssobj and returns true if successful.
//
//------------------------------------------------------------------------------
class Circle : public Graphic
{
    DECLARE_SUBCLASS(Circle,Graphic)
public:
    Circle();

    void drawFunc() override;

    virtual bool setRadius(const LCreal x)   { radius = x; return true; }
    virtual bool setFilled(const bool x)    { filled = x; return true; }
    virtual bool setSlices(const int x)     { slices = x; return true; }

    LCreal getRadius()       { return radius; }
    bool isFilled()         { return filled; }
    int getSlices()         { return slices; }

    bool event(const int event, Basic::Object* const obj = nullptr) override;

protected:
    bool setSlotRadius(const Basic::Number* const x);
    bool setSlotFilled(const Basic::Number* const x);
    bool setSlotSlices(const Basic::Number* const x);

    // event functions
    bool updateRadius(const Basic::Number* const x);

private:
    LCreal radius;
    bool  filled;
    int   slices;
};

//------------------------------------------------------------------------------
// Class: OcclusionCircle
//
// Factory name: OcclusionCircle
// Slots:
//  outerRadius   <Number>    ! Outer radius of our circle (default: 1.1 )
//
// Draws a nice doughnut type circle, based on inner and outer radius
//------------------------------------------------------------------------------
class OcclusionCircle : public Circle
{
    DECLARE_SUBCLASS(OcclusionCircle,Circle)

public:
    OcclusionCircle();

    void drawFunc() override;

    virtual bool setOuterRadius(const LCreal x)  { outerRadius = x; return true; }

    LCreal getOuterRadius()     { return outerRadius; }

protected:
    bool setSlotOuterRadius(const Basic::Number* const x);

private:
    LCreal outerRadius;          // portion that is occluded
};


//------------------------------------------------------------------------------
// Class: Arc
//
// Factory name: Arc
// Slots:
//  startAngle   <Number>    ! Start angle of the arc in degrees (default: 0 )
//  arcLength    <Number>    ! length of the arc in degrees (default: 90)
//  connect      <Number>    ! if true, we connect the lines (so the circle looks like a piece of pie,
//                           ! instead of an open ended arc) (default: false)
//
// Public member functions:
//
//  bool setStartAngle(Number* ssaobj)
//      Sets the start angle to ssaobj and returns true if successful.
//
//  bool setArcLength(Number* seaobj)
//      Sets the arc length te seaobj and returns true if successful.
//
//  bool setIsConnected(Number* iscobj)
//      Sets isConnected (boolean) and returns true if successful.
//
//------------------------------------------------------------------------------
class Arc : public Circle
{
    DECLARE_SUBCLASS(Arc,Circle)
public:
    Arc();

    void drawFunc() override;

    virtual bool setStartAngle(const LCreal x)  { startAngle = x; return true; }
    virtual bool setArcLength(const LCreal x)   { arcLength = x; return true; }
    virtual bool setIsConnected(const bool x)   { connected = x; return true; }

    LCreal getStartAngle()                      { return startAngle; }
    LCreal getArcLength()                       { return arcLength; }
    bool  isConnected()                         { return connected; }

protected:
    bool setSlotStartAngle(const Basic::Number* const x);
    bool setSlotArcLength(const Basic::Number* const x);
    bool setSlotIsConnected(const Basic::Number* const x);

private:
    LCreal startAngle;
    LCreal arcLength;
    bool  connected;
};

//------------------------------------------------------------------------------
// Class: OcclusionArc
//
// Factory name: OcclusionArc
// Slots:
//  outerRadius   <Number>    ! Outer radius of our circle (default: 1.1)
//
// Just like OcclusionCircle, only draws from a start angle to a finish angle
//------------------------------------------------------------------------------
class OcclusionArc : public Arc
{
    DECLARE_SUBCLASS(OcclusionArc,Arc)

public:
    OcclusionArc();

    void drawFunc() override;

    bool setOuterRadius(const LCreal x)  { outerRadius = x; return true; }

    LCreal getOuterRadius()              { return outerRadius; }

protected:
    bool setSlotOuterRadius(const Basic::Number* const x);

private:
    LCreal outerRadius;          // portion that is occluded
};


//------------------------------------------------------------------------------
// Class: Point
//
// Factory name: Point
//
// Public member functions:
//   void drawFunc() override
//   Draws a point for each vertex
//
//------------------------------------------------------------------------------
class Point : public Graphic
{
    DECLARE_SUBCLASS(Point,Graphic)
public:
    Point();
    void drawFunc() override;
};


//------------------------------------------------------------------------------
// Class: LineLoop
//
// Factory name: LineLoop
//
// Public member functions:
//    void drawFunc() override
//    Draws a line based on a set of vertices
//
//------------------------------------------------------------------------------
class LineLoop : public Graphic
{
    DECLARE_SUBCLASS(LineLoop,Graphic)
public:
    LineLoop();
    void drawFunc() override;
};

//------------------------------------------------------------------------------
// Class: Line
//
// Factory name: Line
// Slots:
//  segment   <Number>    ! True if line segments (default: false)
//
//
// Public member functions:
//
//  bool setSegments(Number* ssobj)
//      Sets segments (boolean) and returns true if successful.
//  bool isSegmented()
//      Returns number of segments.
//
//------------------------------------------------------------------------------
class Line : public Graphic
{
    DECLARE_SUBCLASS(Line,Graphic)
public:
    Line();

    void drawFunc() override;

    bool setSegments(const bool x)  { segment = x; return true; }

    bool isSegmented() { return segment; }

protected:
    bool setSlotSegments(const Basic::Number* const x);

private:
    bool segment;            // True if line segments
};

//------------------------------------------------------------------------------
// Class: Quad
//
// Factory name: Quad
// Description:
//      Simple openGL quad strip
//------------------------------------------------------------------------------
class Quad : public Graphic
{
    DECLARE_SUBCLASS(Quad,Graphic)
public:
    Quad();

    bool setStrip(const bool x)     { strip = x; return true; }

    bool isStrip()                  { return strip; }

    void drawFunc() override;

protected:
    bool setSlotStrip(const Basic::Number* const x);
    bool strip;     // are we a Quad Strip?

};

//------------------------------------------------------------------------------
// Class: Triangle
//
// Factory name: Triangle
// Description:
//      Simple openGL triangle primitive (or strip, or fan)
//------------------------------------------------------------------------------
class Triangle : public Quad
{
    DECLARE_SUBCLASS(Triangle,Quad)
public:
    Triangle();

    bool setFan(const bool x)     { fan = x; return true; }

    bool isFan()                  { return fan; }

    void drawFunc() override;

protected:
    bool setSlotFan(const Basic::Number* const x);

private:
    bool fan;       // are we a triangle fan?
};

} // End BasicGL namespace
} // End oe namespace

#endif
