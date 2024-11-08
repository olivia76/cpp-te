/**
 *  \author Olivia Quinet
 */

#include <te2/pimpl.hpp>
#include <te2/typeinfo_pimpl.hpp>

namespace te2::pimpl {
unique_ptr_strategy::value_concept::~value_concept() {}
} // namespace te2::pimpl
namespace te2::type_info_pimpl {
unique_ptr_strategy::value_concept::~value_concept() {}
} // namespace te2::type_info_pimpl
