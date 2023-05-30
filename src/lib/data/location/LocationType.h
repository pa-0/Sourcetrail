#pragma once

enum LocationType : int {
  LOCATION_TOKEN = 0,
  LOCATION_SCOPE = 1,
  LOCATION_QUALIFIER = 2,
  LOCATION_LOCAL_SYMBOL = 3,
  LOCATION_SIGNATURE = 4,
  LOCATION_COMMENT = 5,
  LOCATION_ERROR = 6,
  LOCATION_FULLTEXT_SEARCH = 7,
  LOCATION_SCREEN_SEARCH = 8,
  LOCATION_UNSOLVED = 9
};

int locationTypeToInt(LocationType type);

LocationType intToLocationType(int value);