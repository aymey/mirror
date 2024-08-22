# build plugin
g++ -I`gcc -print-file-name=plugin`/include -fPIC -fno-rtti -shared src/mirror.cc -o mirror.so

# build example target
gcc example_target.c -fplugin=`pwd`/mirror.so -o example_target
