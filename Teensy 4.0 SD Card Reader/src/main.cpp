#include <SPI.h>
#include <SdFat.h>

SdFat SD;       // Create an SdFat object
SdFile root;    // SdFat's file object to represent the root directory
SdFile file;
FsFile n_file;

const int chipSelect = 10;  // Define your SD card CS pin for external SD readers

void listFiles(SdFile& dir, int numTabs);

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ; // Wait for the serial port to connect (Teensy 4.0 specific)
  }

  Serial.println("Initializing SD card...");

  // Initialize the SD card using SdFat
  if (!SD.begin(chipSelect, SD_SCK_MHZ(25)))
  {
    Serial.println("SD card initialization failed!");
    return;
  }

  Serial.println("SD card initialized.");

  // SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

  // Open the root directory
  if (!root.open("/")) {
    Serial.println("Failed to open root directory!");
    return;
  }

  n_file = SD.open("test.txt", FILE_WRITE);
  if (n_file) {
    Serial.print("Writing to test.txt...");
    n_file.println("testing 1, 2, 3.");
    // close the file:
    n_file.close();
  }


  // List files and sizes manually
  while(file.openNext(&root, O_RDONLY))
  {
    int len = 20;
    char name [len];
    file.getName(name, len);
    int size = file.fileSize();
    Serial.print("Name: ");
    Serial.print(name);
    Serial.print('\t');
    Serial.println(size);
    file.close();
  }

  Serial.println("");
  root.open("/");
  // file.rewindDirectory();
  file.openNext(&root, O_RDONLY);
  int len = 20;
  char name [len];
  file.getName(name, len);
  int size = file.fileSize();
  Serial.print("Name: ");
  Serial.print(name);
  Serial.print('\t');
  Serial.println(size);

  // Read actual bytes from .bmp into buffer
  size_t b_size = 36600;
  char buffer [b_size];
  int starttime = millis();
  int bytes_read = file.read(buffer, b_size);
  int endtime = millis();
  Serial.print("Bytes read: ");
  Serial.println(bytes_read);
  Serial.print("Duration: ");
  Serial.println(endtime - starttime);
  file.close();



  // List files and directories
  // SD.ls();

  // Serial.println(root.open("frame000.bmp", O_READ ));

  // Serial.print("File Size is: ");
  // Serial.println(root.fileSize());

  // // size_t byte_count = root.fileSize();
  // size_t byte_count = 50000;
  // char buffer[50000];
  // Serial.println(root.read(buffer, byte_count));



}

void loop()
{
  // Nothing to do here
}
