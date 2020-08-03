#include <type_traits>
#include <vector>

/*
    This file demonstrates how you can perform parallel iteration of two types.
*/


namespace c9 {

/*  This is an example of how you can change the access type for a given type. 
    Because are example  uses std::vector, and std::vector has a specialisation
    for `bool` that returns a special iterator that can only be dereferenced by
    value, then we need to make sure that, in that case, a value type is 
    returned rather than a reference. */
template <typename Iter>
using select_access_type_for = std::conditional_t<
    std::is_same_v<Iter, std::vector<bool>::iterator> ||
    std::is_same_v<Iter, std::vector<bool>::const_iterator>,
    typename Iter::value_type,
    typename Iter::reference
>;


/*  This is an iterator-like object. It's not really a proper iterator, but
    it does satisfy the requirements needed for range-for. */
template <typename Iter1, typename Iter2>
class zip_iterator
{
public:

    using value_type = std::pair<
        select_access_type_for<Iter1>,
        select_access_type_for<Iter2>
    >;

    zip_iterator() = delete;

    zip_iterator(Iter1 iter_1_begin, Iter2 iter_2_begin)
        : m_iter_1_begin {iter_1_begin}
        , m_iter_2_begin {iter_2_begin}
    {
    }

    auto operator++() -> zip_iterator& 
    {
        ++m_iter_1_begin;
        ++m_iter_2_begin;
        return *this;
    }

    auto operator++(int) -> zip_iterator 
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    auto operator!=(zip_iterator const & other)
    {
        return !(*this == other);
    }


    /*  The comparison we need to make in order to detect when iteration should
        end is when any of the iterators equal their end position. From a 
        semantics point of view, equality in this sense is not really accurate;
        you would expect, ordinarily, that the test would be the aggregate
        equivalence of it's members. However, if the things being iterated over
        have different sizes, both iterators will never reach the end at the 
        same time. Therefore we need to return true as soon as any of the 
        iterators are at their end position, terminating the iteration loop.
    */
    auto operator==(zip_iterator const & other)
    {
        return 
            m_iter_1_begin == other.m_iter_1_begin ||
            m_iter_2_begin == other.m_iter_2_begin;
    }

    auto operator*() -> value_type
    {
        return value_type{*m_iter_1_begin, *m_iter_2_begin};
    }

private:
    Iter1 m_iter_1_begin;
    Iter2 m_iter_2_begin;
};


/*  We need to select the correct iterator for the passed in types. If one of
    the types is a `std::vector<int> const &` then we need to make sure we use
    it's `const_iterator, rather than the normal iterator.
    std::decay is needed because T is a reference, and is not a complete type.
    Using decay will give us the fundamental type and allows to access it's type
    definitions */
template <typename T>
using select_iterator_for = std::conditional_t<
    std::is_const_v<std::remove_reference_t<T>>, 
    typename std::decay_t<T>::const_iterator,
    typename std::decay_t<T>::iterator>;


/*  Class that is called upon to do the zipping. It contains the necessary
    functions to satisfy range-for, by providing both begin and end functions.
*/
template <typename T, typename U>
class zipper
{
public:

    using Iter1 = select_iterator_for<T>;
    using Iter2 = select_iterator_for<U>;

    using zip_type = zip_iterator<Iter1, Iter2>;

    template <typename V, typename W>
    zipper(V && a, W && b)
        : m_a{a}
        , m_b{b}
    {
    }

    auto begin() -> zip_type
    {
        return zip_type{std::begin(m_a), std::begin(m_b)};
    }
    auto end() -> zip_type
    {
        return zip_type{std::end(m_a), std::end(m_b)};
    }

private:
    T m_a;
    U m_b;
};


/*  Utility method that used at the user's call site. The reason for this is
    that it allows the types of what's passed in to be deduced. Otherwise if
    you were to use the class directly, you would need to provide template
    arguments. 
    We also need to take a universal reference -- not because we want to take
    both lvalues and rvalues (it doesn't make sense to take an rvalue) but
    because we don't know whether the type is const or non-const. */
template <typename T, typename U>
auto zip(T && t, U && u)
{
    return zipper<T, U>{std::forward<T>(t), std::forward<U>(u)};
}

} // namespace c9