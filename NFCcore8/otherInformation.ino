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
   ・Init 初期化
   ・Crypto 暗号
   ・RF　Radio frequency - Wikipedia, the free encyclopedia
  　　　　　http://en.wikipedia.org/wiki/Radio_frequency
        搬送波が出ているだけの状態でも「RFが出てる」と表現することがある。高周波のことを表すだけのこともある。

    MIFARE Standard と MIFARE UltraLightは、基本的には単なるメモリーカードである。メモリは、セグメントとブロックに分けられ、シンプルなセキュリティのメカニズムによってアクセス制御がなされている。ASICベースのため計算能力は限られている。しかし、安価で高信頼なため、電子マネー、社員証、交通機関や競技場のチケットなどとして用いられている。
  MIFARE Standard 1kは、約768バイトのメモリを持ち、それが16のセクタに分かれている。これらのセクタは、AとBと呼ばれる二つの鍵でプロテクトされている。それぞれのセクタに対して、読み込み、書き込み、値の増減などの操作ができる。MIFARE Standard 4kは、3kBのメモリを持ち、それが40のセクタに分かれている。そのうち32のセクタがMIFARE Standard 1kのセクタと同じ容量で、残りの8つが倍の容量を持つ。MIFARE Standard miniのメモリ容量は320バイトで、5つのセクタを持つ。
  MIFARE UltraLightは、わずか64バイトの容量で、セキュリティ機能もない。非常に安価なため、使い捨てのチケットとして用いられている。2006年ワールドカップでも使われた。
  これらのカードはシンプルなため安価で、そのため大規模に普及した。
  MIFARE Standardは、1994年に発売された非接触ICカードということもあり設計が古く、また独自の暗号化アルゴリズムに脆弱性が判明したため[2]MIFARE Classicに改名された。脆弱性に対応した代替品には、MIFARE Plusがある。
*/


/*  使用例
    nfc_setup_uno();  //RC522とマイコンとの接続設定を全て行う（起動前にSerial.beginを行わないとシリアルモニターでのデバックはできない）
    while (!nfc_check());  //nfcタグがかざされるまで待機
    nfc_antenna_state(false);  //アンテナをオフにして、RC522モジュールでのタグ読み取りを停止する
    nfc_antenna_state(true);   //上記の逆
    if (nfc_uid() == aaaa) Serial.println("OK");  //読み取ったUIDが"aaaa"だった場合...

    byte raedData[18];      //読んだデータを保存するための配列を作成
    nfc_read(raedData, 4);  //ブロック4のデータを読んで配列readataに保存する

    byte raedData[18];      //読んだデータを保存するための配列を作成
    nfc_read(raedData, 4);  //ブロック4のデータを読んで配列readataに保存する
    if(readData[0]==0) Serial.println("No Point");  //ブロック4の１要素目が0だった場合は...

    byte writeData[16] = {0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3};  //書き込む情報を配列writeDataの中に保存する
    nfc_write(writeData, 4);                                 //配列writeDataの情報をブロック4に書き込む

  //カードの情報の一部をバックアップ
    byte raedData[18];      //読んだデータを保存するための配列を作成
    nfc_read(raedData, 4);  //ブロック4のデータを読んで配列readataに保存する
    byte WriteData[16] = {1,1,1,1};  //書き込む情報を配列writeDataの中に保存する
    for(byte i = 4; i < 12; i++) {   //配列writeDataの5~16要素目にnfc_read()で読んだ値の1~12要素目をコピーする
      writeData[i] = readDta[i];
    }
    nfc_write(WriteData, 4);
*/

//MFRC522.cppより引用。未実装で使えそうな関数たち
//
//void nfc_antenna_state(bool state) {
//  Serial.print(F("nfc_antenna_state >> "));
//  if (state) {
//    mfrc522.PCD_AntennaOn();
//    Serial.println(F("activate"));
//  }
//  else {
//    mfrc522.PCD_AntennaOff();
//    Serial.println(F("sleep"));
//  }
//  Serial.println();
//}
//
//
//bool nfc_connected() {
//  Serial.print(F("test connecting... "));
//  //mfrc522.PCD_GetAntennaGain()は接続時に64、そうでない時は0を返す
//  bool state = mfrc522.PCD_GetAntennaGain() / 64;
//  if (state) {
//    Serial.println(F("successed"));
//    Serial.println();
//    return true;
//  }
//  else {
//    Serial.println(F("failed"));
//    return false;
//  }
//}
//
//byte nfc_cardtype() {
//  Serial.print(F("  PICC type: "));
//  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
//  Serial.print(mfrc522.PICC_GetType(mfrc522.uid.sak));
//  Serial.print(": ");
//  Serial.println(mfrc522.PICC_GetTypeName(piccType));
//  return mfrc522.PICC_GetType(mfrc522.uid.sak);
//}


/*MFRC522.hより引用
  StatusCode PCD_Authenticate(byte command, byte blockAddr, MIFARE_Key *key, Uid *uid);
  void PCD_StopCrypto1();
  StatusCode MIFARE_Read(byte blockAddr, byte *buffer, byte *bufferSize);
  StatusCode MIFARE_Write(byte blockAddr, byte *buffer, byte bufferSize);
  StatusCode MIFARE_Ultralight_Write(byte page, byte *buffer, byte bufferSize);  //1pageあたり4byteの配列まで
  StatusCode MIFARE_Decrement(byte blockAddr, int32_t delta);  //指定した分のブロック番号だけ16byteの配列をずらす？
  StatusCode MIFARE_Increment(byte blockAddr, int32_t delta);  //指定した分のブロック番号だけ16byteの配列をずらす？
  StatusCode MIFARE_Restore(byte blockAddr);  //復元？消去？
  StatusCode MIFARE_Transfer(byte blockAddr);  //指定したブロック番号と交換？上書き？
  StatusCode MIFARE_GetValue(byte blockAddr, int32_t *value);  //ブロック内の特定の要素を参照
  StatusCode MIFARE_SetValue(byte blockAddr, int32_t value);
  StatusCode PCD_NTAG216_AUTH(byte *passWord, byte pACK[]);  //Ntag216飲み専用のauthが必要？

  void PCD_DumpVersionToSerial();
  void PICC_DumpToSerial(Uid *uid);
  void PICC_DumpDetailsToSerial(Uid *uid);
  void PICC_DumpMifareClassicToSerial(Uid *uid, PICC_Type piccType, MIFARE_Key *key);
  void PICC_DumpMifareClassicSectorToSerial(Uid *uid, MIFARE_Key *key, byte sector);
  void PICC_DumpMifareUltralightToSerial();
*/
