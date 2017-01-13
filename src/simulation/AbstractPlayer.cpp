
#include "openeaagles/simulation/AbstractPlayer.hpp"

namespace oe {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(AbstractPlayer, "AbstractPlayer")

EMPTY_SLOTTABLE(AbstractPlayer)
EMPTY_COPYDATA(AbstractPlayer)
EMPTY_DELETEDATA(AbstractPlayer)
EMPTY_SERIALIZER(AbstractPlayer)

AbstractPlayer::AbstractPlayer()
{
   STANDARD_CONSTRUCTOR()
}

}
}
