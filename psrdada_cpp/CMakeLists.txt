# define the cheetah libraries
set(PSRDADA_CPP_LIBRARIES ${CMAKE_PROJECT_NAME} ${DEPENDENCY_LIBRARIES})

include_directories(${CMAKE_SOURCE_DIR})

set(psrdada_cpp_src
    src/cli_utils.cpp
    src/dada_client_base.cpp
    src/dada_db.cpp
    src/dada_disk_sink.cpp
    src/dada_null_sink.cpp
    src/dada_output_stream.cpp
    src/dada_read_client.cpp
    src/dada_write_client.cpp
    src/file_to_dada_cli.cpp
    src/multilog.cpp
    src/psrdadaheader.cpp
    src/raw_bytes.cpp
    src/sigprocheader.cpp
    src/simple_file_writer.cpp
   )

set(psrdada_cpp_inc
    cli_utils.hpp
    common.hpp
    cuda_utils.hpp
    dada_client_base.hpp
    dada_db.hpp
    dada_disk_sink.hpp
    dada_input_stream.hpp
    dada_junk_source.hpp
    dada_null_sink.hpp
    dada_output_stream.hpp
    dada_read_client.hpp
    dada_sync_source.hpp
    dada_write_client.hpp
    double_buffer.cuh
    double_buffer.hpp
    double_device_buffer.cuh
    double_host_buffer.cuh
    file_input_stream.hpp
    multilog.hpp
    psrdada_to_sigproc_header.hpp
    psrdadaheader.hpp
    raw_bytes.hpp
    sigprocheader.hpp
    simple_file_writer.hpp
  )

# -- the main library target
add_library(${CMAKE_PROJECT_NAME} ${psrdada_cpp_src})

#junkdb
add_executable(junkdb examples/junkdb.cpp)
target_link_libraries (junkdb ${PSRDADA_CPP_LIBRARIES})

#syncdb
add_executable(syncdb examples/syncdb.cpp)
target_link_libraries (syncdb ${PSRDADA_CPP_LIBRARIES})

#fbfuse_output_db
add_executable(fbfuse_output_db examples/fbfuse_output_db.cpp)
target_link_libraries (fbfuse_output_db ${PSRDADA_CPP_LIBRARIES})

#file_to_dada_cli
add_executable(file_to_dada src/file_to_dada_cli.cpp)
target_link_libraries (file_to_dada ${PSRDADA_CPP_LIBRARIES})

#dbnull
add_executable(dbnull examples/dbnull.cpp)
target_link_libraries (dbnull ${PSRDADA_CPP_LIBRARIES})

#dbreset
add_executable(dbreset examples/dbreset.cpp)
target_link_libraries (dbreset ${PSRDADA_CPP_LIBRARIES})

#dbisk
#add_executable(dbdisk examples/dbdisk.cpp)
#target_link_libraries (dbisk ${PSRDADA_CPP_LIBRARIES})

install (TARGETS junkdb dbnull syncdb dbreset fbfuse_output_db DESTINATION bin)
install (TARGETS ${CMAKE_PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
install(FILES ${psrdada_cpp_inc} DESTINATION include/psrdada_cpp)
install(DIRECTORY detail DESTINATION include/psrdada_cpp)

add_subdirectory(meerkat)
add_subdirectory(effelsberg)
