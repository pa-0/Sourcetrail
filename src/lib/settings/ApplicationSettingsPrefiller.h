#pragma once

class ApplicationSettings;

class ApplicationSettingsPrefiller {
public:
  static void prefillPaths(ApplicationSettings* settings);

private:
  static bool prefillCxxHeaderPaths(ApplicationSettings* settings);
  static bool prefillCxxFrameworkPaths(ApplicationSettings* settings);
};