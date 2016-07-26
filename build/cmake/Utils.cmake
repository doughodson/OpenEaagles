# TODO: Document this function if it works for other examples
function (EPP2EDL _target _epp)
  string (REPLACE ".epp" ".edl" _edl ${_epp})
  get_filename_component (_edl ${_edl} NAME)
  message (STATUS "${_target}: processing ${_epp} to ${_edl}")
  set (_epp ${CMAKE_CURRENT_SOURCE_DIR}/${_epp})
  set (_edl ${EXECUTABLE_OUTPUT_PATH}/${_edl})
  # TODO: Create a portable version of this command for windows
  add_custom_command (
    TARGET ${_target}
    POST_BUILD
    COMMAND cpp ${ARGN} ${_epp} ${_edl})
endfunction (EPP2EDL)

