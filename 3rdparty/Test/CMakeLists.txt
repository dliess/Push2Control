add_executable(TestFmtLib "test_fmtlib.cpp")
target_link_libraries(TestFmtLib fmt)

add_executable(TestLoguru "test_loguru.cpp")
target_link_libraries(TestLoguru Loguru)

add_executable(TestQtUnixSignals "test_qtunixsignals.cpp")
target_link_libraries(TestQtUnixSignals QtUnixSignals)