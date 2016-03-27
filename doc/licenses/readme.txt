
OpenEaagles uses both bison (to generate parser) and math library from OpenSceneGraph.

----------------------------------- 
Based on OpenSceneGraph-3.0.1

OpenEaagles is using just the osg math library (math, vec2, vec3, vec4, matrix and quat)

To keep this version from interfering with the full OSG packages, we made these changes ...
   1) wrapped in 'oe' namespace
   2) changed #include paths from <osg/XXX> to "./XXX"
   3) removed several osg specific include files (eg, <osg/Config>)
   4) added 'value_type' casts to remove 'double' to 'float' warnings
   5) added several new functions (eg. transpose())
