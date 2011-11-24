
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
   includedirs { "../../include" }

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
         buildoptions { "-O3 -Wno-long-long -pthread" }
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
         buildoptions { "-Wno-long-long -pthread" }
      end

   --
   -- libraries
   --

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

   project "basicGL"
      files {
         "../../include/openeaagles/basicGL/**.h",
         "../../src/basicGL/**.cpp"
      }
      if (os.is("linux")) then
      includedirs {
         "/usr/include/freetype2"
      }
      else
      includedirs {
         "../../../OpenEaagles3rdParty/include",
         "../../../OpenEaagles3rdParty/include/freetype2"
      }
      end
      defines { "FTGL_LIBRARY_STATIC" }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeBasicGL"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeBasicGL_d"

   project "gui-glut"
      files {
         "../../include/openeaagles/gui/glut/**.h",
         "../../src/gui/glut/**.cpp"
      }
      includedirs {
         "../../../OpenEaagles3rdParty/include"
      }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeGlut"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeGlut_d"

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

   project "ioDevice"
      files {
         "../../include/openeaagles/ioDevice/**.h",
         "../../src/ioDevice/**.cpp"
      }
      if (os.is("linux")) then
      excludes {
         "../../src/ioDevice/windows/*",
      }
      else
      excludes {
         "../../src/ioDevice/linux/*"
      }
      includedirs {
         "../../../OpenEaagles3rdParty/include"
      }
      end
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeIoDevice"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeIoDevice_d"

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
      includedirs {
         "../../../OpenEaagles3rdParty/include"
      }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeOtw"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeOtw_d"

   project "sensors"
      files {
         "../../include/openeaagles/sensors/**.h",
         "../../src/sensors/**.cpp"
      }
      excludes {
         "../../src/sensors/Makefile"
      }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeSensors"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeSensors_d"

   project "simulation"
      files {
         "../../include/openeaagles/simulation/**.h",
         "../../include/openeaagles/simulation/*.inl",
         "../../src/simulation/**.cpp"
      }
      excludes {
         "../../src/simulation/Makefile"
      }
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeSimulation"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeSimulation_d"

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

   project "vehicles"
      files {
         "../../include/openeaagles/vehicles/**.h",
         "../../src/vehicles/**.cpp"
      }
      if (os.is("linux")) then
      includedirs {
         "/usr/local/include/JSBSim"
      }
      else
      includedirs {
         "../../../OpenEaagles3rdParty/include",
         "../../../OpenEaagles3rdParty/include/JSBSim"
      }
      end
      configuration { "Release" }
         -- base filename for compiled binary target
         targetname "oeVehicles"
      configuration { "Debug" }
         -- base filename for compiled binary target
         targetname "oeVehicles_d"

