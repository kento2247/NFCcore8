#include "NFCcore8.h"
NFCcore8 nfc; //mfrcクラスのインスタンス化

void setup() {
  nfc.setup(5, 17, true);  //ピン番号をアタッチ(SDA=5,RST=17)し、nfcクラスの初期設定完了。シリアルモニターにログ表示あり
}


void loop() {
  //読み込み処理、クレジット減算
  delay(1000);
  nfc.waitTouch();
//  nfc.versionUpdate();
//  nfc.writeName("kento", "tokura");
  nfc.readName();

  Serial.println("User name is " + nfc.userName);
  Serial.println("card programed version is " + String(nfc.cardNFCcoreVersion));
  Serial.println("Latest update time is " + String(nfc.Date[0]) + "/" + String(nfc.Date[1]) + "/" + String(nfc.Date[2]) + " " + String(nfc.Date[3]) + ":" + String(nfc.Date[4]) + ":" + String(nfc.Date[5]));
}
