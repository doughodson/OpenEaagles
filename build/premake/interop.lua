
--
-- Interoperability libraries
--

   -- Common interoperability code
   project "interop"
      files {
         "../../include/openeaagles/networks/interop/**.h*",
         "../../src/networks/interop/**.cpp"
      }
      targetname "interop"

   -- IEEE DIS interface library
   project "dis"
      files {
         "../../include/openeaagles/networks/dis/**.h*",
         "../../src/networks/dis/**.cpp"
      }
      targetname "dis"

   -- IEEE HLA interface library (common code)
   project "hla"
      files {
         "../../include/openeaagles/networks/hla/**.h*",
         "../../src/networks/hla/**.cpp"
      }
      includedirs { HLAIncPath }
      defines { "RTI_USES_STD_FSTREAM" }
      targetname "hla"

   -- IEEE HLA interface library for RPR FOM
   project "rprfom"
      files {
         "../../include/openeaagles/networks/rprfom/**.h*",
         "../../src/networks/rprfom/**.cpp"
      }
      includedirs { HLAIncPath }
      defines { "RTI_USES_STD_FSTREAM" }
      targetname "rprfom"


