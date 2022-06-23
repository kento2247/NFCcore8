#define Pze_speaker 2

void setup() {
  nfc_setup_uno();
  delay(1000);
}


void loop() {
  while (!nfc_check());
  tone(Pze_speaker, 1700);
  delay(30);
  noTone(Pze_speaker);
  delay(30);
  tone(Pze_speaker, 1700, 70);
  //  nfc_show_block1Data();
  if (!nfc_register()) return;
  //
  nfc_show_block1Data();
  nfc_block_erase(1);
  //  byte void_data[18];
  //  nfc_read(void_data, 2);


  //
  //
  //  nfc_version_check();


  //  byte setKey[6] = {5,5,5,5,5,5};
  //  nfc_key_change(1,setKey);

  //  nfc_antenna_state(false);
  //  for(byte i; i<20; i++) {
  //    if(nfc_check()) break;
  //    Serial.println(i);
  //    delay(100);
  //  }
  //  nfc_antenna_state(true);
  //  while (!nfc_check());
  //  nfc_uid();
  //  nfc_cardtype();

  //  byte NEW_UID[4] = {0x59, 0xa5, 0x60, 0xa3};
  //  byte NEW_UID[4] = {0xa5, 0xa5, 0xa5, 0xa3};
  //  nfc_uid_change(NEW_UID);

  //    byte Data[18];
  //    nfc_read(Data, 0);
  //    byte WriteData[16] = {0,0,0,0};
  //    for(byte i = 4; i < 16; i++) {
  //      WriteData[i] = Data[i];
  //    }
  //    nfc_write(WriteData, 0);
  //  nfc_read_name();
  //  nfc_write_name("kento","tokura");
  delay(1000);
}
