//------------------------------------------------------------------------------
// Class: Material
//------------------------------------------------------------------------------
#ifndef __Material_H_B9CB087C_FAC1_472f_B5EC_405B3F0567B1__
#define __Material_H_B9CB087C_FAC1_472f_B5EC_405B3F0567B1__

#include "openeaagles/basic/Component.h"
#include <GL/glu.h>

namespace Eaagles {
namespace BasicGL {

//------------------------------------------------------------------------------
// Class: Material
// 
// Description: Object which will hold materialistic information, which can then
// have light applied to it.
//   Ambient color - light coming from material scattered in all directions equally
//   Diffuse color - light coming from the material after light hits it (main color)
//   Emissive color - light emanating from the material
//   Specular color - mirror reflecting color of the material (shiny highlights)
//   How shiny our material is (0 = dull, 128 = really shiny)
//
// Form name: Material
// Slots
//    ambientColor   <Pairstream>  ! List of numbers for the ambient values
//    ambientColor   <Number>      ! Ambient color - light coming from material scattered in all directions equally
//                                 (default: (0,0,0,1))
//    diffuseColor   <Pairstream>  ! List of numbers for the diffuse values
//    diffuseColor   <Number>      ! Diffuse color - light coming from the material after light hits it (main color)
//                                 (default: (1,1,1,1))
//    emissiveColor  <Pairstream>  ! List of numbers for the emissive values
//    emissiveColor  <Number>      ! Emissive color - light emanating from the material
//                                 (default: (0,0,0,1))
//    specularColor  <Pairstream>  ! List of numbers for the specular values
//    specularColor  <Number>      ! Specular color - mirror reflecting color of the material (shiny highlights)
//                                 (default: (0,0,0,1))
//    shininess      <Number>      ! How shiny our material is (0 = dull, 128 = really shiny) (default: 0)
//------------------------------------------------------------------------------
class Material : public Basic::Component {
    DECLARE_SUBCLASS(Material,Basic::Component)

public:
    Material();
    
    // set data
    virtual bool setAmbientColor(osg::Vec4 x);
    virtual bool setAmbientColor(LCreal x, LCreal y, LCreal z, LCreal w = 1.0);
    virtual bool setDiffuseColor(osg::Vec4 x);
    virtual bool setDiffuseColor(LCreal x, LCreal y, LCreal z, LCreal w = 1.0);
    virtual bool setEmissiveColor(osg::Vec4 x);
    virtual bool setEmissiveColor(LCreal x, LCreal y, LCreal z, LCreal w = 1.0);
    virtual bool setSpecularColor(osg::Vec4 x);
    virtual bool setSpecularColor(LCreal x, LCreal y, LCreal z, LCreal w = 1.0);
    virtual bool setShininess(LCreal x);

    // get data
    osg::Vec4& getAmbientColor()        { return ambientColor; }
    osg::Vec4& getDiffuseColor()        { return diffuseColor; }
    osg::Vec4& getEmissiveColor()       { return emissiveColor; }
    osg::Vec4& getSpecularColor()       { return specularColor; }
    LCreal     getShininess()           { return shininess; }
   
protected:
    bool setSlotAmbientColor(const Basic::PairStream* const x);
    bool setSlotAmbientColor(const Basic::Number* const x);
    bool setSlotDiffuseColor(const Basic::PairStream* const x);
    bool setSlotDiffuseColor(const Basic::Number* const x);
    bool setSlotEmissiveColor(const Basic::PairStream* const x);
    bool setSlotEmissiveColor(const Basic::Number* const x);
    bool setSlotSpecularColor(const Basic::PairStream* const x);
    bool setSlotSpecularColor(const Basic::Number* const x);
    bool setSlotShininess(const Basic::Number*const x);
    
private:
    osg::Vec4 ambientColor;     // holds our ambient color
    osg::Vec4 diffuseColor;     // holds our diffuse color 
    osg::Vec4 emissiveColor;    // holds our emissive color
    osg::Vec4 specularColor;    // holds our specular color
    LCreal shininess;           // hold our shininess value
};

} // End BasicGL namespace
} // End Eaagles namespace

#endif	/* __Material_H_B9CB087C_FAC1_472f_B5EC_405B3F0567B1__ */
