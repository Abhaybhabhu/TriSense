// Main entry: forward Arduino setup/loop to logger implementations
#include "csv_logger/CsvLogger.h"

extern "C" void setup() {
  setupLogger();
}

extern "C" void loop() {
  loopLogger();
}
