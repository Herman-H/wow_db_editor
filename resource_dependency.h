#ifndef RESOURCE_DEPENDENCY_H
#define RESOURCE_DEPENDENCY_H

#include <string>
#include "tmp/tmp.h"

/* Representation of the physical IO device used (disk, network, etc) */
template <typename T>
class io_object
{

};

/* The input object should represent the input data that is loaded from outside the program,
 * i.e. files, network communication or databases, etc. Note that this class declares the interface
 * but using type T directly as an object is fine */
template <typename T>
class input_object
{
private:
    T instance;
public:

    using io_object = T::io_object;
    using output_type = T::output_type;

    /* If the input data can be accessed, then return true, otherwise false */
    bool        is_valid() const        { return instance.is_valid(); }
    /* Display the error, or if no error, display success */
    std::string error_msg() const       { return instance.error_msg(); }
    /* Try to perform the load */
    void        load()                  { instance.load(); }
    /* Perform necessary work to enable a succesful load */
    void        correct_error()         { instance.correct_error(); }
    /* Display progress */
    std::string progress_msg() const    { return instance.progress_msg(); }
    /* Is loading completed successfully? */
    bool        is_completed() const    { return instance.is_completed(); }
    /* Get the data */
    output_type operator()() const      { return instance(); }
    /* Discard the data */
    void        discard()               { instance.discard(); }
    /* Lock/Unlock data from use */
    void        set_lock(bool b) const  { instance.set_lock(b); }
};

template <typename T>
class resource_object
{
private:
    T instance;
public:
    using input_type = T::input_type;
    using output_type = T::output_type;

    template <typename U>
    void        set_input(const resource_object<U> & res) { instance.set_input(res); }
    template <typename U>
    void        set_input(const input_object<U> & res) { instance.set_input(res); }

    /* If the input data can be accessed, then return true, otherwise false */
    bool        is_valid() const        { return instance.is_valid(); }
    /* Display the error, or if no error, display success */
    std::string error_msg() const       { return instance.error_msg(); }
    /* Perform necessary work to enable a succesful load */
    void        correct_error()         { instance.correct_error(); }
    /* Try to perform the load */
    void        load()                  { instance.load(); }
    /* Display progress */
    std::string progress_msg() const    { return instance.progress_msg(); }
    /* Is loading completed successfully? */
    bool        is_completed() const    { return instance.is_completed(); }
    /* Get the data */
    output_type operator()() const      { return instance(); }
    /* Discard the data */
    void        discard()               { instance.discard(); }
    /* Lock/Unlock data from use */
    void        set_lock(bool b) const  { instance.set_lock(b); }
};

/* T2 depends on resource T1 */
template <typename T1, typename T2>
struct dependency
{

};

namespace detail
{
    template <typename O>
    struct is_input_object
    {
        enum { value = false };
    };
    template <typename O>
    struct is_input_object<input_object<O>>
    {
        enum { value = true };
    };
}

template <typename ... DS>
class dependencies
{
    using input_objects = tmp::set_of<tmp::filter<detail::is_input_object,tuple_t<DS...>>;
};









#endif // RESOURCE_DEPENDENCY_H
