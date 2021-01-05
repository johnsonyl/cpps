#
#  Copyright (C) 2013-2016 MariaDB Corporation AB
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the COPYING-CMAKE-SCRIPTS file.
#
# plugin installation

MACRO(INSTALL_PLUGIN name binary_dir)
  INSTALL(TARGETS ${name} COMPONENT ClientPlugins DESTINATION ${INSTALL_PLUGINDIR})
  IF(MSVC)
    INSTALL(FILES $<TARGET_PDB_FILE:${name}> COMPONENT Debuginfo
      DESTINATION symbols CONFIGURATIONS Debug RelWithDebInfo)
  ENDIF()
  IF(WIN32)
    FILE(APPEND ${CC_BINARY_DIR}/win/packaging/plugin.conf "<File Id=\"${name}.dll\" Name=\"${name}.dll\" DiskId=\"1\" Source=\"${binary_dir}/${CMAKE_BUILD_TYPE}/${name}.dll\"/>\n")
    FILE(APPEND ${CC_BINARY_DIR}/win/packaging/plugin.conf "<File Id=\"${name}.pdb\" Name=\"${name}.pdb\" DiskId=\"1\" Source=\"${binary_dir}/${CMAKE_BUILD_TYPE}/${name}.pdb\"/>\n")
  ENDIF()
ENDMACRO()
