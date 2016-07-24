#ifndef TMP_H
#define TMP_H

#include <cstddef>
#include <type_traits>
#include "tmp_predicates.h"

namespace tmp
{

struct no_type;

template <typename T>
using identity = T;

template <size_t N>
struct int_type
{
    enum { value = N };
};

template <size_t N1, size_t N2>
struct int_pair_type
{
    enum { v1 = N1 };
    enum { v2 = N2 };
};

template <typename,typename>
struct pair_t;

template <size_t,size_t>
struct pair_i;

template <typename ...>
struct tuple_t;

template <size_t ...>
struct tuple_i;

template <typename,typename>
struct types_equal
{
    enum { value = false };
};

template <typename T>
struct types_equal<T,T>
{
    enum { value = true };
};

namespace detail
{

    template <size_t N, typename ... TS>
    struct get_element_at_impl
    {
        static_assert(sizeof...(TS) >= N, "Element at index does not exist. Index too large.\n");
    };

    template <typename T, typename ... TS>
    struct get_element_at_impl<0, T, TS...>
    {
        typedef T type;
    };

    template <size_t N, typename T, typename ... TS>
    struct get_element_at_impl<N, T, TS...>
    {
        typedef typename get_element_at_impl<N-1,TS...>::type type;
    };

} // detail

template <size_t N, typename ... TS>
using get_element_at = typename detail::get_element_at_impl<N,TS...>::type;
template <size_t N, size_t ... VS>
using get_element_at_i = typename detail::get_element_at_impl<N,int_type<VS>...>::type;

namespace detail
{
    // the interface of this has arguments in an order which makes sense
    template <template <typename> class PREDICATE>
    struct get_element_of_impl
    {
        template <typename T>
        struct that_satisfies
        {
            typedef no_type type;
        };
        template <template <typename...> class T, typename E, typename ... ES>
        struct that_satisfies<T<E,ES...>>
        {
            typedef get_element_at<PREDICATE<E>::value != 0,
                                            typename that_satisfies<tuple_t<ES...>>::type,
                                            E>
                        type;
        };

    };

} // detail

template <typename T>
struct get_element_of
{
    template <template <typename> class PREDICATE>
    using that_satisfies = typename detail::get_element_of_impl<PREDICATE>::template that_satisfies<T>::type;
};

template <template <typename,typename> class OPERATOR, template <typename> class PREDICATE, typename INITVAL>
struct fold
{
    template <typename TUPLE>
    struct on
    {
        enum { value = INITVAL::value };
    };

    template <template <typename...> class TUPLE, typename E, typename ... ES>
    struct on<TUPLE<E,ES...>>
    {
        enum { value = OPERATOR<PREDICATE<E>,on<tuple_t<ES...>>>::value };
    };
};

namespace detail
{
    template <typename A, typename B>
    struct addition
    {
        enum { value = A::value + B::value };
    };
    template <typename A, typename B>
    struct multiplication
    {
        enum { value = A::value * B::value };
    };
}


template <template <typename> class PREDICATE>
struct sum
{
    template <typename TUPLE>
    using of = typename fold<detail::addition,PREDICATE,int_type<0>>::template on<TUPLE>;
};

template <template <typename> class PREDICATE>
struct product
{
    template <typename TUPLE>
    using of = typename fold<detail::multiplication,PREDICATE,int_type<1>>::template on<TUPLE>;
};


namespace detail
{

    template <typename T,typename S>
    struct set_of_impl;

    template <typename ... S>
    struct set_of_impl<tuple_t<>,tuple_t<S...>>
    {
        typedef tuple_t<S...> type;
    };

    template <typename E, typename ... ES, typename ... S>
    struct set_of_impl<tuple_t<E,ES...>, tuple_t<S...>>
    {
        typedef get_element_at<    sum<compare<E>::template with>::template of<tuple_t<S...>>::value != 0,
                                                typename set_of_impl<tuple_t<ES...>,tuple_t<S...,E>>::type,
                                                typename set_of_impl<tuple_t<ES...>,tuple_t<S...>>::type
                                > type;

    };

} // namespace detail

/*  Compare all elements against each other to determine if duplicates exists.
    This requires (N^2/)2 comparisons. */

template <typename T>
using set_of = typename detail::set_of_impl<T,tuple_t<>>::type;

template <typename>
struct append
{
    template <typename>
    struct with;
};

template <template <typename...> class T, typename ... ES>
struct append<T<ES...>>
{
    template <typename B>
    using with = tuple_t<ES...,B>;
};

template <typename>
struct prepend
{
    template <typename>
    struct with;
};

template <template <typename...> class T, typename ... ES>
struct prepend<T<ES...>>
{
    template <typename A>
    using with = tuple_t<A,ES...>;
};

namespace detail
{
    template <typename MERGED, typename ... TS>
    struct merge_tuples_impl;
    template <typename ... MS>
    struct merge_tuples_impl<tuple_t<MS...>>
    {
        typedef tuple_t<MS...> type;
    };
    template <typename ... MS, typename ... NWS, typename ... TS>
    struct merge_tuples_impl<tuple_t<MS...>,tuple_t<NWS...>,TS...>
    {
        typedef typename merge_tuples_impl<tuple_t<MS...,NWS...>,TS...>::type type;
    };
} // namespace detail

template <typename ... TS>
using merge_tuples = typename detail::merge_tuples_impl<tuple_t<>,TS...>::type;

template <class META, typename ... CS>
struct tree_node : public META
{
};

namespace detail
{
    // tree_impl<CS>::type is a tuple of tuples
    template <typename NODE>
    struct tree_impl
    {
        typedef tuple_t<tuple_t<NODE>> type;
    };
    template <typename C, typename T>
    struct prepend_all_of;
    template <typename C, typename ... ES>
    struct prepend_all_of<C,tuple_t<ES...>>
    {
        typedef tuple_t<typename prepend<ES>::template with<C>...> type;
    };
    template <typename T>
    struct tree_extract_impl;
    template <typename T>
    struct tree_extract_impl<tuple_t<T>>
    {
        typedef T type;
    };
    template <typename META, typename ... CS>
    struct tree_impl<tree_node<META,CS...>>
    {
        // The child of tree_impl will provide a list of all its nodes
        typedef merge_tuples<typename prepend_all_of<META, typename tree_impl<CS>::type>::type...> type;
    };
} // namespace detail

/*  If the given input is 'tree-like', then provide information of every end-node as a tuple_t
    of the path down to it. */

template <typename T>
using tree = typename detail::tree_impl<T>::type;

namespace detail
{
    template <template <typename> class F, typename T, typename N>
    struct map_impl;
    template <template <typename> class F, template <typename...> class T, typename ... NS>
    struct map_impl<F,T<>,tuple_t<NS...>>
    {
        typedef tuple_t<NS...> type;
    };
    template <template <typename> class F, template <typename...> class T, typename E, typename ... ES, typename ... NS>
    struct map_impl<F,T<E,ES...>,tuple_t<NS...>>
    {
        typedef typename map_impl<F,T<ES...>,tuple_t<NS...,typename F<E>::type>>::type type;
    };
} // namespace detail

template <template <typename> class F, typename T>
using map = typename detail::map_impl<F,T,tuple_t<>>::type;

namespace detail
{
    template <template <typename> class F, typename T, typename N>
    struct filter_impl;
    template <template <typename> class F, template <typename...> class T, typename ... NS>
    struct filter_impl<F,T<>,tuple_t<NS...>>
    {
        typedef tuple_t<NS...> type;
    };
    template <template <typename> class F, template <typename...> class T, typename E, typename ... ES, typename ... NS>
    struct filter_impl<F,T<E,ES...>,tuple_t<NS...>>
    {
        typedef typename get_element_at<F<E>::value,
                                        typename filter_impl<F,T<ES...>,tuple_t<NS...>>::type,
                                        typename filter_impl<F,T<ES...>,tuple_t<NS...,E>>::type>
            type;
    };
} // namespace detail

template <template <typename> class F, typename T>
using filter = typename detail::filter_impl<F,T,tuple_t<>>::type;

namespace detail
{

}


namespace detail
{
    template <typename T, bool IS_CLASS>
    struct data_wrapper : public T
    { };
    template <typename T>
    struct data_wrapper<T, false>
    {
        T data;
    };
    template <size_t I, typename T>
    struct leaf_node : public data_wrapper<T,std::is_class<T>::value>
    {
    };

    template <size_t I, typename T, typename IS_CLASS>
    T& get_leaf_data(leaf_node<I,T> & l, IS_CLASS c)
    {
        return static_cast<T&>(l);
    }

    template <size_t I, typename T>
    T& get_leaf_data(leaf_node<I,T> & l, int_type<0> c)
    {
        return static_cast<data_wrapper<T,false>&>(l).data;
    }

    template <size_t ... SEQ>
    struct sequence_impl;

    template <size_t N, size_t ... NS>
    struct sequence_impl<N, NS...> : public sequence_impl<N-1,N-1,NS...>
    {
    };

    template <size_t ... SEQ>
    struct sequence_impl<1,SEQ...>
    {
        typedef tuple_i<0,SEQ...> type;
    };

} // namespace detail

template <size_t ... SEQ>
using sequence = typename detail::sequence_impl<SEQ...>::type;

template <typename T>
struct fail
{
    typedef typename T::alalallalalalalla f;
};

} // namespace tmp








#endif // TMP_H
