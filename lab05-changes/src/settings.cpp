#include "settings.h"
#include <QFile>
#include <QSettings>

Settings settings;


/**
  Loads the application settings, or, if no saved settings are available,
  loads default values for the settings. You can change the defaults here.
**/
void Settings::loadSettingsOrDefaults() {
    // Set the default values below
    QSettings s("CS123", "Lab05");
    linesEnabled = s.value("linesEnabled", false).toBool();
}

void Settings::saveSettings() {
    QSettings s("CS123", "Lab05");
    s.setValue("linesEnabled", linesEnabled);
    s.setValue("fillMode", shaderProgram);

}
