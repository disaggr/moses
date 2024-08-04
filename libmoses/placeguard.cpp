#include "placeguard.h"

#include <jemalloc/jemalloc.h>

#include <thread>
#include <pthread.h>
#include <string.h>

#define mallctl(C, ...) do { \
    int res = mallctl(C, __VA_ARGS__); \
    if (res != 0) { \
        fprintf(stderr, "%s:%i:mallctl:%s: %i - %s\n", __FILE__, __LINE__, C, res, strerror(res)); \
} } while (0)

namespace moses {

// a thread-local reference to a placeguard stack for each thread
// FIXME: does a new thread inherit its parent placeguard stack? interaction is unclear.
thread_local PlaceGuardStack _pg_stack;


void PlaceGuardStack::Push(Place *place) {
    // push the given place to the top of the placeguard stack, unless it is a repeat of the previous place, then update the jemalloc thread arena
    // FIXME: why is the repeat a special case? wouldn't we get in trouble if we nested the same placeguard, ignored the push, but don't ignore the pop?
    /*if (!_places.empty() && _places.back() == place) {
        return;
    }*/

    _places.push_back(place);
    UpdateArena();
}

void PlaceGuardStack::Pop() {
    // remove the place currently at the top of the placeguard stack, unless the stack would become empty, then update the jemalloc thread arena
    // FIXME: why is the empty stack a special case? don't we start with an empty stack? shouldn't we be able to return to that state?
    /*if (_places.size() == 1) {
        return;
    }*/

    _places.pop_back();
    UpdateArena();
}

Place* PlaceGuardStack::Top() {
    // produce the place currently at the top of the placeguard stack
    return _places.back();
}

void PlaceGuardStack::UpdateArena() {
    unsigned arena_id = 0;
    if(!_places.empty())
    {
        // update jemalloc with the arena currently at the top of the stack
        arena_id = _places.back()->GetArena()->GetId();
    }
    //mallctl("thread.tcache.flush", NULL, NULL, NULL, 0);
    mallctl("thread.arena", NULL, NULL, (void*)&arena_id, sizeof(arena_id));
}

PlaceGuard::PlaceGuard(Place *place) {
    // creating the PlaceGuard pushes the given place to the top of the thread-local place stack
    _pg_stack.Push(place);
}

PlaceGuard::~PlaceGuard() {
    // when the PlaceGuard goes out of scope and is destroyed, its place is remvoed from the top of the stack
    // FIXME: this assumes that placeguards are destroyed in reverse order of their creation. Is that always true?
    _pg_stack.Pop();
}

}
