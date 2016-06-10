
-- CLIPS
project "ubf-clips"
   files {
      "../../include/openeaagles/ubf/clips/**.h",
      "../../src/ubf/clips/**.cpp",
      "../../src/ubf/clips/**.c"
   }
   buildoptions {"/TP"}
   defines { "WIN_MVC" }
   includedirs { 
      "../../include/openeaagles/ubf/clips/clips",
      "../../include/openeaagles/ubf/clips" 
   }
   targetname "clips"
      
-- LUA
project "ubf-lua"
   files {
      "../../include/openeaagles/ubf/lua/**.h",
      "../../include/openeaagles/ubf/lua/**.hpp",
      "../../src/ubf/lua/**.cpp",
      "../../src/ubf/lua/**.c"
   }
   includedirs { 
      "../../include/openeaagles/ubf/lua",
      "../../include/openeaagles/ubf/lua/lua-5.3.3"
   }
   targetname "lua"
    
-- SOAR
project "ubf-soar"
   files {
      "../../include/openeaagles/ubf/soar/**.h",
      "../../include/openeaagles/ubf/soar/**.hpp",
      "../../src/ubf/soar/**.c",
      "../../src/ubf/soar/**.cpp"
   }
   defines { "STATIC_LINKED" }
   targetname "soar"

--  example UBF behaviors
project "ubf-behaviors"
   files {
      "../../include/openeaagles/ubf/behaviors/*.h",
      "../../src/ubf/behaviors/*.cpp",
   }
   targetname "behaviors"

