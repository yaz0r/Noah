cmake_minimum_required (VERSION 3.0)
project (bin2c)

ADD_EXECUTABLE(${PROJECT_NAME} bin2c.cpp)

function( add_bin2c ARG_FILE PARENT_OUTPUT_DEPENDENCY)
	string(REPLACE  ".bin" ".generated.h" OUTPUT_DEPENDENCY ${ARG_FILE})
	set(${PARENT_OUTPUT_DEPENDENCY} ${OUTPUT_DEPENDENCY} PARENT_SCOPE)

	set( COMMANDS "" )

	list( APPEND COMMANDS COMMAND "$<TARGET_FILE:bin2c>" ${ARG_FILE} )

	# Add command
	add_custom_command(
		MAIN_DEPENDENCY ${ARG_FILE}
		OUTPUT ${OUTPUT_DEPENDENCY}
		COMMAND bin2c ${ARG_FILE} ${OUTPUT_DEPENDENCY}
		COMMENT "Compiling bin2c ${ARG_FILE}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
endfunction()

function(add_bin2c_list source_files parent_output_dependencies)
	set(output_dependencies "")
	foreach(source_file ${${source_files}})
		set_source_files_properties(${source_file} PROPERTIES HEADER_FILE_ONLY TRUE)
		set(output_dependency, "")
		add_bin2c("${source_file}" output_dependency)
		list( APPEND output_dependencies ${output_dependency})
	endforeach(source_file)
	set(parent_output_dependencies output_dependencies PARENT_SCOPE)
endfunction(add_bin2c_list)


