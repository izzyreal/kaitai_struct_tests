cmake_minimum_required(VERSION 3.11)

project(KS_TEST_CPP_STL)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Iconv REQUIRED)

# Enforce UTF-8 source files encoding for MSVC
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(RUNTIME_SRC_PATH ../../../runtime/cpp_stl)
set(PREREQ_PATH prereq)
set(KS_RW_PATH "${KS_PATH}_write")

include(${INC_PATH})

set(PREREQ_SOURCES
	${PREREQ_PATH}/my_custom_fx.cpp
	${PREREQ_PATH}/custom_fx.cpp
	${PREREQ_PATH}/custom_fx_no_args.cpp
)

set(RUNTIME_SOURCES
	${RUNTIME_SRC_PATH}/kaitai/kaitaistream.cpp
)

set(RW_SOURCES)
if (EXISTS "${KS_RW_PATH}/hello_world_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/hello_world_write.cpp")
	add_compile_definitions(KS_HAS_RW_HELLO_WORLD=1)
endif()
if (EXISTS "${KS_RW_PATH}/fixed_record_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/fixed_record_write.cpp")
	add_compile_definitions(KS_HAS_RW_FIXED_RECORD=1)
endif()
if (EXISTS "${KS_RW_PATH}/nested_record_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/nested_record_write.cpp")
	add_compile_definitions(KS_HAS_RW_NESTED_RECORD=1)
endif()
if (EXISTS "${KS_RW_PATH}/repeat_expr_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/repeat_expr_write.cpp")
	add_compile_definitions(KS_HAS_RW_REPEAT_EXPR=1)
endif()
if (EXISTS "${KS_RW_PATH}/repeat_expr_bytes_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/repeat_expr_bytes_write.cpp")
	add_compile_definitions(KS_HAS_RW_REPEAT_EXPR_BYTES=1)
endif()
if (EXISTS "${KS_RW_PATH}/repeat_expr_user_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/repeat_expr_user_write.cpp")
	add_compile_definitions(KS_HAS_RW_REPEAT_EXPR_USER=1)
endif()
if (EXISTS "${KS_RW_PATH}/if_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/if_write.cpp")
	add_compile_definitions(KS_HAS_RW_IF=1)
endif()
if (EXISTS "${KS_RW_PATH}/repeat_until_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/repeat_until_write.cpp")
	add_compile_definitions(KS_HAS_RW_REPEAT_UNTIL=1)
endif()
if (EXISTS "${KS_RW_PATH}/repeat_eos_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/repeat_eos_write.cpp")
	add_compile_definitions(KS_HAS_RW_REPEAT_EOS=1)
endif()
if (EXISTS "${KS_RW_PATH}/bits_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/bits_write.cpp")
	add_compile_definitions(KS_HAS_RW_BITS_WRITE=1)
endif()
if (EXISTS "${KS_RW_PATH}/bits_write_le.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/bits_write_le.cpp")
	add_compile_definitions(KS_HAS_RW_BITS_WRITE_LE=1)
endif()
if (EXISTS "${KS_RW_PATH}/pos_instance_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/pos_instance_write.cpp")
	add_compile_definitions(KS_HAS_RW_POS_INSTANCE=1)
endif()
if (EXISTS "${KS_RW_PATH}/io_pos_instance_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/io_pos_instance_write.cpp")
	add_compile_definitions(KS_HAS_RW_IO_POS_INSTANCE=1)
endif()
if (EXISTS "${KS_RW_PATH}/raw_substream_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/raw_substream_write.cpp")
	add_compile_definitions(KS_HAS_RW_RAW_SUBSTREAM=1)
endif()
if (EXISTS "${KS_RW_PATH}/raw_substream_sized_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/raw_substream_sized_write.cpp")
	add_compile_definitions(KS_HAS_RW_RAW_SUBSTREAM_SIZED=1)
endif()
if (EXISTS "${KS_RW_PATH}/repeat_expr_raw_substream_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/repeat_expr_raw_substream_write.cpp")
	add_compile_definitions(KS_HAS_RW_REPEAT_EXPR_RAW_SUBSTREAM=1)
endif()
if (EXISTS "${KS_RW_PATH}/repeat_eos_raw_substream_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/repeat_eos_raw_substream_write.cpp")
	add_compile_definitions(KS_HAS_RW_REPEAT_EOS_RAW_SUBSTREAM=1)
endif()
if (EXISTS "${KS_RW_PATH}/repeat_until_raw_substream_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/repeat_until_raw_substream_write.cpp")
	add_compile_definitions(KS_HAS_RW_REPEAT_UNTIL_RAW_SUBSTREAM=1)
endif()
if (EXISTS "${KS_RW_PATH}/switch_user_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/switch_user_write.cpp")
	add_compile_definitions(KS_HAS_RW_SWITCH_USER=1)
endif()
if (EXISTS "${KS_RW_PATH}/params_stateful_switch_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/params_stateful_switch_write.cpp")
	add_compile_definitions(KS_HAS_RW_PARAMS_STATEFUL_SWITCH=1)
endif()
if (EXISTS "${KS_RW_PATH}/io_lookahead_switch_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/io_lookahead_switch_write.cpp")
	add_compile_definitions(KS_HAS_RW_IO_LOOKAHEAD_SWITCH=1)
endif()
if (EXISTS "${KS_RW_PATH}/running_status_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/running_status_write.cpp")
	add_compile_definitions(KS_HAS_RW_RUNNING_STATUS=1)
endif()
if (EXISTS "${KS_RW_PATH}/mpc2000snd_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/mpc2000snd_write.cpp")
	add_compile_definitions(KS_HAS_RW_MPC2000SND=1)
endif()
if (EXISTS "${KS_RW_PATH}/mpc2000pgm_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/mpc2000pgm_write.cpp")
	add_compile_definitions(KS_HAS_RW_MPC2000PGM=1)
endif()
if (EXISTS "${KS_RW_PATH}/mpc2000aps_write.cpp")
	list(APPEND RW_SOURCES "${KS_RW_PATH}/mpc2000aps_write.cpp")
	add_compile_definitions(KS_HAS_RW_MPC2000APS=1)
endif()

set(RW_TEST_SOURCES
	test_hello_world_write.cpp
	test_fixed_record_write.cpp
	test_nested_record_write.cpp
	test_repeat_expr_write.cpp
	test_repeat_expr_bytes_write.cpp
	test_repeat_expr_user_write.cpp
	test_if_write.cpp
	test_repeat_until_write.cpp
	test_repeat_eos_write.cpp
	test_bits_write.cpp
	test_bits_write_le.cpp
	test_pos_instance_write.cpp
	test_io_pos_instance_write.cpp
	test_raw_substream_write.cpp
	test_raw_substream_sized_write.cpp
	test_repeat_expr_raw_substream_write.cpp
	test_repeat_eos_raw_substream_write.cpp
	test_repeat_until_raw_substream_write.cpp
	test_switch_user_write.cpp
	test_params_stateful_switch_write.cpp
	test_io_lookahead_switch_write.cpp
	test_running_status_write.cpp
	test_mpc2000snd_write.cpp
	test_mpc2000pgm_write.cpp
	test_mpc2000aps_write.cpp
)

add_executable (ks_tests
	${RUNTIME_SOURCES}
	${PREREQ_SOURCES}
	${RW_SOURCES}
	${RW_TEST_SOURCES}
	${DISPOSABLE_SOURCES}
)

include_directories(
	"${PROJECT_BINARY_DIR}"
	"${KS_PATH}"
	"${KS_RW_PATH}"
	"${RUNTIME_SRC_PATH}"
	"${PREREQ_PATH}"
	"${ZLIB_INCLUDE_DIRS}"
)

# NB: no quotes around variables here, as it messes up new "optimized
# A debug B" syntax which gets generated on Windows platforms
target_link_libraries(ks_tests
	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	${ZLIB_LIBRARIES}
	${Iconv_LIBRARIES}
)

target_compile_definitions(ks_tests PRIVATE -DKS_ZLIB -DKS_STR_ENCODING_ICONV)

# Maximum warnings emission
#
# See https://github.com/kaitai-io/kaitai_struct_cpp_stl_runtime/blob/dbe1ca1c946794324d0d409fe53d56a397fe8874/Common.cmake#L11-L26
target_compile_options(ks_tests PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/W4>
	$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
		-Wall -Wextra -Wpedantic
		# See <https://gcc.gnu.org/onlinedocs/gcc-13.3.0/gcc/Warning-Options.html#index-Wstrict-aliasing_003dn>:
		#
		# > Level 1: (...) it has very few false negatives. However, it has many false positives.
		#
		# If we run into false positives in the future, we can increase the level, but let's
		# start with the most aggressive option to make sure we don't miss anything.
		-fstrict-aliasing -Wstrict-aliasing=1
	>
)

add_test(NAME ks_tests COMMAND $<TARGET_FILE:ks_tests>)

enable_testing()
