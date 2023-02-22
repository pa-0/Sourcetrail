# ${CMAKE_SOURCE_DIR}/src/lib_cxx/CMakeLists.txt
find_package(Clang REQUIRED)

if(LLVM_FOUND)
  message(STATUS "Found LLVM ${LLVM_VERSION}")
endif()

if(UNIX)
  set(LLVM_CONFIGURATION_TYPES ".")
endif()

set(CLANG_COMPILER_HEADER_SEARCH_PATH "")
foreach(LLVM_CONFIGURATION_TYPE IN LISTS LLVM_CONFIGURATION_TYPES)
  if(CLANG_COMPILER_HEADER_SEARCH_PATH STREQUAL "")
    set(_CLANG_HEADERS_SEARCH_LIST
      "${LLVM_BINARY_DIR}/${LLVM_CONFIGURATION_TYPE}/lib/clang/${LLVM_VERSION}/include"
      "${LLVM_BINARY_DIR}/${LLVM_CONFIGURATION_TYPE}/lib64/clang/${LLVM_VERSION}/include")
    foreach(_CLANG_HEADER_PATH IN LISTS _CLANG_HEADERS_SEARCH_LIST)
      if(EXISTS ${_CLANG_HEADER_PATH})
        set(CLANG_COMPILER_HEADER_SEARCH_PATH ${_CLANG_HEADER_PATH})
        break()
      endif()
    endforeach ()

    message(STATUS "Trying to find Clang compiler headers in '${LLVM_CONFIGURATION_TYPE}' build config in directory '${CLANG_COMPILER_HEADER_SEARCH_PATH}'.")
    if(EXISTS ${CLANG_COMPILER_HEADER_SEARCH_PATH})
      message(STATUS "Found headers for '${LLVM_CONFIGURATION_TYPE}' build config.")
      file(GLOB_RECURSE CLANG_COMPILER_HEADER_PATHS RELATIVE "${CLANG_COMPILER_HEADER_SEARCH_PATH}" "${CLANG_COMPILER_HEADER_SEARCH_PATH}/*")
      foreach(CLANG_COMPILER_HEADER_PATH IN LISTS CLANG_COMPILER_HEADER_PATHS)
        configure_file("${CLANG_COMPILER_HEADER_SEARCH_PATH}/${CLANG_COMPILER_HEADER_PATH}" "${CMAKE_SOURCE_DIR}/bin/app/data/cxx/include/${CLANG_COMPILER_HEADER_PATH}" COPYONLY)
      endforeach()
    else()
      set(CLANG_COMPILER_HEADER_SEARCH_PATH "")
    endif()
  endif()
endforeach()

if(CLANG_COMPILER_HEADER_SEARCH_PATH STREQUAL "")
  message(FATAL_ERROR "Unable to copy Clang compiler headers from clang build dir.")
endif()

add_library(Sourcetrail_lib_cxx)
add_library(Sourcetrail::lib_cxx ALIAS Sourcetrail_lib_cxx)

target_sources(
  Sourcetrail_lib_cxx
  PRIVATE
    data/indexer/CxxIndexerCommandProvider.cpp
    data/indexer/IndexerCommandCxx.cpp
    data/indexer/IndexerCxx.cpp

    data/parser/cxx/name/CxxDeclName.cpp
    data/parser/cxx/name/CxxFunctionDeclName.cpp
    data/parser/cxx/name/CxxName.cpp
    data/parser/cxx/name/CxxQualifierFlags.cpp
    data/parser/cxx/name/CxxStaticFunctionDeclName.cpp
    data/parser/cxx/name/CxxTypeName.cpp
    data/parser/cxx/name/CxxVariableDeclName.cpp

    data/parser/cxx/name_resolver/CxxDeclNameResolver.cpp
    data/parser/cxx/name_resolver/CxxNameResolver.cpp
    data/parser/cxx/name_resolver/CxxSpecifierNameResolver.cpp
    data/parser/cxx/name_resolver/CxxTemplateArgumentNameResolver.cpp
    data/parser/cxx/name_resolver/CxxTemplateParameterStringResolver.cpp
    data/parser/cxx/name_resolver/CxxTypeNameResolver.cpp

    data/parser/cxx/ASTAction.cpp
    data/parser/cxx/ASTConsumer.cpp
    data/parser/cxx/CanonicalFilePathCache.cpp
    data/parser/cxx/ClangInvocationInfo.cpp
    data/parser/cxx/CommentHandler.cpp
    data/parser/cxx/CxxAstVisitor.cpp
    data/parser/cxx/CxxAstVisitorComponent.cpp
    data/parser/cxx/CxxAstVisitorComponentBraceRecorder.cpp
    data/parser/cxx/CxxAstVisitorComponentContext.cpp
    data/parser/cxx/CxxAstVisitorComponentDeclRefKind.cpp
    data/parser/cxx/CxxAstVisitorComponentImplicitCode.cpp
    data/parser/cxx/CxxAstVisitorComponentIndexer.cpp
    data/parser/cxx/CxxAstVisitorComponentTypeRefKind.cpp
    data/parser/cxx/CxxCompilationDatabaseSingle.cpp
    data/parser/cxx/CxxContext.cpp
    data/parser/cxx/CxxDiagnosticConsumer.cpp
    data/parser/cxx/CxxParser.cpp
    data/parser/cxx/CxxVerboseAstVisitor.cpp
    data/parser/cxx/GeneratePCHAction.cpp
    data/parser/cxx/PreprocessorCallbacks.cpp
    data/parser/cxx/SingleFrontendActionFactory.cpp
    data/parser/cxx/utilityClang.cpp

    project/SourceGroupCxxCdb.cpp
    project/SourceGroupCxxCodeblocks.cpp
    project/SourceGroupCxxEmpty.cpp
    project/SourceGroupFactoryModuleCxx.cpp
    project/utilitySourceGroupCxx.cpp

    utility/codeblocks/CodeblocksCompiler.cpp
    utility/codeblocks/CodeblocksCompilerVarType.cpp
    utility/codeblocks/CodeblocksProject.cpp
    utility/codeblocks/CodeblocksTarget.cpp
    utility/codeblocks/CodeblocksTargetRelationType.cpp
    utility/codeblocks/CodeblocksUnit.cpp
    utility/CompilationDatabase.cpp
    utility/IncludeDirective.cpp
    utility/IncludeProcessing.cpp

    LanguagePackageCxx.cpp)

target_include_directories(
  Sourcetrail_lib_cxx
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/data/indexer/
    ${CMAKE_CURRENT_LIST_DIR}/data/parser/cxx/name/
    ${CMAKE_CURRENT_LIST_DIR}/data/parser/cxx/name_resolver/
    ${CMAKE_CURRENT_LIST_DIR}/data/parser/cxx/
    ${CMAKE_CURRENT_LIST_DIR}/project/
    ${CMAKE_CURRENT_LIST_DIR}/utility/
    ${CMAKE_CURRENT_LIST_DIR}/utility/codeblocks/)

target_include_directories(
  Sourcetrail_lib_cxx
  PRIVATE
    ${LIB_CXX_INCLUDE_PATHS}
    ${LIB_UTILITY_INCLUDE_PATHS}
    ${LIB_INCLUDE_PATHS}
    ${CMAKE_BINARY_DIR}/src/lib)

target_include_directories(
  Sourcetrail_lib_cxx
  SYSTEM PRIVATE
    ${LLVM_INCLUDE_DIRS}
    ${CLANG_INCLUDE_DIRS}
    ${EXTERNAL_INCLUDE_PATHS})

if(LLVM_LINK_LLVM_DYLIB)
  set(REQ_LLVM_LIBS LLVM)
else()
  llvm_map_components_to_libnames(REQ_LLVM_LIBS support core libdriver passes option)
  foreach(LLVM_TARGET IN LIST LLVM_TARGETS_TO_BUILD)
    get_property(lib_deps GLOBAL PROPERTY "LLVMBUILD_LIB_DEPS_LLVM${LLVM_TARGET}CodeGen")
    list(APPEND REQ_LLVM_LIBS "LLVM${LLVM_TARGET}CodeGen")
    list(APPEND REQ_LLVM_LIBS "${lib_deps}")
    get_property(lib_deps GLOBAL PROPERTY "LLVMBUILD_LIB_DEPS_LLVM${LLVM_TARGET}AsmParser")
    if(NOT "${lib_deps}" STREQUAL "")
      list(APPEND REQ_LLVM_LIBS "LLVM${LLVM_TARGET}AsmParser")
      list(APPEND REQ_LLVM_LIBS "${lib_deps}")
    endif()
  endforeach()
endif()

# Should be CLANG_LINK_CLANG_DYLIB in future LLVM release
if(LLVM_LINK_LLVM_DYLIB)
  set(CLANG_LIBRARIES clang-cpp)
else()
  set(CLANG_LIBRARIES
      clangASTMatchers
      clangFrontend
      clangSerialization
      clangDriver
      clangTooling
      clangParse
      clangSema
      clangStaticAnalyzerFrontend
      clangStaticAnalyzerCheckers
      clangStaticAnalyzerCore
      clangAnalysis
      clangRewriteFrontend
      clangEdit
      clangAST
      clangLex
      clangBasic)
endif()

target_link_libraries(
  Sourcetrail_lib_cxx
  PRIVATE
    Sourcetrail::lib_utility
    ${CLANG_LIBRARIES}
    ${REQ_LLVM_LIBS}
    TinyXML::TinyXML
    Qt5::Core)

if(WIN32)
  target_compile_definitions(Sourcetrail_lib_cxx PRIVATE _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS) # Due to Clang
  target_link_libraries(Sourcetrail_lib_cxx PUBLIC version)
endif()
