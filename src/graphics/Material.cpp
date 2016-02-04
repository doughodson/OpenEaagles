#include "openeaagles/graphics/Material.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

namespace oe {
namespace BasicGL {

IMPLEMENT_SUBCLASS(Material,"Material")
EMPTY_SERIALIZER(Material)

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Material)
    "ambientColor",         //  1: Ambient color - light coming from material scattered in all directions equally
    "diffuseColor",         //  2: Diffuse color - light coming from the material after light hits it (main color)
    "emissiveColor",        //  3: Emissive color - light emanating from the material
    "specularColor",        //  4: Specular color - mirror reflecting color of the material (shiny highlights)
    "shininess",            //  5: How shiny our material is (0 = dull, 128 = really shiny)
END_SLOTTABLE(Material)

//------------------------------------------------------------------------------
//  Map slot table to handles
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(Material)
    ON_SLOT( 1, setSlotAmbientColor, basic::PairStream )
    ON_SLOT( 1, setSlotAmbientColor, basic::Number)
    ON_SLOT( 2, setSlotDiffuseColor, basic::PairStream )
    ON_SLOT( 2, setSlotDiffuseColor, basic::Number)
    ON_SLOT( 3, setSlotEmissiveColor, basic::PairStream )
    ON_SLOT( 3, setSlotEmissiveColor, basic::Number)
    ON_SLOT( 4, setSlotSpecularColor, basic::PairStream )
    ON_SLOT( 4, setSlotSpecularColor, basic::Number)
    ON_SLOT( 5, setSlotShininess, basic::Number)
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Material::Material()
{
    STANDARD_CONSTRUCTOR()
    ambientColor.set(0,0,0,1);      // no ambient color from the material
    diffuseColor.set(1,1,1,1);      // white light diffuse off of the material (this is the closest thing to actual material color)
    emissiveColor.set(0,0,0,1);     // no emissive color
    specularColor.set(0,0,0,1);     // no specular color
    shininess = 0;                  // no shine
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Material::copyData(const Material& org, const bool)
{
    BaseClass::copyData(org);
    ambientColor = org.ambientColor;
    diffuseColor = org.diffuseColor;
    emissiveColor = org.emissiveColor;
    specularColor = org.specularColor;
    shininess = org.shininess;
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Material::deleteData()
{
}

//------------------------------------------------------------------------------
// setSlotAmbientColor() - give us a list of numbers (0 - 1) for the ambient
// values (red, green, blue, and alpha) of our material.
//------------------------------------------------------------------------------
bool Material::setSlotAmbientColor(const basic::PairStream* const x)
{
    if (x != nullptr) {
        // go through and get our data from the pairstream, and set our ambient color
        const basic::List::Item* item = x->getFirstItem();
        int count = 0;
        LCreal temp[4] = { 1,1,1,1 };
        while (item != nullptr && count < 4) {
            basic::Pair* pair = const_cast<basic::Pair*>(static_cast<const basic::Pair*>(item->getValue()));
            if (pair != nullptr) {
                basic::Number* num = dynamic_cast<basic::Number*>(pair->object());
                if (num != nullptr) temp[count++] = num->getFloat();
            }
            item = item->getNext();
        }
        // if we only put in red, green, and blue, we will default to 1 on alpha
        if (count == 3) temp[3] = 1;
        // now set our vector
        ambientColor.set(temp[0], temp[1], temp[2], temp[3]);
    }
    return true;
}
//------------------------------------------------------------------------------
// setSlotAmbientColor() - set our ambient values all the same as the number
// given (ie - 1 would be white, 0 black, 0.5 somewhere in between)
// ALPHA here is always 1.
//------------------------------------------------------------------------------
bool Material::setSlotAmbientColor(const basic::Number* const x)
{
    if (x != nullptr) {
        LCreal temp = x->getFloat();
        ambientColor.set(temp, temp, temp, 1);
    }
    return true;
}

//------------------------------------------------------------------------------
// setSlotDiffuseColor() - give us a list of numbers (0 - 1) for the diffuse
// values (red, green, blue, and alpha) of our material.
//------------------------------------------------------------------------------
bool Material::setSlotDiffuseColor(const basic::PairStream* const x)
{
    if (x != nullptr) {
        // go through and get our data from the pairstream, and set our ambient color
        const basic::List::Item* item = x->getFirstItem();
        int count = 0;
        LCreal temp[4] = { 1,1,1,1 };
        while (item != nullptr && count < 4) {
            basic::Pair* pair = const_cast<basic::Pair*>(static_cast<const basic::Pair*>(item->getValue()));
            if (pair != nullptr) {
                basic::Number* num = dynamic_cast<basic::Number*>(pair->object());
                if (num != nullptr) temp[count++] = num->getFloat();
            }
            item = item->getNext();
        }
        // if we only put in red, green, and blue, we will default to 1 on alpha
        if (count == 3) temp[3] = 1;
        // now set our vector
        diffuseColor.set(temp[0], temp[1], temp[2], temp[3]);
    }
    return true;
}
//------------------------------------------------------------------------------
// setSlotDiffuseColor() - set our diffuse values all the same as the number
// given (ie - 1 would be white, 0 black, 0.5 somewhere in between)
// ALPHA here is always 1.
//------------------------------------------------------------------------------
bool Material::setSlotDiffuseColor(const basic::Number* const x)
{
    if (x != nullptr) {
        LCreal temp = x->getFloat();
        diffuseColor.set(temp, temp, temp, 1);
    }
    return true;
}

//------------------------------------------------------------------------------
// setSlotEmissiveColor() - give us a list of numbers (0 - 1) for the emissive
// values (red, green, blue, and alpha) of our material.
//------------------------------------------------------------------------------
bool Material::setSlotEmissiveColor(const basic::PairStream* const x)
{
    if (x != nullptr) {
        // go through and get our data from the pairstream, and set our ambient color
        const basic::List::Item* item = x->getFirstItem();
        int count = 0;
        LCreal temp[4] = { 1,1,1,1 };
        while (item != nullptr && count < 4) {
            basic::Pair* pair = const_cast<basic::Pair*>(static_cast<const basic::Pair*>(item->getValue()));
            if (pair != nullptr) {
                basic::Number* num = dynamic_cast<basic::Number*>(pair->object());
                if (num != nullptr) temp[count++] = num->getFloat();
            }
            item = item->getNext();
        }
        // if we only put in red, green, and blue, we will default to 1 on alpha
        if (count == 3) temp[3] = 1;
        // now set our vector
        emissiveColor.set(temp[0], temp[1], temp[2], temp[3]);
    }
    return true;
}
//------------------------------------------------------------------------------
// setSlotEmissiveColor() - set our emissive values all the same as the number
// given (ie - 1 would be white, 0 black, 0.5 somewhere in between)
// ALPHA here is always 1.
//------------------------------------------------------------------------------
bool Material::setSlotEmissiveColor(const basic::Number* const x)
{
    if (x != nullptr) {
        LCreal temp = x->getFloat();
        emissiveColor.set(temp, temp, temp, 1);
    }
    return true;
}

//------------------------------------------------------------------------------
// setSlotSpecularColor() - give us a list of numbers (0 - 1) for the specular
// values (red, green, blue, and alpha) of our material.
//------------------------------------------------------------------------------
bool Material::setSlotSpecularColor(const basic::PairStream* const x)
{
    if (x != nullptr) {
        // go through and get our data from the pairstream, and set our ambient color
        const basic::List::Item* item = x->getFirstItem();
        int count = 0;
        LCreal temp[4] = { 1,1,1,1 };
        while (item != nullptr && count < 4) {
            basic::Pair* pair = const_cast<basic::Pair*>(static_cast<const basic::Pair*>(item->getValue()));
            if (pair != nullptr) {
                basic::Number* num = dynamic_cast<basic::Number*>(pair->object());
                if (num != nullptr) temp[count++] = num->getFloat();
            }
            item = item->getNext();
        }
        // if we only put in red, green, and blue, we will default to 1 on alpha
        if (count == 3) temp[3] = 1;
        // now set our vector
        specularColor.set(temp[0], temp[1], temp[2], temp[3]);
    }
    return true;
}
//------------------------------------------------------------------------------
// setSlotSpecularColor() - set our specular values all the same as the number
// given (ie - 1 would be white, 0 black, 0.5 somewhere in between)
// ALPHA here is always 1.
//------------------------------------------------------------------------------
bool Material::setSlotSpecularColor(const basic::Number* const x)
{
    if (x != nullptr) {
        LCreal temp = x->getFloat();
        specularColor.set(temp, temp, temp, 1);
    }
    return true;
}
//------------------------------------------------------------------------------
// setSlotShininess() - sets the shininess of our material
//------------------------------------------------------------------------------
bool Material::setSlotShininess(const basic::Number* const x)
{
    if (x != nullptr) {
        shininess = x->getReal();
    }
    return true;
}

//------------------------------------------------------------------------------
// setAmbientColor() - set our ambient color with an OSG or LCreal
//------------------------------------------------------------------------------
bool Material::setAmbientColor(osg::Vec4 x)
{
    ambientColor = x;
    return true;
}
bool Material::setAmbientColor(LCreal x, LCreal y, LCreal z, LCreal w)
{
    ambientColor.set(x, y, z, w);
    return true;
}
//------------------------------------------------------------------------------
// setDiffuseColor() - set our diffuse color with an OSG
//------------------------------------------------------------------------------
bool Material::setDiffuseColor(osg::Vec4 x)
{
    diffuseColor = x;
    return true;
}
bool Material::setDiffuseColor(LCreal x, LCreal y, LCreal z, LCreal w)
{
    diffuseColor.set(x, y, z, w);
    return true;
}
//------------------------------------------------------------------------------
// setEmissiveColor() - set our emissive color with an OSG
//------------------------------------------------------------------------------
bool Material::setEmissiveColor(osg::Vec4 x)
{
    emissiveColor = x;
    return true;
}
bool Material::setEmissiveColor(LCreal x, LCreal y, LCreal z, LCreal w)
{
    emissiveColor.set(x, y, z, w);
    return true;
}
//------------------------------------------------------------------------------
// setSpecularColor() - set our specular color with an OSG
//------------------------------------------------------------------------------
bool Material::setSpecularColor(osg::Vec4 x)
{
    specularColor = x;
    return true;
}
bool Material::setSpecularColor(LCreal x, LCreal y, LCreal z, LCreal w)
{
    specularColor.set(x, y, z, w);
    return true;
}
//------------------------------------------------------------------------------
// setShininess() - set our shininess
//------------------------------------------------------------------------------
bool Material::setShininess(LCreal x)
{
    shininess = x;
    return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
basic::Object* Material::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End BasicGL namespace
} // End oe namespace

