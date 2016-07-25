
project (oebase)

set (SRCS
  Boolean.cpp
  Cie.cpp 
  Cmy.cpp
  Color.cpp
  Complex.cpp
  Component.cpp
  Decibel.cpp
  EarthModel.cpp
  EdlParser.cpp
  EdlScanner.cpp
  factory.cpp
  FileReader.cpp
  Float.cpp
  Hls.cpp
  Hsv.cpp
  Hsva.cpp
  Identifier.cpp
  Integer.cpp
  IoAdapter.cpp
  IoData.cpp
  IoDevice.cpp
  IoHandler.cpp
  LatLon.cpp
  List.cpp
  Locus.cpp
  Logger.cpp
  Matrix.cpp
  MonitorMetrics.cpp
  Nav.cpp
  NavDR.cpp
  NetHandler.cpp
  Number.cpp
  Object.cpp
  Operators.cpp
  Pair.cpp
  PairStream.cpp
  Rgb.cpp
  Rgba.cpp
  Rng.cpp
  SlotTable.cpp
  Stack.cpp
  StateMachine.cpp
  Statistic.cpp
  String.cpp
  Terrain.cpp
  Thread.cpp
  ThreadPool.cpp
  Timers.cpp
  Transforms.cpp
  Vectors.cpp
  Yiq.cpp)

set (SRCS
  ${SRCS}
  distributions/Exponential.cpp
  distributions/Lognormal.cpp
  distributions/Pareto.cpp
  distributions/Uniform.cpp)

set (SRCS
  ${SRCS}
  functors/Function.cpp
  functors/Functions.cpp
  functors/Table.cpp
  functors/Tables.cpp)

set (SRCS
  ${SRCS}
  nethandlers/PosixHandler.cpp
  nethandlers/TcpClient.cpp
  nethandlers/TcpHandler.cpp
  nethandlers/TcpServerMultiple.cpp
  nethandlers/TcpServerSingle.cpp
  nethandlers/UdpBroadcastHandler.cpp
  nethandlers/UdpMulticastHandler.cpp
  nethandlers/UdpUnicastHandler.cpp)

set (SRCS
  ${SRCS}
  osg/Math.cpp
  osg/MatrixDecomposition.cpp
  osg/Matrixd.cpp
  osg/Matrixf.cpp
  osg/Quat.cpp)

if (WIN32)
  set (SRCS ${SRCS} platform/Thread_msvc.cpp}
else ()
  set (SRCS ${SRCS} platform/Thread_linux.cpp}
endif ()

set (SRCS
  ${SRCS}
  ubf/Action.cpp
  ubf/Agent.cpp
  ubf/Arbiter.cpp
  ubf/Behavior.cpp
  ubf/State.cpp)

set (SRCS
  ${SRCS}
  units/Angles.cpp
  units/AngularVelocity.cpp
  units/Areas.cpp
  units/Density.cpp
  units/Distances.cpp
  units/Energies.cpp
  units/FlowRate.cpp
  units/Forces.cpp
  units/Frequencies.cpp
  units/LinearVelocity.cpp
  units/Masses.cpp
  units/Powers.cpp
  units/Times.cpp
  units/Volumes.cpp)

set (SRCS
  ${SRCS}
  util/lfi.cpp
  util/math_utils.cpp
  util/osg_utils.cpp
  util/str_utils.cpp
  util/system.cpp}

if (WIN32)
  set (SRCS ${SRCS} util/system_msvc}
else ()
  set (SRCS ${SRCS} util/system_linux.cpp}
endif ()

add_library (${PROJECT_NAME} ${SRCS})
