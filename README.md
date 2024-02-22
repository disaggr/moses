
moses
=====


Building Instructions
---------------------

initiate the git submodules

$> git submodule update --init

prepare jemalloc

$> cd jemalloc/
$> ./autogen.sh
$> ./configure --with-jemalloc-prefix=je_
$> make

prepare moses

$> mkdir build/
$> cd build/
$> cmake ..
$> make
