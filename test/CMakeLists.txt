include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG "v3.1.1"
)
FetchContent_MakeAvailable(Catch2)

function(make_test src target std)
  add_executable(${target} ${src})
  target_compile_options(${target} PRIVATE ${OPTIONS})
  target_link_libraries(${target} PRIVATE ${CMAKE_PROJECT_NAME} Catch2::Catch2WithMain)
  add_test(NAME ${target} COMMAND ${target})
  set_property(TARGET ${target} PROPERTY CXX_STANDARD 20)
endfunction()

make_test(test.cc test-cpp20 c++20)

