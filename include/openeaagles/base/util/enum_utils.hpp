
#ifndef __oe_base_util_enum_utils_H__
#define __oe_base_util_enum_utils_H__

// enum utilities

namespace oe {
namespace base {

//
// template converts enum class to underlying integer value representation
//
template <typename Enumeration>
auto as_integer(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

}
}

#endif
