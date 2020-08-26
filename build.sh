rm -rf build
rm -rf lib
mkdir build
mkdir lib
cd build
emcmake cmake ..
emmake make
cp offset.js ../lib/offset.js
cp offset.wasm ../lib/offset.wasm
