# Splitting the heap to control data placement
This project implements an idea to partition the heap of an application into several units, which are identifiable by e.g. name, in a pseudo file system or a dump of address ranges.
The design philosphy is resembled in Multics, where a SingleStore principle is present.

The Implementaiton is closely coupled with jemalloc, but does not neccessarily have to.
We build on concepts and ideas of jemalloc.

## Arena
An arena is the unit off which memory is allocated, meaning an arena is a collection of memory pages.
A page is assigned to an arena in a 1:1 fashion.
We use a jemalloc arena, which has some consequences, e.g. an arena is locked, so having multiple threads allocating memory from a single arena will be a bottleneck.
Also an arena has a thread-cache, which needs to get flushed when an arena switch is done.

## Extent Hooks
Jemalloc has the ability to register extent hooks to an arena, which are called when the arena needs additional memory pages from the operating system.
It is in the responsability of the extent hook to provide those, release them and so on.

## Placeguard
A placeguard is a construct which makes it easy to partition the heap of an appliation. As ...

## Placeguard Stack
To manage the placeguards and decide which placeguard is active, moses internally uses a stack datastructure, with the top placeguard being active.

## Place
A place is a logical abstraction from an arena...
We would need an arena per NUMA-node, perhaps even per core to mitigate contention.