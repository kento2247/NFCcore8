//関数一覧
//String nfc_read_name()
//bool nfc_write_name(String FirstName, String LastName)
//bool nfc_version_check
//bool nfc_version_update(byte WriteData[])
//bool nfc_register()
//bool nfc_show_block1Data()
//bool nfc_block_erase(byte block_number) 


String nfc_read_name() {
  Serial.println(F("NFC begin read user_name data"));
  byte nfc_nameData[18];
  String returnString;   //呼び出し元に返送するための変数を生成
  if (nfc_read(nfc_nameData, 2) == true) {
    Serial.print(F(" raw name data = "));
    for (byte i = 0; i < 16; i++) {
      if (nfc_nameData[i] == 0) break;       //変換完了
      Serial.print(nfc_nameData[i]);
      returnString += char(nfc_nameData[i]); //1文字ずつ変換して、文字列に付け加えていく
      Serial.print(F(" "));
    }
    Serial.println();
    Serial.print(F(">convert result: "));
    Serial.println(returnString);
    Serial.println();
    return returnString;  //変換したデータを呼び出し元に返送する
  }
  else {
    Serial.println(F(">nfc_read_name() failed"));
    Serial.println();
    return String (F("failed"));
  }
}



bool nfc_write_name(String FirstName, String LastName) {
  Serial.println(F("NFC begin write user_name data"));
  String Name;
  for (byte i = 0; i < FirstName.length() - 1; i++) {
    Name += FirstName[i];   //1文字ずつ変換して、文字列に付け加えていく（改行マークがどうしても消えないため）
  }
  Name += '-';  
  for (byte i = 0; i < LastName.length() - 1; i++) {
    Name += LastName[i];   //1文字ずつ変換して、文字列に付け加えていく（改行マークがどうしても消えないため）
  }
  Serial.print(Name);

  if (Name.length() >= 16) {
    Serial.println(F("nfc_write_name() failed (name data is too long)"));   //16要素を超えてしまう場合
    return false;
  }
  else {
    for (byte i = Name.length(); i < 17; i++) {
      Name += "";  //名前が16字以下の場合、残りスペースを0で上書き（前のデータが残っている可能性をなくすため）
    }
    Serial.println(F(">>convert to ASCIIcode"));
    byte nfc_writeData[16] = {   //配列の初期化
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0
    };
    Name.getBytes(nfc_writeData, 16);  //Name変数をASCIIコード配列に変換
    if (!nfc_write(nfc_writeData, 2)) {   //書き込み失敗
      Serial.println(F(">nfc_write_name() failed"));
      Serial.println();
      return false;
    }
    else {
      Serial.println(F(">nfc_writ_ename() is in success"));
      Serial.println();
      return true;
    }
  }
}




bool nfc_version_check() {
  byte block1_data[18];
  if (!nfc_read(block1_data, 1)) return false;
  if (block1_data[0] == NFCCore_version) {
    Serial.println(F("This  is the latest version"));
    Serial.println();
    return true;
  }
  else {
    bool Update;
    Serial.println(F("This is not the latest version"));
    Serial.println(F("Enter 1 if you want to update, enter 0 to end"));
    Serial.print(F("command >> "));
    while (Serial.available()) Serial.read();  //シリアルポートに溜まっているいらない情報を捨てる
    while (!Serial.available() > 0);  //pcからのシリアル信号（コマンド入力）を待つ
    while (Serial.available()) Update = (byte)(Serial.readStringUntil("¥n").toInt());  //読んだ信号をint型の数字に変換
    Serial.println(Update);
    Serial.println();

    if (Update && nfc_version_update(block1_data)) return true;
    if (!Update) return true;
    else return false;
  }
}





bool nfc_version_update(byte WriteData[]) {
  Serial.println(F("NFC begin card version update"));
  WriteData[0] = NFCCore_version;
  if (!nfc_write(WriteData, 1)) {
    Serial.println(F(">nfc_version_update failed"));
    return false;
  }
  else {
    Serial.println(F(">nfc_version_update is in success"));
    return true;
  }
  Serial.println();
}



bool nfc_register() {
  byte block1_data[18];
  byte block2_data[18];
  if (!nfc_read(block1_data, 1))return false;
  if (!nfc_read(block2_data, 2))return false;
  if (block1_data[13] == 0 || block1_data[14] == 0  || block2_data[1]  == 0) {

    Serial.print(F("The registration is incomplete > "));
    Serial.println(F("You have to re-register"));

    Serial.println(F("Please,enter your organization number between 0 and 255."));
    Serial.print(F("Organization number >> "));
    byte organizationNum;
    while (Serial.available()) Serial.read();  //シリアルポートに溜まっているいらない情報を捨てる
    while (!Serial.available() > 0);  //pcからのシリアル信号（コマンド入力）を待つ
    while (Serial.available()) organizationNum = (byte)(Serial.readStringUntil("¥n").toInt());  //改行マークがあるまで読んだ信号をbyte型の数字に変換
    Serial.println(organizationNum);
    Serial.println();

    Serial.println(F("Please,enter your user number between 0 and 65024."));
    Serial.print(F("User number >> "));
    unsigned int userNum;
    while (Serial.available()) Serial.read();  //シリアルポートに溜まっているいらない情報を捨てる
    while (!Serial.available() > 0);  //pcからのシリアル信号（コマンド入力）を待つ
    while (Serial.available()) userNum = (unsigned int)(Serial.readStringUntil("¥n").toInt());  //改行マークがあるまで読んだ信号をunsigned int型の数字に変換
    Serial.println(userNum);
    Serial.println();

    Serial.println(F("Please,enter your name"));
    Serial.print(F("First name >> "));
    String firstName;
    while (Serial.available()) Serial.read();  //シリアルポートに溜まっているいらない情報を捨てる
    while (!Serial.available() > 0);  //pcからのシリアル信号（コマンド入力）を待つ
    while (Serial.available()) firstName = Serial.readStringUntil("¥n");  //改行マークがあるまで信号を読む。（元がString型なため、変換は必要ない）
    Serial.println(firstName);
    Serial.println();
    Serial.print(F("Last name >> "));
    String lastName;
    while (Serial.available()) Serial.read();  //シリアルポートに溜まっているいらない情報を捨てる
    while (!Serial.available() > 0);  //pcからのシリアル信号（コマンド入力）を待つ
    while (Serial.available()) lastName = Serial.readStringUntil("¥n");  //改行マークがあるまで信号を読む。（元がString型なため、変換は必要ない）
    Serial.println(lastName);
    Serial.println();


    Serial.println(F("Please,touch your card again"));

    while (!nfc_check());
    block1_data[0] = NFCCore_version;
    block1_data[13] = organizationNum;

    if (userNum > 255) {
      byte lowData;
      byte highData;
      lowData = lowByte(userNum);
      highData = lowByte(userNum);
      block1_data[14] = highData;
      block1_data[15] = lowData;
    }

    else {  
      //1byteの数値は上位バイトと下位バイトに分けようがないので、両方とも1byteの数値そのままが出るバグが発生。
      //それ対策に1byteの数値はlowByte関数等を使わないでそのまま15要素目に代入
      block1_data[14] = userNum;
      block1_data[15] = 0;
    }

    nfc_write(block1_data, 1);
    nfc_write_name(firstName, lastName);
    return false;
  }
  return true;
}



bool nfc_show_block1Data() {
  byte block1_data[18];
  if (!nfc_read(block1_data, 1)) return false;
  Serial.println(F("NFC begin show block1 data"));
  Serial.print(F(" >card software version: "));
  Serial.println(block1_data[0]);
  Serial.print(F(" >NFCCore software version: "));
  Serial.println(NFCCore_version);
  Serial.print(F(" >organization number: "));
  Serial.println(block1_data[13]);
  Serial.print(F(" >user number: "));
  if (block1_data[15] == 0) {
    Serial.println(block1_data[14]);
  }
  else {
    Serial.println(makeWord(block1_data[14], block1_data[15]));
  }
  nfc_read_name();
  return true;
}


bool nfc_block_erase(byte block_number) {
  Serial.print(F("NFC begin erase block"));
  Serial.print(block_number);
  Serial.println(F(" data and fill all with 0"));
  byte Write_block_Data[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  if (!nfc_write(Write_block_Data, block_number)) {
    Serial.print(F(">NFC begin block failed"));
    return false;
  }
  else {
    Serial.println(F(">nfc_block_erase() is in success"));
    return true;
  }
}
