#pragma once

#include "fixed.hpp"

#define ERROR_TYPES_MACRO \
static_assert(false,"You should specify available types through compile flag -DTYPES");
#define TYPES FIXED(10,7), FAST_FIXED(13,1), FIXED(13,7)
#define FIXED(N,K) Fixed<N,K>
#define FAST_FIXED(N,K) Fast_Fixed<N,K>
#define DOUBLE Double
#define FLOAT Float
#ifndef TYPES
ERROR_TYPES_MACRO
#endif
#ifdef SIZES
// Deal with size choosing
#endif
template<typename... ts>
struct Type_List {
};

template<typename T>
struct TypeListSize;

template<typename... Ts>
struct TypeListSize<Type_List<Ts...> > {
    static constexpr size_t value = sizeof...(Ts);
};

typedef Type_List<TYPES > AllTypes;


template<typename F, typename S, typename T>
struct triplet {
};


template<typename L1, typename L2>
struct Type_List_Concat;


template<typename... Ts1, typename... Ts2>
struct Type_List_Concat<Type_List<Ts1...>, Type_List<Ts2...> > {
    using type = Type_List<Ts1..., Ts2...>;
};


template<typename... F, typename... S, typename... T>
struct triplet<Type_List<F...>, Type_List<S...>, Type_List<T...> > {
    using type = Type_List<std::tuple<F, S, T>...>;
};


template<typename F, typename S, typename T>
struct cartesian_product_impl;

template<typename... Fs, typename... Ss, typename... Ts>
struct cartesian_product_impl<Type_List<Fs...>, Type_List<Ss...>, Type_List<Ts...> > {
private:
    // Создает комбинации для одного F с одним S и всеми T
    template<typename CurrentF, typename CurrentS>
    struct make_combinations_with_t {
        using type = Type_List<std::tuple<CurrentF, CurrentS, Ts>...>;
    };

    // Создает комбинации для одного F со всеми S
    template<typename CurrentF>
    struct make_combinations_with_s {
        template<typename... CurrentSs>
        struct helper {
            using type = Type_List<>;
        };

        template<typename CurrentS, typename... RestS>
        struct helper<CurrentS, RestS...> {
            using type = typename Type_List_Concat<
                typename make_combinations_with_t<CurrentF, CurrentS>::type,
                typename helper<RestS...>::type
            >::type;
        };

        using type = typename helper<Ss...>::type;
    };

    // Объединяет все комбинации для всех F
    template<typename... Args>
    struct combine_all {
        using type = Type_List<>;
    };

    template<typename First, typename... Rest>
    struct combine_all<First, Rest...> {
        using type = typename Type_List_Concat<
            typename make_combinations_with_s<First>::type,
            typename combine_all<Rest...>::type
        >::type;
    };

public:
    using type = typename combine_all<Fs...>::type;
};


template<typename tuple>
void print_types() {
    using p_type = std::tuple_element_t<0, tuple>;
    using v_type = std::tuple_element_t<1, tuple>;
    using v_flow_type = std::tuple_element_t<2, tuple>;
    std::cout << "Type: " << typeid(p_type).name() << " " << typeid(v_type).name() << " " << typeid(v_flow_type).
            name()
            << " " << "\n";
}

template<typename... ts>
void call_print_types() {
    (print_types<ts>(), ...);
}

template<typename... ts>
void Process_Types(Type_List<ts...>) {
    (call_print_types<ts>(), ...);
}

using types = typename cartesian_product_impl<AllTypes, AllTypes, AllTypes>::type;
