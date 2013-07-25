# Top level Qt OpenEaagles profile, lists all the OpenEaagles
# projects we wish to build.
TEMPLATE      = subdirs
SUBDIRS       = basic       \
                basicGL     \
                basicGL2.0  \
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
