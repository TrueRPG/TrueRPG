include(ccache.cmake)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

set(CMAKE_C_COMPILER_TARGET x86_64-pc-linux-gnu)
set(CMAKE_CXX_COMPILER_TARGET x86_64-pc-linux-gnu)

# TODO: FORTIFY_SOURCE?
set(COMMON_CFLAGS "-fPIC -flto=thin -fstack-protector-strong")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_CFLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_CFLAGS}")

# TODO: LLVM libunwind
set(COMMON_LDFLAGS "-fuse-ld=lld -rtlib=compiler-rt -Wl,--as-needed -Wl,-z,relro,-z,now -pie -fPIC")

set(CMAKE_EXE_LINKER_FLAGS_INIT "${COMMON_LDFLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "${COMMON_LDFLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "${COMMON_LDFLAGS}")
