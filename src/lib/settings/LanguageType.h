#pragma once

#include <string>

#include "SourceGroupType.h"

enum LanguageType {
#if BUILD_CXX_LANGUAGE_PACKAGE
  LANGUAGE_CPP,
  LANGUAGE_C,
#endif    // BUILD_CXX_LANGUAGE_PACKAGE
  LANGUAGE_CUSTOM,
  LANGUAGE_UNKNOWN
};

std::string languageTypeToString(LanguageType t);

LanguageType stringToLanguageType(std::string s);

LanguageType getLanguageTypeForSourceGroupType(SourceGroupType t);