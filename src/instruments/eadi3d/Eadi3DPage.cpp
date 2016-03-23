
#include "openeaagles/instruments/eadi3D/Eadi3DPage.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/graphics/Display.h"
#include "openeaagles/base/Number.h"

// setup local variables and static variables
#define RADIUS_180      1.625                   /* 180 in view */
#define RADIUS_160      1.650                   /* 160 in view */
#define RADIUS_140      1.729                   /* 140 in view */
#define RADIUS_120      1.877                   /* 120 in view */

namespace oe {
namespace instruments {

static double GROUND[3] = { 0.588235294f, 0.294117647f, 0.0f };
static double SKY[3] =    { 0.0f, 0.5f, 1.0f };
static double WHITE[3] =  { 1.0f, 1.0f, 1.0f };
static double BLACK[3] =  { 0.0f, 0.0f, 0.0f };
static double GREEN[3] =  { 0.0f, 1.0f, 0.0f };

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Eadi3DPage,"Eadi3DPage")
EMPTY_SERIALIZER(Eadi3DPage)

// Event handler
BEGIN_EVENT_HANDLER(Eadi3DPage)
    ON_EVENT_OBJ(USER_EVENTS+100, onEventSetAltitude, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+101, onEventSetAirspeed, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+102, onEventSetHeading, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+103, onEventSetAOA, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+104, onEventSetVVI, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+105, onEventSetPitch, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+106, onEventSetRoll, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+107, onEventSetMach, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+108, onEventSetGLoad, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+109, onEventSetLandingMode, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+110, onEventSetPitchSteeringCmd, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+111, onEventSetRollSteeringCmd, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+112, onEventSetPitchSteeringValid, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+113, onEventSetRollSteeringValid, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+114, onEventSetGlideslopeDev, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+115, onEventSetLocalizerDev, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+116, onEventSetTurnRate, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+117, onEventSetSlipInd, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+118, onEventSetGlideslopeValid, base::Number)
    ON_EVENT_OBJ(USER_EVENTS+119, onEventSetLocalizerValid, base::Number)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Eadi3DPage::Eadi3DPage() : radius(RADIUS_180), eadiObjs(RADIUS_180)
{
    STANDARD_CONSTRUCTOR()

    altitudeFT = 0.0;
    airspeedKTS = 0.0;
    headingDEG = 0.0;
    aoaDEG = 0.0;
    machNo = 0.0;
    vviFPM = 0.0;
    pitchDEG = 0.0;
    rollDEG = 0.0;
    Gload = 0.0;
    landingMode = false;
    pitchSteeringCmd = 0.0;
    rollSteeringCmd = 0.0;
    pitchSteeringValid = false;
    rollSteeringValid = false;
    glideslopeDevDOTS = 0.0;
    localizerDevDOTS = 0.0;
    turnRateDOTS = 0.0;
    slipIndDOTS = 0.0;
    glideslopeDevValid = false;
    localizerDevValid = false;
}


//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Eadi3DPage::copyData(const Eadi3DPage& org, const bool)
{
    BaseClass::copyData(org);

    radius = org.radius;
    altitudeFT = org.altitudeFT;
    airspeedKTS = org.airspeedKTS;
    headingDEG = org.headingDEG;
    aoaDEG = org.aoaDEG;
    machNo = org.machNo;
    vviFPM = org.vviFPM;
    pitchDEG = org.pitchDEG;
    rollDEG = org.rollDEG;
    Gload = org.Gload;
    landingMode = org.landingMode;
    pitchSteeringCmd = org.pitchSteeringCmd;
    rollSteeringCmd = org.rollSteeringCmd;
    pitchSteeringValid = org.pitchSteeringValid;
    rollSteeringValid = org.rollSteeringValid;
    glideslopeDevDOTS = org.glideslopeDevDOTS;
    localizerDevDOTS = org.localizerDevDOTS;
    turnRateDOTS = org.turnRateDOTS;
    slipIndDOTS = org.slipIndDOTS;
    glideslopeDevValid = org.glideslopeDevValid;
    localizerDevValid = org.localizerDevValid;
}


//------------------------------------------------------------------------------
// deleteData() -
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(Eadi3DPage)

// Event functions
//------------------------------------------------------------------------------
// onEventSetAltitude() - set altitude
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetAltitude(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        altitudeFT = x->getReal();
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetAirspeed() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetAirspeed(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        airspeedKTS = x->getReal();
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetHeading() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetHeading(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        headingDEG = x->getReal();
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetAOA() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetAOA(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        aoaDEG = x->getReal();
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetMach() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetMach(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        machNo = x->getReal();
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetVVI() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetVVI(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        vviFPM = x->getReal();
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetPitch() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetPitch(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        pitchDEG = x->getReal();
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetRoll() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetRoll(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        rollDEG = x->getReal();
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetGLoad() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetGLoad(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        Gload = x->getReal();
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetLandingMode() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetLandingMode(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        landingMode = x->getReal() != 0.0;
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetPitchSteeringCmd() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetPitchSteeringCmd(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        pitchSteeringCmd = x->getReal();
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetRollSteeringCmd() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetRollSteeringCmd(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        rollSteeringCmd = x->getReal();
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetPitchSteeringValid() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetPitchSteeringValid(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        pitchSteeringValid = x->getReal() != 0.0;
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetRollSteeringValid() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetRollSteeringValid(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        rollSteeringValid = x->getReal() != 0.0;
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetGlideslopeDev() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetGlideslopeDev(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        glideslopeDevDOTS = x->getReal();
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetLocalizerDev() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetLocalizerDev(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        localizerDevDOTS = x->getReal();
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetTurnRate() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetTurnRate(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        turnRateDOTS = x->getReal();
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetSlipInd() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetSlipInd(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        slipIndDOTS = x->getReal();
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetGlideslopeValid() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetGlideslopeValid(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        glideslopeDevValid = x->getReal() != 0;
    }
    return ok;
}

//------------------------------------------------------------------------------
// onEventSetLocalizerValid() -
//------------------------------------------------------------------------------
bool Eadi3DPage::onEventSetLocalizerValid(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = true;
        localizerDevValid = x->getReal() != 0.0;
    }
    return ok;
}


//------------------------------------------------------------------------------
// draw() --
//------------------------------------------------------------------------------
void Eadi3DPage::draw()
{
    graphics::Display* dsp = getDisplay();
    if (dsp != nullptr) {

        eadiObjs.makeObjects();

        GLsizei vpWidth;
        GLsizei vpHeight;
        dsp->getViewportSize(&vpWidth, &vpHeight);
        double ratio = static_cast<double>(vpWidth) / static_cast<double>(vpHeight);

        bool depthTest = false;
        if (glIsEnabled(GL_DEPTH_TEST))
            depthTest = true;

        if (depthTest)
            glDisable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(-2.635 * ratio, 2.635 * ratio, -2.635, 2.635, -3.51, 0.01);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        globeBall(pitchDEG, rollDEG, pitchSteeringCmd, rollSteeringCmd, pitchSteeringValid, rollSteeringValid, landingMode);

        background();

        const char* airSpeedType = "C";
        scales(glideslopeDevDOTS, localizerDevDOTS, turnRateDOTS, slipIndDOTS, glideslopeDevValid, localizerDevValid, landingMode);
        windows(airspeedKTS, altitudeFT, aoaDEG, machNo, vviFPM, airSpeedType, Gload);
        double hdgCmd = 0.0;
        heading(headingDEG, hdgCmd);

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        if (depthTest)
            glEnable(GL_DEPTH_TEST);

        // now draw our components
        BaseClass::draw();
    }
}


//------------------------------------------------------------------------------
// globeBall() - draw the globe given specific input parameters
//------------------------------------------------------------------------------
void Eadi3DPage::globeBall(double pitch, double roll, double pitchCmd, double rollCmd, bool psValid, bool rcValid, bool landMode)
{
    static double    fdPscV1[2] = {-0.5833f, 0.0f};
    static double    fdPscV2[2] = {0.5833f, 0.0f};
    static double    fdBscV1[2] = {0.0f, -0.5833f};
    static double    fdBscV2[2] = {0.0f, 0.5833f};

    glLineWidth(1.0);

    glPushMatrix();
    glRotated(static_cast<GLdouble>(roll), 0.0, 0.0, 1.0);
    lcColor3v(GROUND);
    eadiObjs.irisgl.circf(0., 0., 1.625);
    lcColor3v(SKY);
    eadiObjs.irisgl.arcf(0., 0., 1.625, 0, 180.0);
    glPopMatrix();

    glPushMatrix();
    glRotated(static_cast<GLdouble>(roll),  0.0, 0.0, 1.0);
    glRotated(static_cast<GLdouble>(pitch), 1.0, 0.0, 0.0);
    glPushMatrix();
            if (pitch >= 0.0) lcColor3v(SKY);
            else lcColor3v(GROUND);
    glRotated(90.0, 1.0, 0.0, 0.0);
            /* arcf(0.,0.,radius,180.0,0); */
            eadiObjs.irisgl.circf(0., 0., static_cast<float>(radius));
    glPopMatrix();
    eadiObjs.drawObj(Eadi3DObjects::GLOBE);
    glPopMatrix();

    glLineWidth(1.0);
    lcColor3v(BLACK);
    eadiObjs.irisgl.washerf(0., 0., 1.625, 2.0);
    glLineWidth(2.0);
    lcColor3v(WHITE);
    eadiObjs.irisgl.circ(0., 0., 1.625);

    glPushMatrix();    /* ground pointer */
    glRotated(static_cast<GLdouble>(roll), 0.0, 0.0, 1.0);
    glTranslated(0.0, -1.625, 0.0);
    eadiObjs.drawObj(Eadi3DObjects::GROUND_POINTER_2);
    glPopMatrix();


    lcColor3v(BLACK);
    /* lcColor3v(WHITE);   * temp */
    glLineWidth(4.0);
    eadiObjs.drawObj(Eadi3DObjects::AC_REF);    /* aircraft reference symbol */
    glLineWidth(2.0);

    if (landMode) {
        if (psValid) {
            double x = static_cast<double>(pitchCmd / 30.0 * 1.625);   /* pitch fd command */
            glPushMatrix();
            glTranslated(0.0, static_cast<GLdouble>(x), 0.0);
            glBegin(GL_LINES);
                    lcVertex2v(fdPscV1);
                    lcVertex2v(fdPscV2);
            glEnd();
            glPopMatrix();
        }
        if (rcValid) {
            double x = static_cast<double>(rollCmd / 30.0 * 1.625);    /* roll fd command */
            glPushMatrix();
            glTranslated(static_cast<GLdouble>(x), 0.0, 0.0);
            glBegin(GL_LINES);
                    lcVertex2v(fdBscV1);
                    lcVertex2v(fdBscV2);
            glEnd();
            glPopMatrix();
        }
    }
    glLineWidth(1.0);
}


//------------------------------------------------------------------------------
// scales() - draw the scales
//------------------------------------------------------------------------------
void Eadi3DPage::scales(double gsDev, double locDev, double turnRate, double slipInd, bool gsValid, bool locValid, bool landMode)
{

    if (landMode) {
        glLineWidth(2.0);
        lcColor3v(GREEN);

        eadiObjs.drawObj(Eadi3DObjects::GLIDESLOPE);

        if (gsValid) {
            double x = static_cast<double>(gsDev / 2.0);
            glPushMatrix();
            glTranslated(-2.0, static_cast<GLdouble>(x), 0.);
            eadiObjs.drawObj(Eadi3DObjects::GS_TEXT1);
            glPopMatrix();
        }
        else
            eadiObjs.drawObj(Eadi3DObjects::GS_TEXT2);

        eadiObjs.drawObj(Eadi3DObjects::LOCALIZER);

        if (locValid) {
            double x = static_cast<double>(locDev / 2.0);
            glPushMatrix();
            glTranslated(static_cast<GLdouble>(x), 2.0, 0.);
            eadiObjs.drawObj(Eadi3DObjects::LOC_TEXT1);
            glPopMatrix();
        }
        else eadiObjs.drawObj(Eadi3DObjects::LOC_TEXT2);
    }
    glLineWidth(2.0);
    lcColor3v(WHITE);

    double x = static_cast<double>(turnRate / 2.0 * 0.8125);

    glPushMatrix();
    glTranslated(static_cast<GLdouble>(x), -2.125, 0.);
    eadiObjs.drawObj(Eadi3DObjects::UP_TRIANGLE);
    glPopMatrix();

    x = static_cast<double>(slipInd / 2. * 0.40625);
    glPushMatrix();
    glTranslated(static_cast<GLdouble>(x), -2.4375, 0.);
    eadiObjs.drawObj(Eadi3DObjects::SLIP_BALL);
    glPopMatrix();

    glLineWidth(1.0);
}

//------------------------------------------------------------------------------
// windows() -
//------------------------------------------------------------------------------
void Eadi3DPage::windows(double cas, double alt, double aoa, double mach, double vvi, const char* airSpeedType, double gload)
{
    double a = cas;
    if (a > 9999.) a = 9999.;
    if (a < 0.) a = 0.;

    int airC[4];
    airC[0] = static_cast<int>(a) / 1000;
    a -= static_cast<double>(airC[0] * 1000);

    airC[1] = static_cast<int>(a) / 100;
    a -= static_cast<double>(airC[1] * 100);

    airC[2] = static_cast<int>(a) / 10;
    a -= static_cast<double>(airC[2] * 10);

    airC[3] = static_cast<int>(a);
    for (int i = 0, j = 1; (i < 3) & j; i++) {
        if (airC[i] == 0) airC[i] = -1;
        else j = 0;
    }

    a = alt;
    if (a > 99999.) a = 99999.;
    if (a < 0.) a = 0.;

    int altC[5];
    altC[0] = static_cast<int>(a) / 10000;
    a -= static_cast<double>(altC[0] * 10000);

    altC[1] = static_cast<int>(a) / 1000;
    a -= static_cast<double>(altC[1] * 1000);
    altC[2] = static_cast<int>(a) / 100;
    a -= static_cast<double>(altC[2] * 100);
    altC[3] = static_cast<int>(a) / 10;
    a -= static_cast<double>(altC[3] * 10);
    altC[4] = static_cast<int>(a);
    for (int i = 0, j = 1; (i < 4) & j; i++) {
        if (altC[i] == 0) altC[i] = -1;
        else j = 0;
    }

    a = static_cast<double>(aoa * 10.0);
    int aoaC[4];
    if (a < 0) {
        a = -a;
        aoaC[0] = Eadi3DObjects::PUNC05;
    }
    else aoaC[0] = -1;
    if (a > 999.) a = 999.;
    aoaC[1] = static_cast<int>(a) / 100;
    a -= static_cast<double>(aoaC[1] * 100);

    aoaC[2] = static_cast<int>(a) / 10;
    a -= static_cast<double>(aoaC[2] * 10);

    aoaC[3] = static_cast<int>(a);
    for (int i = 1, j = 1; (i < 3) & j; i++) {
        if (aoaC[i] == 0) {
            if (aoaC[i - 1] == Eadi3DObjects::PUNC05) {
                aoaC[i - 1] = -1;
                aoaC[i] = Eadi3DObjects::PUNC05;
            }
            else aoaC[i] = -1;
        }
        else j = 0;
    }

    a = static_cast<double>(mach * 100.0);
    int machC[3];
    if (a < 0) {
        a = 0.0;
    }
    if (a > 999.) {
        a = 999.;
    }
    machC[0] = static_cast<int>(a) / 100;
    a -= static_cast<double>(machC[0] * 100);

    machC[1] = static_cast<int>(a) / 10;
    a -= static_cast<double>(machC[1] * 10);

    machC[2] = static_cast<int>(a);
    a = static_cast<double>(gload * 10.0);
    int gloadC[2];

    if (a < 0) a = 0.0;
    if (a > 99.) a = 99.;

    gloadC[0] = static_cast<int>(a) / 10;
    a -= static_cast<double>(gloadC[0] * 10);

    gloadC[1] = static_cast<int>(a);

    a = vvi;
    int vviC[5];
    if (a < 0) {
        a = -a;
        vviC[0] = Eadi3DObjects::PUNC05;
    }
    else vviC[0] = -1;

    if (a > 9999.) a = 9999.;

    vviC[1] = static_cast<int>(a) / 1000;
    a -= static_cast<double>(vviC[1] * 1000);

    vviC[2] = static_cast<int>(a) / 100;
    a -= static_cast<double>(vviC[2] * 100);

    vviC[3] = static_cast<int>(a) / 10;
    a -= static_cast<double>(vviC[3] * 10);

    vviC[4] = static_cast<int>(a);

    for (int i = 1, j = 1; (i < 4) & j; i++) {
        if (vviC[i] == 0) {
            if (vviC[i - 1] == Eadi3DObjects::PUNC05) {
                vviC[i - 1] = -1;
                vviC[i] = Eadi3DObjects::PUNC05;
            }
            else vviC[i] = -1;
        }
        else j = 0;
    }

    lcColor3v(WHITE);
    glLineWidth(2.0);

    /* cas */
    glPushMatrix();
    glTranslated(-2.4375, 1.59375, 0.);
        if (airSpeedType != nullptr) {
            if (airSpeedType[0] == 'T') eadiObjs.drawObj(Eadi3DObjects::FONT_T);
            else if (airSpeedType[0] == 'G') eadiObjs.drawObj(Eadi3DObjects::FONT_G);
            else eadiObjs.drawObj(Eadi3DObjects::FONT_C);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslated(-2.4375, 1.59375, 0.0);
    glTranslated(0.2404, 0.0, 0.0);
        if (airC[0] != -1) eadiObjs.drawObj((Eadi3DObjects::EadiObj) airC[0]);
    glTranslated(0.16, 0.0, 0.0);

        if (airC[1] != -1) eadiObjs.drawObj((Eadi3DObjects::EadiObj) airC[1]);
    glTranslated(0.16, 0.0, 0.0);

        if (airC[2] != -1) eadiObjs.drawObj((Eadi3DObjects::EadiObj) airC[2]);
    glTranslated(0.16, 0.0, 0.0);

        eadiObjs.drawObj((Eadi3DObjects::EadiObj) airC[3]);
    glPopMatrix();

    /* alt */
    glPushMatrix();
    glTranslated(1.3625, 1.59375, 0.0);
    glTranslated(0.2404, 0.0, 0.0);
        if (altC[0] != -1) eadiObjs.drawObj((Eadi3DObjects::EadiObj) altC[0]);

    glTranslated(0.16, 0.0, 0.0);

        if (altC[1] != -1) eadiObjs.drawObj((Eadi3DObjects::EadiObj) altC[1]);

    glTranslated(0.16, 0.03125, 0.0);
    glScaled(.666667, .666667, 1.0);

        if (altC[2] != -1) eadiObjs.drawObj((Eadi3DObjects::EadiObj) altC[2]);

    glTranslated(0.16, 0.0, 0.0);

        if (altC[3] != -1) eadiObjs.drawObj((Eadi3DObjects::EadiObj) altC[3]);

    glTranslated(0.16, 0.0, 0.0);

        eadiObjs.drawObj((Eadi3DObjects::EadiObj) altC[4]);
    glPopMatrix();

    /* aoa */
    glPushMatrix();
    glTranslated(-2.3000, 1.34375, 0.0);
    glTranslated(0.1900, 0.03125, 0.0);
    glScaled(.666667, .666667, 1.0);

        if (aoaC[0] != -1) eadiObjs.drawObj((Eadi3DObjects::EadiObj) aoaC[0]);

    glTranslated(0.16, 0.0, 0.0);

        if (aoaC[1] != -1) eadiObjs.drawObj((Eadi3DObjects::EadiObj) aoaC[1]);

    glTranslated(0.16, 0.0, 0.0);

        if (aoaC[2] != -1) eadiObjs.drawObj((Eadi3DObjects::EadiObj) aoaC[2]);

    glTranslated(0.16, 0.0, 0.0);
    eadiObjs.drawObj(Eadi3DObjects::PUNC06);
    glTranslated(0.16, 0.0, 0.0);
        eadiObjs.drawObj((Eadi3DObjects::EadiObj) aoaC[3]);
    glPopMatrix();

    /* mach */
    glPushMatrix();
    glTranslated(-2.3000, 1.09375, 0.0);
    glTranslated(0.2800, 0.03125, 0.0);
    glScaled(.666667, .666667, 1.0);
        eadiObjs.drawObj((Eadi3DObjects::EadiObj) machC[0]);
    glTranslated(0.16, 0.0, 0.0);
    eadiObjs.drawObj(Eadi3DObjects::PUNC06);
    glTranslated(0.16, 0.0, 0.0);
        eadiObjs.drawObj((Eadi3DObjects::EadiObj) machC[1]);
    glTranslated(0.16, 0.0, 0.0);
        eadiObjs.drawObj((Eadi3DObjects::EadiObj) machC[2]);
    glPopMatrix();

    /* G */
    glPushMatrix();
    glTranslated(-2.3000, 0.84375, 0.0);
    glTranslated(0.3600, 0.03125, 0.0);
    glScaled(.666667, .666667, 1.0);
        eadiObjs.drawObj((Eadi3DObjects::EadiObj) gloadC[0]);
    glTranslated(0.16, 0.0, 0.0);
    eadiObjs.drawObj(Eadi3DObjects::PUNC06);
    glTranslated(0.16, 0.0, 0.0);
        eadiObjs.drawObj((Eadi3DObjects::EadiObj) gloadC[1]);
    glPopMatrix();

    /* vvi */
    glPushMatrix();
    glTranslated(1.3625, 1.34375, 0.0);
    glTranslated(0.1607, 0.0, 0.0);
    glTranslated(0.1900, 0.03125, 0.0);
    glScaled(.666667, .666667, 1.0);
        if (vviC[0] != -1) eadiObjs.drawObj((Eadi3DObjects::EadiObj) vviC[0]);

    glTranslated(0.16, 0.0, 0.0);
        if (vviC[1] != -1) eadiObjs.drawObj((Eadi3DObjects::EadiObj) vviC[1]);

    glTranslated(0.16, 0.0, 0.0);
        if (vviC[2] != -1) eadiObjs.drawObj((Eadi3DObjects::EadiObj) vviC[2]);

    glTranslated(0.16, 0.0, 0.0);
        if (vviC[3] != -1) eadiObjs.drawObj((Eadi3DObjects::EadiObj) vviC[3]);

    glTranslated(0.16, 0.0, 0.0);
        eadiObjs.drawObj((Eadi3DObjects::EadiObj) vviC[4]);
    glPopMatrix();

    glLineWidth(1.0);
}

//------------------------------------------------------------------------------
// heading() -
//------------------------------------------------------------------------------
void Eadi3DPage::heading(double hdg, double hdgCmd)
{
    static double baseLineV1[2] = {-1.6f, 2.1875f};   /* base line */
    static double baseLineV2[2] = {1.6f, 2.1875f};
    static double refTicV1[2] = {0.0f, 2.1875f};      /* reference tic */
    static double refTicV2[2] = {0.0f, 2.125f};


    glLineWidth(2.0);
    lcColor3v(BLACK);
    glRectf(-1.6f, 2.125f, 1.6f, 2.5f);

    lcColor3v(WHITE);

    if (hdg < 0.0) hdg += 360.0;
    double x = static_cast<double>(hdg / 10.0);
    int i = static_cast<int>(x);
    x = static_cast<double>(i) - x;

    int ihu[4];
    int ihl[4];
    for (int j = 0; j < 4; j++) {
        int ih = i + j - 1;
        if (ih <= 0) ih += 36;
        if (ih > 36) ih -= 36;

        ihu[j] = ih / 10;
        ihl[j] = ih - ihu[j] * 10;
    }

    glPushMatrix();

    glBegin(GL_LINES);
            lcVertex2v(baseLineV1);
            lcVertex2v(baseLineV2);
    glEnd();

    glTranslated(static_cast<GLdouble>(x), 0.0, 0.0);

    eadiObjs.drawObj(Eadi3DObjects::HEADING_SCALE);

    for (i = 0, x = -1.; i < 4; i++, x++) {
        glPushMatrix();
        glTranslated(static_cast<GLdouble>(x), 2.28, 0.0);
        glPushMatrix();
        glTranslated(-0.137142857, 0.0, 0.0);
        eadiObjs.drawObj((Eadi3DObjects::EadiObj) ihu[i]);
        glPopMatrix();
        glTranslated(0.03, 0.0, 0.0);
        eadiObjs.drawObj((Eadi3DObjects::EadiObj) ihl[i]);
        glPopMatrix();
    }

    glPopMatrix();

    glLineWidth(4.0);
    glBegin(GL_LINES);
        lcVertex2v(refTicV1);
        lcVertex2v(refTicV2);
    glEnd();
    glLineWidth(2.0);

    x = static_cast<double>(hdgCmd / 10.0);
    lcColor3v(GREEN);
    glPushMatrix();
    glTranslated(static_cast<GLdouble>(x), 2.1875, 0.0);
    eadiObjs.drawObj(Eadi3DObjects::UP_CARET);
    glPopMatrix();

    glLineWidth(2.0);
    lcColor3v(BLACK);
    glRectf(1.6f, 2.125f, 3.3f, 2.5f);
    glRectf(-3.3f, 2.125f, -1.6f, 2.5f);

    glLineWidth(1.0);
}

//------------------------------------------------------------------------------
// background() -
//------------------------------------------------------------------------------
void Eadi3DPage::background()
{
    glLineWidth(2.0);

    lcColor3v(WHITE);
    eadiObjs.drawObj(Eadi3DObjects::TURN_RATE);
    eadiObjs.drawObj(Eadi3DObjects::SLIP_IND);

    eadiObjs.irisgl.rect(-2.25f, 1.5625f, -1.55f, 1.8125f);   /* cas */
    eadiObjs.irisgl.rect(1.55f, 1.5625f, 2.25f, 1.8125f);     /* alt */
    glPushMatrix();
    glTranslated(1.3625, 1.59375, 0.0);
    eadiObjs.drawObj(Eadi3DObjects::FONT_B);
    glPopMatrix();

    glPushMatrix();     /* aoa */
    glTranslated(-2.3000, 1.34375, 0.0);
    eadiObjs.drawObj(Eadi3DObjects::ALPHA_SYMB);
    glPopMatrix();

    glPushMatrix();    /* mach */
    glTranslated(-2.3000, 1.09375, 0.0);
    glTranslated(0.0, 0.03125, 0.0);
    glScaled(.666667, .666667, 1.0);
    eadiObjs.drawObj(Eadi3DObjects::FONT_M);
    glPopMatrix();

    glPushMatrix();    /* G */
    glTranslated(-2.3000, 0.84375, 0.0);
    glTranslated(0.0, 0.03125, 0.0);
    glScaled(.666667, .666667, 1.0);
    eadiObjs.drawObj(Eadi3DObjects::FONT_G);
    glPopMatrix();

    glPushMatrix();    /* vvi */
    glTranslated(1.3625, 1.34375, 0.0);
    eadiObjs.drawObj(Eadi3DObjects::FONT_V);
    glTranslated(0.1607, 0.0, 0.0);
    eadiObjs.drawObj(Eadi3DObjects::FONT_V);
    glPopMatrix();

    glLineWidth(2.0);
    eadiObjs.drawObj(Eadi3DObjects::BALL_FRAME);
    glLineWidth(1.0);
}

//------------------------------------------------------------------------------
// setAltitude() - sets our altitude
//------------------------------------------------------------------------------
void Eadi3DPage::setAltitude(const double alt)
{
    altitudeFT = alt;
}

//------------------------------------------------------------------------------
// setAirspeed() -
//------------------------------------------------------------------------------
void Eadi3DPage::setAirspeed(const double spd)
{
    airspeedKTS = spd;
}

//------------------------------------------------------------------------------
// setHeading() -
//------------------------------------------------------------------------------
void Eadi3DPage::setHeading(const double hdg)
{
    headingDEG = hdg;
}

//------------------------------------------------------------------------------
// setAOA() -
//------------------------------------------------------------------------------
void Eadi3DPage::setAOA(const double aoa)
{
    aoaDEG = aoa;
}

//------------------------------------------------------------------------------
// setMach() -
//------------------------------------------------------------------------------
void Eadi3DPage::setMach(const double mach)
{
    machNo = mach;
}

//------------------------------------------------------------------------------
// setVVI() -
//------------------------------------------------------------------------------
void Eadi3DPage::setVVI(const double vvi)
{
    vviFPM = vvi;
}

//------------------------------------------------------------------------------
// setPitch() -
//------------------------------------------------------------------------------
void Eadi3DPage::setPitch(const double pitch)
{
    pitchDEG = pitch;
}

//------------------------------------------------------------------------------
// setRoll() -
//------------------------------------------------------------------------------
void Eadi3DPage::setRoll(const double roll)
{
    rollDEG = roll;
}

//------------------------------------------------------------------------------
// setGLoad() -
//------------------------------------------------------------------------------
void Eadi3DPage::setGLoad(const double gload)
{
    Gload = gload;
}

//------------------------------------------------------------------------------
// setLandingMode() -
//------------------------------------------------------------------------------
void Eadi3DPage::setLandingMode(const bool landMode)
{
    landingMode = landMode;
}

//------------------------------------------------------------------------------
// setPitchSteeringCmd() -
//------------------------------------------------------------------------------
void Eadi3DPage::setPitchSteeringCmd(const double pitchCmd)
{
     pitchSteeringCmd = pitchCmd;
}

//------------------------------------------------------------------------------
// setRollingSteeringCmd() -
//------------------------------------------------------------------------------
void Eadi3DPage::setRollSteeringCmd(const double rollCmd)
{
    rollSteeringCmd = rollCmd;
}

//------------------------------------------------------------------------------
// setPitchSteeringValid() -
//------------------------------------------------------------------------------
void Eadi3DPage::setPitchSteeringValid(const bool psValid)
{
     pitchSteeringValid = psValid;
}

//------------------------------------------------------------------------------
// setRollSteeringValid() -
//------------------------------------------------------------------------------
void Eadi3DPage::setRollSteeringValid(const bool rcValid)
{
    rollSteeringValid = rcValid;
}

//------------------------------------------------------------------------------
// setGlideslopeDev() -
//------------------------------------------------------------------------------
void Eadi3DPage::setGlideslopeDev(const double gsDev)
{
    glideslopeDevDOTS = gsDev;
}

//------------------------------------------------------------------------------
// setLocalizerDev() -
//------------------------------------------------------------------------------
void Eadi3DPage::setLocalizerDev(const double locdev)
{
    localizerDevDOTS = locdev;
}

//------------------------------------------------------------------------------
// setTurnRate() -
//------------------------------------------------------------------------------
void Eadi3DPage::setTurnRate(const double turnRate)
{
    turnRateDOTS = turnRate;
}

//------------------------------------------------------------------------------
// setSlipInd() -
//------------------------------------------------------------------------------
void Eadi3DPage::setSlipInd(const double slipInd)
{
    slipIndDOTS = slipInd;
}

//------------------------------------------------------------------------------
// setGlideslopeValid() -
//------------------------------------------------------------------------------
void Eadi3DPage::setGlideslopeValid(const bool gsValid)
{
    glideslopeDevValid = gsValid;
}

//------------------------------------------------------------------------------
// setLocalizerValid() -
//------------------------------------------------------------------------------
void Eadi3DPage::setLocalizerValid(const bool locValid)
{
    localizerDevValid = locValid;
}

}; // end instruments namespace
}; // end oe namespace
