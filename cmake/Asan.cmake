# Address Sanitizer
# https://clang.llvm.org/docs/AddressSanitizer.html

set(ASAN_COMPILE_FLAGS -g -fsanitize=address,undefined -fno-sanitize-recover=all)
set(ASAN_LINK_FLAGS -g -fsanitize=address,undefined)

if(CMAKE_BUILD_TYPE MATCHES ASAN)
    message(STATUS "Sanitize with address sanitizer")
    add_compile_options(${ASAN_COMPILE_FLAGS})
    add_link_options(${ASAN_LINK_FLAGS})
endif()
