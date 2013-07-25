# Top level Qt OpenEaagles profile, lists all the OpenEaagles
# projects we wish to build.
TEMPLATE      = subdirs
SUBDIRS       = basic       \
                basicGL     \
                dafif       \
                dis         \
                gui         \   # This is a subdirectory
                instruments \
                ioDevice    \
                linearSys   \
                maps        \
                otw         \
                recorder    \
                sensors     \
                simulation  \
                terrain     \
                vehicles
