# Paddle2Mouse-For-ArcadeArchives

このデバイスはAruduino Pro Microを用いてロータリーエンコーダ―とボタン操作をUSB HIDへ変換し、Nintendo Switch/Switch2のアーケードアーカイブスタイトルでマウスとして動作します。（PS4/5では動作確認できていませんが同じ要領で動くと思います）<br>
感度切り替えスイッチを装備し、高感度モードではボード上のLEDが点灯します。<br>
Arduino Pro Micro (5V/16MHz）をPCへ接続し、Arduino IDEからPaddle2MouseForAA.inoを書き込んで使用してください。<br>
■材料と配線<br>
・Arduino Pro Micro (5V/16MHz版)もしくはその互換ボード<br>
・産業用ロータリーエンコーダ (オープンコレクタ出力型、A/B相出力)<br>
-[作者が使用したものはこちら（600パルス）](https://aliexpress.com/item/1005005779039462.html)<br>
・プッシュボタン×2 (左右クリック用)<br>
・トグルスイッチもしくはプッシュボタン×1 (モード切替用)<br>
・ノブ（金属製のものを推奨）<br>
-[作者が使用したものはこちら](https://ja.aliexpress.com/item/4001041459364.html)<br>
・外装（お好みで）<br>
その他、必要に応じて接続ケーブルやピンヘッダなど<br>
<br>
【エンコーダ信号】<br>
A相:Pin 2<br>
B相:Pin 3<br>
【操作入力】（各ボタン・スイッチの片方は以下のピン、もう片方はすべてGNDへ）<br>
左ボタン:Pin 4<br>
右ボタン:Pin 5<br>
モード切替スイッチ:Pin 6<br>
【電源系】<br>
エンコーダの電源入力はArduinoのVCCピンへ、各パーツのGNDはGNDピンへ適当にまとめておく<br>
<br>
■設定項目（カスタマイズ）<br>
プログラム冒頭の「設定項目」を書き換えることで、各モード時の感度を個別に調整が可能です。<br>
```
// --- モード1：低感度（スイッチOFF時） ---
const int LOW_SENS_SPEED = 1; 

// --- モード2：高感度（スイッチON時） ---
const int HIGH_SENS_SPEED = 3;
```
LOWは1～2、HIGHは2～3に設定し、ソフト側のマウス感度で微調整することをお勧めします。<br>
-設定例<br>
・タイトーマイルストーン版アルカノイドではデフォルトの「const int LOW_SENS_SPEED = 1;（1パルス＝1ピクセル移動） 」の状態でソフト側のマウス感度を6～7程度にするとちょうどいいかと思います<br>
・アルカノイド以外（キャメルトライ、クエスター等）では1パルス＝1ピクセルでは感度が低すぎるようなので2から3ピクセルもしくはHIGHモードでの使用をお勧めします<br>
<br>
-感度切り替えをトグルスイッチではなくボタンで行いたいときは以下のように書き換えます
```
// --- 追加する変数（loopの外で定義） ---
bool lastBtnState = HIGH; // 前回のボタンの状態を保存

void loop() {
  bool currentBtnState = digitalRead(SW_SENS);

  if (currentBtnState == LOW && lastBtnState == HIGH) {
    isHighSensMode = !isHighSensMode; 

    if (isHighSensMode) {
      RXLED1; 
    } else {
      RXLED0; 
    }
    
    delay(50); 
  }
  lastBtnState = currentBtnState; 
```
