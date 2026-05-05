# Paddle2Mouse-For-ArcadeArchives

USB HIDとして認識され、Nintendo Switch/Switch2でマウスとして動作します。（PS4/5では動作確認できていませんが同じ要領で動くと思います

・Arduino Pro Micro (5V/16MHz版)もしくは互換ボード
・産業用ロータリーエンコーダ (オープンコレクタ出力型、A/B相出力)
・プッシュボタン×2 (左右クリック用)
・トグルスイッチもしくはプッシュボタン×1 (モード切替用)
・ノブ（金属製のものを推奨）
・外装（お好みで）
その他、必要に応じて接続ケーブルやピンヘッダなど

【エンコーダ信号】
A相:Pin 2
B相:Pin 3
【操作入力】（各ボタン・スイッチの片方は以下のピン、もう片方はすべてGNDへ）
左ボタン:Pin 4
右ボタン:Pin 5
モード切替スイッチ:Pin 6
【電源系】
エンコーダの電源入力はArduinoのVCCピンへ、各パーツのGNDはGNDピンへ適当にまとめておく

■設定項目（カスタマイズ）
プログラム冒頭の「設定項目」を書き換えることで、各モード時の感度を個別に調整が可能です。
```
// --- モード1：低感度（スイッチOFF時） ---
const int LOW_SENS_SPEED = 1; 

// --- モード2：高感度（スイッチON時） ---
const int HIGH_SENS_SPEED = 3;
```
LOWは1～2、HIGHは2～3に設定し、ソフト側のマウス感度で微調整することをお勧めします。
-設定例
・タイトーマイルストーン版アルカノイドではデフォルトの「const int LOW_SENS_SPEED = 1;（1パルス＝1ピクセル移動） 」の状態でソフト側のマウス感度を6～7程度にするとちょうどいいかと思います
・アルカノイド以外（キャメルトライ、クエスター等）では1パルス＝1ピクセルでは感度が低すぎるようなので、HIGHモードでの使用をお勧めします

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
