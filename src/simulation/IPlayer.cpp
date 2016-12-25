
#include "openeaagles/simulation/IPlayer.hpp"

namespace oe {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(IPlayer, "IPlayer")

EMPTY_SLOTTABLE(IPlayer)
EMPTY_COPYDATA(IPlayer)
EMPTY_DELETEDATA(IPlayer)
EMPTY_SERIALIZER(IPlayer)

IPlayer::IPlayer()
{
   STANDARD_CONSTRUCTOR()
}

}
}
