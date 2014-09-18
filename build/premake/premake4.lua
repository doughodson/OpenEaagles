
--
-- If premake command is not supplied an action (target compiler), exit!
--
-- Targets of interest:
--     vs2008     (Visual Studio 2008)
--     vs2010     (Visual Studio 2010)
--     vs2012     (Visual Studio 2012)
--     vs2013     (Visual Studio 2013)
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
-- determine target directories for project/solution files and 
-- compiled libraries
--
locationPath  = "../" .. _ACTION
if (_ACTION == "vs2008") or (_ACTION == "vs2010") or (_ACTION == "vs2012") or (_ACTION == "vs2013") then
  targetDirPath = "../../lib/".._ACTION
end
if (_ACTION == "codelite") or (_ACTION == "codeblocks") then
  targetDirPath = "../../lib/mingw"
end
if (os.is("linux")) then
  targetDirPath = "../../lib/linux"
end
print ("Target directory path: "..targetDirPath)

-- for now, premake does not support this action, so use 2012 instead
if (_ACTION == "vs2013") then
   _ACTION = "vs2012"
end

solution "oe"

   -- destination directory for generated solution/project files
   location (locationPath)

   -- destination directory for compiled binary target
   targetdir (targetDirPath)

   -- creating static libraries
   kind "StaticLib"

   -- C++ code in all projects
   language "C++"

   -- common include directories (all configurations/all projects)
   includedirs { OEIncPath, OE3rdPartyIncPath }

   -- target suffix (all configurations/all projects)
   targetprefix "oe"
   if (_ACTION == "codelite") or (_ACTION == "codeblocks") then
      targetprefix "liboe"
   end

   --
   -- Build (solution) configuration options:
   --     Release        (Runtime library is Multi-threaded DLL)
   --     Debug          (Runtime library is Multi-threaded Debug DLL)
   --
   configurations { "Release", "Debug" }

   -- common release configuration flags and symbols
   configuration { "Release" }
      flags { "Optimize" }
      if (_ACTION == "vs2008") or (_ACTION == "vs2010") or (_ACTION == "vs2012") then
         -- enable compiler intrinsics and favour speed over size
         buildoptions { "/Oi", "/Ot" }
         defines { "WIN32", "_LIB", "NDEBUG" }
      end
      if (os.is("linux")) then
         buildoptions { "-O3 -Wall -Wno-long-long -pthread" }
      end

   -- common debug configuration flags and symbols
   configuration { "Debug" }
      targetsuffix "_d"
      flags { "Symbols" }
      if (_ACTION == "vs2008") or (_ACTION == "vs2010") or (_ACTION == "vs2012") then
         -- enable compiler intrinsics
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
         "../../src/basic/osg/Matrix_implementation.cpp",
         "../../src/basic/linux/**.*"
      }
      targetname "Basic"

   -- basic OpenGL library
   project "basicGL"
      files {
         "../../include/openeaagles/basicGL/**.h",
         "../../src/basicGL/**.cpp"
      }
      includedirs { OE3rdPartyIncPath.."/freetype2" }
      defines { "FTGL_LIBRARY_STATIC" }
      targetname "BasicGL"

   -- GLUT OpenGL interface library
   project "gui-glut"
      files {
         "../../include/openeaagles/gui/glut/**.h",
         "../../src/gui/glut/**.cpp"
      }
      targetname "Glut"

   -- DAFIF airport loader library
   project "dafif"
      files {
         "../../include/openeaagles/dafif/**.h",
         "../../src/dafif/**.cpp"
      }
      targetname "Dafif"

   -- IEEE DIS interface library
   project "dis"
      files {
         "../../include/openeaagles/dis/**.h",
         "../../src/dis/**.cpp"
      }
      targetname "Dis"

   -- dynamics library
   project "dynamics"
      files {
         "../../include/openeaagles/dynamics/**.h",
         "../../src/dynamics/**.cpp"
      }
      includedirs { OE3rdPartyIncPath.."/JSBSim" }
      targetname "Dynamics"

   -- graphical instruments library
   project "instruments"
      files {
         "../../include/openeaagles/instruments/**.h",
         "../../include/openeaagles/instruments/**.epp",
         "../../src/instruments/**.cpp"
      }
      targetname "Instruments"

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
      targetname "IoDevice"

   -- linear systems library
   project "linearSys"
      files {
         "../../include/openeaagles/linearSys/**.h",
         "../../src/linearSys/**.cpp"
      }
      targetname "LinearSys"

   -- maps library
   project "maps"
      files {
         "../../include/openeaagles/maps/**.h",
         "../../src/maps/**.cpp"
      }
      targetname "Maps"

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
      targetname "Otw"

   project "recorder"
      files {
         "../../include/openeaagles/recorder/**.h",
         "../../include/openeaagles/recorder/*.inl",
         "../../include/openeaagles/recorder/**.proto",
         "../../src/recorder/**.cpp",
         "../../src/recorder/**.cc"
      }
      defines { "_SCL_SECURE_NO_WARNINGS" } -- suppress protocol buffer warning
      targetname "Recorder"

   -- sensors library
   project "sensors"
      files {
         "../../include/openeaagles/sensors/**.h",
         "../../src/sensors/**.cpp"
      }
      targetname "Sensors"

   -- simulation library
   project "simulation"
      files {
         "../../include/openeaagles/simulation/**.h",
         "../../include/openeaagles/simulation/*.inl",
         "../../src/simulation/**.cpp"
      }
      targetname "Simulation"

   -- terrain library
   project "terrain"
      files {
         "../../include/openeaagles/terrain/**.h",
         "../../src/terrain/**.cpp"
      }
      targetname "Terrain"


