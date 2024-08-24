# build plugin
g++ -I`gcc -print-file-name=plugin`/include -fPIC -fno-rtti -shared src/mirror.cc -DDEBUG=1 -o mirror.so

# build example target
gcc example_target.c -fplugin=`pwd`/mirror.so -fdump-tree-original-raw -o example_target
