#ifndef DBCTMP_H
#define DBCTMP_H

#include <cstdlib>
#include <utility>

template <typename ...>
struct tuple_t;
template <size_t ... S>
struct tuple_i;

template <typename,typename>
struct pair_t;
template <typename,size_t>
struct pair_tv;
template <typename,size_t,size_t>
struct triple_tvv;

template <size_t>
struct integral_type;

template <typename ... T>
struct identity;

template <typename T1, typename T2, bool B>
struct typlex
{
    typedef T1 type;
};

template <typename T1, typename T2>
struct typlex<T1,T2,false>
{
    typedef T2 type;
};

template <template <typename> class P, typename T>
struct accumulate;
template <template <typename> class P>
struct accumulate<P,tuple_t<>>
{
    enum { value = 0 };
};
template <template <typename> class P, typename T, typename ... TS>
struct accumulate<P,tuple_t<T, TS...>>
{
    enum { value = P<T>::value + accumulate<P, tuple_t<TS...>>::value };
};

template <template <typename> class P, typename S, typename T>
struct accumulate_types_impl;
template <template <typename> class P, typename ... S>
struct accumulate_types_impl<P,tuple_t<S...>,tuple_t<>>
{
    typedef tuple_t<S...> type;
};
template <template <typename> class P, typename ... S, typename T, typename ... TS>
struct accumulate_types_impl<P,tuple_t<S...>,tuple_t<T,TS...>>
{
    typedef typename accumulate_types_impl<P, tuple_t<S..., typename P<T>::type>,tuple_t<TS...>>::type type;
};

template <template <typename> class P, typename T>
struct accumulate_types
{
    typedef typename accumulate_types_impl<P,tuple_t<>,T>::type type;
};

template <template <typename,typename> class P, typename C, typename T>
struct find_first_match;
template <template <typename,typename> class P, typename C>
struct find_first_match<P,C,tuple_t<>>
{
    typedef int type;
    enum { found = 0 };
    enum { index = 0 };
};
template <template <typename,typename> class P, typename C, typename T, typename ... TS>
struct find_first_match<P,C,tuple_t<T,TS...>>
{
    typedef typename typlex<
                        T,
                        typename find_first_match<P,C,tuple_t<TS...>>::type,
                        (P<C,T>::value == 1)
                    >::type
    type;
    enum { found = (P<C,T>::value == 1) || find_first_match<P,C,tuple_t<TS...>>::found };
    enum { index = (P<C,T>::value == 1) ? 0 : 1 + find_first_match<P,C,tuple_t<TS...>>::index };
};

template <size_t,typename>
struct return_Nth;
template <size_t N,typename ... TS>
struct return_Nth<N,tuple_t<TS...>>
{
    static_assert(sizeof...(TS) != 0, "Index out of range!\n");
};
template <typename T, typename ... TS>
struct return_Nth<0,tuple_t<T,TS...>>
{
    typedef T type;
};
template <size_t N,typename T,typename ... TS>
struct return_Nth<N,tuple_t<T, TS...>>
{
    typedef typename return_Nth<N-1,tuple_t<TS...>>::type type;
};

template <size_t,typename,typename...>
struct return_N_first_impl;
template <typename ... S,typename T, typename ... TS>
struct return_N_first_impl<0,tuple_t<S...>,T,TS...>
{
    typedef tuple_t<S...> type;
};
template <typename ... S, typename ... TS>
struct return_N_first_impl<0,tuple_t<S...>,TS...>
{
    typedef tuple_t<S...> type;
};
template <size_t N, typename ... S, typename T, typename ... TS>
struct return_N_first_impl<N,tuple_t<S...>,T,TS...>
{
    typedef typename return_N_first_impl<N-1,tuple_t<S...,T>,TS...>::type type;
};

template <size_t N,typename...TS>
struct return_N_first
{
    typedef typename return_N_first_impl<N,tuple_t<>,TS...>::type type;
};

template <typename T1,typename T2>
struct types_equal
{
    enum { value = false };
};

template <typename T1>
struct types_equal<T1,T1>
{
    enum { value = true };
};

template <typename T>
struct firsttype
{
    template <typename V>
    struct equals_secondtype
    {
        enum { value = types_equal<T,V>::value };
    };
};

template <typename S,typename T>
struct make_set_impl;
template <typename...S>
struct make_set_impl<tuple_t<S...>,tuple_t<>>
{
    typedef tuple_t<S...> type;
};
template <typename...S,typename T,typename ... TS>
struct make_set_impl<tuple_t<S...>, tuple_t<T,TS...>>
{
    typedef typename typlex<    typename make_set_impl<tuple_t<S...,T>,tuple_t<TS...>>::type,
                                typename make_set_impl<tuple_t<S...>,tuple_t<TS...>>::type,
                                accumulate<firsttype<T>::template equals_secondtype, tuple_t<S...>>::value == 0
                     >::type
    type;
};

template <typename T>
struct make_set
{
    typedef typename make_set_impl<tuple_t<>,T>::type type;
};

template <typename T>
struct count_tuple;
template <typename ... TS>
struct count_tuple<tuple_t<TS...>>
{
    enum { value = sizeof...(TS) };
};

template <size_t ... S>
struct seq;

template <size_t ... SEQ>
struct seq<0,SEQ...>
{
    typedef tuple_i<SEQ...> type;
};

template <size_t S, size_t ... SEQ>
struct seq<S, SEQ...> : seq<S-1, S-1, SEQ...>
{

};

template <typename T, size_t I>
struct leaf_node : public T
{
    leaf_node(T && t) :
        T(std::forward<T>(t))
    {
    }
    leaf_node() :
        T()
    {
    }
};

template <typename T, typename S>
struct inherit_all_impl;

template <typename ... T, size_t ... SEQ>
struct inherit_all_impl<tuple_t<T...>,tuple_i<SEQ...>> : private leaf_node<T,SEQ> ...
{
    template <size_t I>
    leaf_node<typename return_Nth<I, tuple_t<T...>>::type, I> * get()
    {
        return static_cast<leaf_node<typename return_Nth<I, tuple_t<T...>>::type, I>*>(this);
    }
    inherit_all_impl() :
        leaf_node<T,SEQ>()...
    {
    }
    inherit_all_impl(T&& ... t) :
        leaf_node<T,SEQ>(std::forward<T>(t))...
    {
    }
};

template <typename T>
struct inherit_all;
template <typename ... TS>
struct inherit_all<tuple_t<TS...>> : public inherit_all_impl<tuple_t<TS...>,typename seq<sizeof...(TS)>::type>
{
    template <size_t I>
    leaf_node<typename return_Nth<I, tuple_t<TS...>>::type, I> * get()
    {
        return static_cast<inherit_all_impl<tuple_t<TS...>,typename seq<sizeof...(TS)>::type>*>(this)->get<I>();
    }
    inherit_all() :
        inherit_all_impl<tuple_t<TS...>,typename seq<sizeof...(TS)>::type>()
    {
    }

    inherit_all(TS&&...ts) :
        inherit_all_impl<tuple_t<TS...>,typename seq<sizeof...(TS)>::type>(std::forward<TS>(ts)...)
    {
    }
};

#endif // DBCTMP_H
