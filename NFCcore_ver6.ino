//2020/03/12更新

/*関数一覧
   bool nfc_setup_uno()
   bool nfc_check()
   bool nfc_read(nfc_readData, byte ReadBlock)
   bool nfc_write(byte WriteData[], byte WriteBlock)
   void nfc_setKeyB(byte block)
   bool nfc_autha(byte block)
   bool nfc_authb(byte block)
   String nfc_uid()
   bool ApplePay()
   bool nfc_uid_change(byte newUid[])
   void nfc_reset()
   void nfc_key_show()
   void nfc_antenna_state(bool state)
   bool nfc_connected()
   byte nfc_cardtype()
*/


/*用語集

   ・UID (Unique Identification)　Mifareのカードを識別するためのID個体識別のために利用される
   ・PCD (Proximity Coupling Device)　近接型ICのRead/Writeできる端末を指す
  　　・PICC (Proximity IC Card・近接型カード)　非接触ICカードの中でも近接型のカードを指す
   ・Halt 止める
   ・Init 起動
   ・Crypto 暗号
   ・RF　Radio frequency - Wikipedia, the free encyclopedia
  　　　　　http://en.wikipedia.org/wiki/Radio_frequency
        搬送波が出ているだけの状態でも「RFが出てる」と表現することがある。高周波のことを表すだけのこともある。


*/








#include <MFRC522.h>
#include <SPI.h>


#define NFCCore_version 6
#define RST_PIN 9            // NFC RSTピン(SPI通信時には設定必要)
#define SS_PIN 10             // NFC SDAピン(SPI通信時には設定必要)

MFRC522::StatusCode status;         //ステータス変数を定義
MFRC522 mfrc522(SS_PIN, RST_PIN);   // RC522と接続

MFRC522::MIFARE_Key KeyA = {keyByte: {255, 255, 255, 255, 255, 255}};  //デフォルト
MFRC522::MIFARE_Key KeyB = {keyByte: {255, 255, 255, 255, 255, 255}};  //デフォルト
byte keyB_sector[16][6] = {
  {255, 255, 255, 255, 255, 255}, {255, 255, 255, 255, 255, 255}, {255, 255, 255, 255, 255, 255}, {255, 255, 255, 255, 255, 255},
  {255, 255, 255, 255, 255, 255}, {255, 255, 255, 255, 255, 255}, {255, 255, 255, 255, 255, 255}, {255, 255, 255, 255, 255, 255},
  {255, 255, 255, 255, 255, 255}, {255, 255, 255, 255, 255, 255}, {255, 255, 255, 255, 255, 255}, {255, 255, 255, 255, 255, 255},
  {255, 255, 255, 255, 255, 255}, {255, 255, 255, 255, 255, 255}, {255, 255, 255, 255, 255, 255}, {255, 255, 255, 255, 255, 255}
};
MFRC522::PICC_Type piccType;







bool nfc_setup_uno() {
  Serial.begin(9600);
  Serial.println(F(">begin Serial com"));
  while (!Serial);
  Serial.println(F(">begin SPI com"));
  SPI.begin();
  delay(10);
  Serial.println(F(">initialize nfc_PCD"));
  mfrc522.PCD_Init();  //PCD (Proximity Coupling Device)の初期化
  if (nfc_connected()) return true;
  else return false;
}



bool nfc_check() {  // Mifareカードの確認（新しいカードが無ければ終了し、loop関数を繰り返す）
  nfc_reset(); //これがないと、前回のカード通信が切れずにNewCardPresentを通過してしまう
  delay(10);  //変更可能(タッチ判定の更新速度)
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) return false;
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println(F(">NFCtag touched"));
  Serial.println();
  return true;
}




bool nfc_read(byte nfc_readData[], byte block) {
  Serial.println(F("NFC begin read data"));
  Serial.print(F(" use sector: "));
  Serial.println(int(block / 4));
  Serial.print(F("     block: "));
  Serial.println(block);
  if (!nfc_auth_a(block)) return false;
  byte size = 18; //18固定
  status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(block, nfc_readData,  &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.println(F(">nfc_read() failed"));
    Serial.println();
    return false;
  }
  Serial.print(F(">nfc_read() result: "));
  for (byte i = 0; i < 16; i++) {
    Serial.print(nfc_readData[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.println();
  return true;
}


bool nfc_write(byte WriteData[], byte block) { //書き込む配列、書き込み先ブロック
  //loopに必ず入れること
  //書き込む情報を1バイト*16個保存するための変数（配列）

  //NFC_WRITE()前にdataBlock[16] = { , , , ...}の形式で書き込む内容を定義
  //呼び出しはNFC_WRITE(配列名, ブロック番号);
  //呼び出し時の引数の配列名に[]はいらないので注意


  Serial.println(F("NFC begin write data"));
  Serial.print(F(" use sector: "));
  Serial.println(int(block / 4) );
  Serial.print(F("     block: "));
  Serial.println(block);
  if (!nfc_auth_b(block)) return false;
  byte size = 16; //16固定
  Serial.print(F(" write data "));
  for (byte i = 0; i < 16; i++) {
    Serial.print(WriteData[i]);
    Serial.print(" ");
  }
  Serial.println();
  status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(block, WriteData, size);
  if (status != MFRC522::STATUS_OK) {
    Serial.println(F(">nfc_write() failed"));
    Serial.println();
    return false;
  }
  Serial.println(F(">nfc_write() is in success"));
  Serial.println();
  return true;
}



void nfc_setKeyB(byte sector) {
  Serial.print(F(" set keyB data for sector: "));
  Serial.println(sector);
  Serial.print(F(" KeyB: "));
  for (byte i = 0; i < 6; i++) {
    KeyB.keyByte[i] = keyB_sector[sector][i];
    Serial.print(KeyB.keyByte[i]);
    Serial.print(" ");
  }
  Serial.println();
}


bool nfc_auth_a(byte block) {
  byte sector = int(block / 4);
  byte trailerBlock   = sector * 4 + 3;  //そのセクターの認証キーが格納されているブロック番号
  Serial.print(F("     piccType: "));
  piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(piccType);
  Serial.println(F(" start authenticate typeA"));
  if (piccType != MFRC522::PICC_TYPE_MIFARE_UL) {
    Serial.print(F(" use KeyA: "));
    for (byte i = 0; i < 6; i++) {
      Serial.print(KeyA.keyByte[i]);
      Serial.print(" ");
    }
    Serial.println();
    status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &KeyA, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.println(F(">nfc_auth_a() failed"));
      Serial.println();
      return false;
    }
    else return true;  //MifareULではなく、認証に成功した場合
  }
  return true;  //MifareULの場合
}



bool nfc_auth_b(byte block) {
  byte sector = int(block / 4);
  byte trailerBlock   = sector * 4 + 3;  //そのセクターの認証キーが格納されているブロック番号
  Serial.print(F("     piccType: "));
  piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(piccType);
  Serial.println(F(" start authenticate typeB"));
  if (piccType != MFRC522::PICC_TYPE_MIFARE_UL) {
    nfc_setKeyB(sector);
    status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &KeyB, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.println(F(">nfc_auth_b() failed"));
      Serial.println();
      return false;
    }
    else return true;  //MifareULではなく、認証に成功した場合
  }
  return true;  //MifareULの場合
}


String nfc_uid() {
  Serial.println(F("check card UID ..."));
  String strBuf[mfrc522.uid.size];
  String strUID;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    strBuf[i] =  String(mfrc522.uid.uidByte[i], HEX);
    strUID += strBuf[i] += " ";
  }
  Serial.print(F(">UID = "));
  Serial.println(strUID);
  return strUID;
}


bool ApplePay() {
  Serial.println(F("check card UID & Apple Pay..."));
  if (String(mfrc522.uid.uidByte[0], HEX) == "8") {
    Serial.println(F("Apple Pay"));
    return true;
  }
  else {
    Serial.println(F("isn't Apple Pay"));
    return false;
  }
}



bool nfc_uid_change(byte newUid[]) {
  //Mifare Ultra Liteでのみ使用可能

  //  NFC_UID_CHANGE呼び出し方法
  //  byte NEW_UID[4] = {0xDE, 0xAD, 0xBE, 0xEF};
  //  nfc_uid_change(NEW_UID);

  Serial.println(F("NFC begin change UID"));
  if (! mfrc522.MIFARE_SetUid(newUid, (byte)4, true) ) {
    Serial.println(F(">nfc_uid_change() failed"));
    Serial.println();

    return false;
  }
  Serial.println(F(">nfc_uid_change() is in success"));
  Serial.print(F(">new UID is "));
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  Serial.println();
  return true;
}



void nfc_reset() {
  Serial.println(F("NFC reset()"));
  mfrc522.PICC_HaltA();             // 接続中の端末との通信を停止
  mfrc522.PCD_StopCrypto1();        // 端末とのPCDの暗号化を停止
}



void dump_byte_array(byte * buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? "0" : "");
    Serial.print(buffer[i], HEX);
  }
  Serial.println();
}




void nfc_key_show() {
  Serial.println(F("NFC begin key show"));
  Serial.print(F(">authenticate KeyA :"));
  dump_byte_array(KeyA.keyByte, MFRC522::MF_KEY_SIZE);
  Serial.println();
  Serial.print(F(">authenticate KeyB :"));
  dump_byte_array(KeyB.keyByte, MFRC522::MF_KEY_SIZE);
  Serial.println();
}


void nfc_antenna_state(bool state) {
  Serial.print(F("nfc_antenna_state >> "));
  if (state) {
    mfrc522.PCD_AntennaOn();
    Serial.println(F("activate"));
  }
  else {
    mfrc522.PCD_AntennaOff();
    Serial.println(F("sleep"));
  }
  Serial.println();
}


bool nfc_connected() {
  Serial.print(F("test connecting... "));
  //mfrc522.PCD_GetAntennaGain()は接続時に64、そうでない時は0を返す
  bool state = mfrc522.PCD_GetAntennaGain() / 64;
  if (state) {
    Serial.println(F(">successed"));
    Serial.println();
    return true;
  }
  else {
    Serial.println(F(">failed"));
    return false;
  }
}

byte nfc_cardtype() {
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.print(mfrc522.PICC_GetType(mfrc522.uid.sak));
  Serial.print(": ");
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
  return mfrc522.PICC_GetType(mfrc522.uid.sak);

}
