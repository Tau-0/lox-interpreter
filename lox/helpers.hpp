#pragma once

#include <type_traits>

template <typename T, typename... Args>
concept IsTypeOf = std::disjunction_v<std::is_same<T, Args>...>;
