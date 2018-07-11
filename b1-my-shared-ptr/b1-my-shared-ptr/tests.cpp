#define CATCH_CONFIG_MAIN
#include "Catch.h"

/*

    In this exercise, we make our own (simplified) shared_ptr, which we'll call my_shared_ptr.

    A shared pointer is relatively simple to make, but it's important to have the correct mental model.

        my_shared_pointer ------> pointee

    The primary purpose of a shared pointer is automating memory management: it is responsible for deallocating
    the memory used by the pointee.

    The pointee always resides on the heap. If it were allocated on the stack, there would be no need
    for a smart pointer, as every stack allocated object is already automatically freed
    whenever it goes out of scope.


    A shared pointer (contrary to a unique pointer) allows many pointers pointing to the same pointee X.
    As long as there is at least one shared pointer pointing to X, X should be kept alive.
    When the last shared pointer "dies", it should take X with it, i.e. delete X.
    For this to be possible, each smart pointer needs to know which other smart pointers
    are pointing to X. In other words, the smart pointers need some way to communicate with each other,
    keep updates about who else is pointing to X.

    We can simplify this a bit: it is not important to know *which* smart pointers are currently pointing to X;
    instead, they only need to know *how many* are. This is generally called
    the "reference count" of "refcount". Do not get confused by the usage of the word "reference",
    it does not refer specifically to C++ references. The refcount of an object is the number of
    shared pointers pointing to it.

                                       p5
                                       |
                                       v

     p1 ---->  Foo                    Bar <--------- p3                   Qux <----- p6

                ^                      ^
                |                      |
                p2                     p4

    
    The refcount of Foo is 2
                    Bar is 3
                    Qux is 1


    It is important to realize that the refcount of a pointee should be shared by all shared pointers pointing to the same pointee.
    In the example above, there are 6 pointers in total but only three refcounts. 

    Therefore, the following implementation would not work:

    class my_shared_pointer {
    int refcount;
    ...
    };

    This would mean that each shared pointer has its own refcount (= 6 refcounts total), while there should be only one per pointee.
    The refcount will need to be an int on the heap. Each shared pointer will need to keep a pointer to this refcount.

*/
