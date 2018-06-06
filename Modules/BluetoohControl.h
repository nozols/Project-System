#ifndef BluetoothControl_h
#define BluetoothControl_h

int bluetooth_modePointer = 0;
String bluetooth_inCommand = "";

void bluetooth_init(int *mode)
{
  bluetooth_modePointer = mode;
}

void bluetooth_loop()
{
  while(Serial1.available() >= 1)
  {
    char read = Serial1.read();

    if(read == '\n')
    {
      bluetooth_parseCommand(bluetooth_inCommand);
      str = "";
    }
    else
    {
      str += read;
    }
  }
}

#endif
