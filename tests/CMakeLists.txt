macro(carnot_test CARNOT_TEST_NAME )
    add_executable(${CARNOT_TEST_NAME} "test_${CARNOT_TEST_NAME}.cpp")
    target_link_libraries(${CARNOT_TEST_NAME} carnot)
endmacro()

carnot_test(stroke)
carnot_test(trigger)
