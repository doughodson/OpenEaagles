
--
-- If premake command is not supplied an action (target compiler), exit!
--
-- Targets of interest:
--     vs2008     (Visual Studio 2008)
--     vs2010     (Visual Studio 2010)
--     codeblocks (Code::Blocks)
--     codelite   (CodeLite)
--
if (_ACTION == nil) then
    return
end

--
-- directory location for 3rd party dependencies
--
OE_3RD_PARTY_ROOT = "../../../OpenEaagles3rdParty"
if (os.is("linux")) then
   OE_3RD_PARTY_ROOT = "/usr/local"
end
--
-- set include and library paths
--
OEIncPath         = "../../include"
OE3rdPartyIncPath = OE_3RD_PARTY_ROOT.."/include"

--
-- determine target directory for compiled libraries
--
if (_ACTION == "vs2008") or (_ACTION == "vs2010") then
  targetDirPath = "../../lib/".._ACTION
end
if (_ACTION == "codelite") or (_ACTION == "codeblocks") then
  targetDirPath = "../../lib/mingw"
end
if (os.is("linux")) then
  targetDirPath = "../../lib/linux"
end
print ("Target directory path: "..targetDirPath)

solution "oe"

   -- destination directory for generated solution/project files
   location ("../" .. _ACTION)

   -- destination directory for compiled binary target
   targetdir (targetDirPath)

   -- creating static libraries
   kind "StaticLib"

   -- C++ code in all projects
   language "C++"

   -- common include directories (all configurations/all projects)
   includedirs { OEIncPath, OE3rdPartyIncPath }

   --
   -- Build (solution) configuration options:
   --     Release        (Runtime library is Multi-threaded DLL)
   --     Debug          (Runtime library is Multi-threaded Debug DLL)
   --
   configurations { "Release", "Debug" }

   -- common release configuration flags and symbols
   configuration { "Release" }
      flags { "Optimize" }
      if (_ACTION == "vs2008") or (_ACTION == "vs2010") then
         -- enable compilier intrinsics and favor speed over size
         buildoptions { "/Oi", "/Ot" }
         defines { "WIN32", "_LIB", "NDEBUG" }
      end
      if (os.is("linux")) then
         buildoptions { "-O3 -Wall -Wno-long-long -pthread" }
      end

   -- common debug configuration flags and symbols
   configuration { "Debug" }
      flags { "Symbols" }
      if (_ACTION == "vs2008") or (_ACTION == "vs2010") then
         -- enable compilier intrinsics
         buildoptions { "/Oi" }
         defines { "WIN32", "_LIB", "_DEBUG" }
      end
      if (os.is("linux")) then
         buildoptions { "-Wall -Wno-long-long -pthread" }
      end

   --
   -- libraries
   --

   -- basic library
   project "basic"
      files {
         "../../include/openeaagles/basic/**.h",
         "../../include/openeaagles/basic/**.inl",
         "../../include/openeaagles/basic/**.epp",
         "../../include/openeaagles/basic/osg/*",
         "../../src/basic/**.cpp"
      }
      excludes {
         "../../src/basic/osg/Matrix_implementation.cpp"
      }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeBasic"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeBasic_d"

   -- basic OpenGL library
   project "basicGL"
      files {
         "../../include/openeaagles/basicGL/**.h",
         "../../src/basicGL/**.cpp"
      }
      includedirs { OE3rdPartyIncPath.."/freetype2" }
      defines { "FTGL_LIBRARY_STATIC" }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeBasicGL"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeBasicGL_d"

   -- GLUT OpenGL interface library
   project "gui-glut"
      files {
         "../../include/openeaagles/gui/glut/**.h",
         "../../src/gui/glut/**.cpp"
      }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeGlut"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeGlut_d"

   -- DAFIF airport loader library
   project "dafif"
      files {
         "../../include/openeaagles/dafif/**.h",
         "../../src/dafif/**.cpp"
      }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeDafif"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeDafif_d"

   -- IEEE DIS interface library
   project "dis"
      files {
         "../../include/openeaagles/dis/**.h",
         "../../src/dis/**.cpp"
      }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeDis"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeDis_d"

   -- graphical instruments library
   project "instruments"
      files {
         "../../include/openeaagles/instruments/**.h",
         "../../include/openeaagles/instruments/**.epp",
         "../../src/instruments/**.cpp"
      }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeInstruments"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeInstruments_d"

   -- i/o device library
   project "ioDevice"
      files {
         "../../include/openeaagles/ioDevice/**.h",
         "../../src/ioDevice/**.cpp"
      }
      if (os.is("linux")) then
         excludes { "../../src/ioDevice/windows/*" }
      else
         excludes { "../../src/ioDevice/linux/*"   }
      end
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeIoDevice"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeIoDevice_d"

   -- linear systems library
   project "linearSys"
      files {
         "../../include/openeaagles/linearSys/**.h",
         "../../src/linearSys/**.cpp"
      }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeLinearSys"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeLinearSys_d"

   -- maps library
   project "maps"
      files {
         "../../include/openeaagles/maps/**.h",
         "../../src/maps/**.cpp"
      }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeMaps"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeMaps_d"

   -- otw library
   project "otw"
      files {
         "../../include/openeaagles/otw/**.h",
         "../../src/otw/**.h",
         "../../src/otw/**.cpp"
      }
      excludes {
         "../../include/openeaagles/otw/OtwCigiClV2.h",
         "../../src/otw/OtwCigiClV2.cpp"
      }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeOtw"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeOtw_d"

   -- sensors library
   project "sensors"
      files {
         "../../include/openeaagles/sensors/**.h",
         "../../src/sensors/**.cpp"
      }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeSensors"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeSensors_d"

   -- simulation library
   project "simulation"
      files {
         "../../include/openeaagles/simulation/**.h",
         "../../include/openeaagles/simulation/*.inl",
         "../../src/simulation/**.cpp"
      }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeSimulation"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeSimulation_d"

   -- terrain library
   project "terrain"
      files {
         "../../include/openeaagles/terrain/**.h",
         "../../src/terrain/**.cpp"
      }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeTerrain"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeTerrain_d"

   -- ubf library
   project "ubf"
      files {
         "../../include/openeaagles/ubf/**.h",
         "../../src/ubf/**.cpp"
      }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeUbf"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeUbf_d"

   -- vehicles library
   project "vehicles"
      files {
         "../../include/openeaagles/vehicles/**.h",
         "../../src/vehicles/**.cpp"
      }
      includedirs { OE3rdPartyIncPath.."/JSBSim" }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeVehicles"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeVehicles_d"

