#pragma once

#include <concepts>
#include <tuple>

namespace typelist
{
	template <typename ... Ts>
	struct typelist{};
}

namespace typelist::details
{
	template <typename Head, typename ... Ts>
	struct last
	{
		using type = typename last<Ts...>::type;
	};

	template <typename T>
	struct last<T>
	{
		using type = T;
	};

	template <typename Head, typename ... Tail>
	struct first
	{
		using type = Head; 
	};

	template <typename Head, typename ... Tail>
	struct get_tail
	{
		using type = typelist<Tail...>;
	};

	template <typename ... Ts>
	static constexpr typelist<Ts...> decay_typelist(typelist<typelist<Ts...>>)
	{
		return {};
	}

	template <typename ... FirstList, typename ... SecondList>
	constexpr static auto combine_helper(typelist<FirstList...>, typelist<SecondList...>) noexcept
	{
		return typelist<FirstList..., SecondList...>{};
	}

	template <std::size_t Size, typename First, typename ... TypeLists>
	struct combine
	{
		using type = typename combine<sizeof ... (TypeLists), decltype(combine_helper(First{}, typename first<TypeLists...>::type{})), decltype(decay_typelist(typename get_tail<TypeLists...>::type{}))>::type;
	};

	template <typename First, typename ... TypeLists>
	struct combine<2, First, TypeLists...>
	{
		using type = decltype(details::combine_helper(First{}, TypeLists{}...));
	};

	template <std::size_t Idx, typename Head, typename ... Tail>
	struct get_head_impl
	{
		using helper = typelist<Head>;
	};

	template <std::size_t Idx, typename Head, typename ... Tail>
	struct get_nth
	{
		using type = typename get_nth<Idx - 1, Tail...>::type;
	};

	template <typename Head, typename ... Tail>
	struct get_nth<0, Head, Tail...>
	{
		using type = Head;
	};

	template <typename Head, typename Next, typename ... Ts>
	struct get_head
	{

	};
}

namespace typelist
{
	template <typename ... Ts>
	struct typelist;

	template <typename Head, typename ... Ts>
	using first_t = typename details::first<Head, Ts...>::type;

	template <typename Head, typename ... Ts>
	using last_t = typename details::last<Head, Ts...>::type;

	template <typename Head, typename ... Ts>
	using get_tail_t = typename details::get_tail<Head, Ts...>::type;

	template <std::size_t Idx, typename Head, typename ... Ts>
	using get_nth_t = typename details::get_nth<Idx, Head, Ts...>::type;

	template <typename  First, typename Second, typename ... TypeLists>
	using combine_t = typename details::combine<sizeof ... (TypeLists) + 2, First, Second, TypeLists...>::type;

}
