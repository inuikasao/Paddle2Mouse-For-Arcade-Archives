#include <Mouse.h>

//Paddle2MouseForAA ver1.2
// ======================================================================
// 【設定項目】ここを書き換えて、2つのモードの速さを調整してください
// ======================================================================

// --- モード1：低感度（スイッチOFF / LED消灯時） ---
// 1カウントで動くピクセル数です。基本は 1 でOKです。
const int LOW_SENS_SPEED = 1;

// --- モード2：高感度（スイッチON / LED点灯時） ---
// 1カウントで何ピクセル分ジャンプするかを指定します。
// ここを 2 や 4 にすると、パドルを少し回すだけで大きく動きます。
const int HIGH_SENS_SPEED = 3;

// ======================================================================
// 【ピン番号の設定】ボタンのピン配列を変更する際はconst int = n;を変更
// ======================================================================
const int pinA = 2;      // エンコーダ A相
const int pinB = 3;      // エンコーダ B相
const int BTN_LEFT = 4;  // 左ボタン
const int BTN_RIGHT = 5; // 右ボタン
const int SW_SENS = 6;   // 感度切替スイッチ
const int BTN_MIDDLE = 8; // ホイールクリックボタン（増設分・Pin 8）

// 中央ボタンの状態管理用変数
bool middleWasPressed = false; 
// ======================================================================

volatile long encoderPos = 0;
long lastEncoderPos = 0;
int lastStateA;
bool isHighSensMode = false;

void setup() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(SW_SENS, INPUT_PULLUP); 
  pinMode(BTN_MIDDLE, INPUT_PULLUP); // 中央クリックボタン設定

  lastStateA = digitalRead(pinA);
  attachInterrupt(digitalPinToInterrupt(pinA), updateEncoder, CHANGE);
  Mouse.begin();

  // 初期化：RXLED0(消灯)
  RXLED0; 
  TXLED1; 
}

void loop() {
  // --- A. スイッチの状態を確認して感度モードとLEDを切り替える ---
  if (digitalRead(SW_SENS) == LOW) {
    // 【スイッチON：高感度モード】
    isHighSensMode = true;
    RXLED1; // LED点灯
  } else {
    // 【スイッチOFF：低感度モード】
    isHighSensMode = false;
    RXLED0; // LED消灯
  }

  // --- B. エンコーダの回転処理 (感度設定を反映) ---
  if (encoderPos != lastEncoderPos) {
    long diff = encoderPos - lastEncoderPos;
    lastEncoderPos = encoderPos;

    int currentSpeed = isHighSensMode ? HIGH_SENS_SPEED : LOW_SENS_SPEED;
    
    // 1パルス × 設定されたスピード分だけマウスを動かす
    Mouse.move((int)diff * currentSpeed, 0, 0);
  }

  // --- C. 左右ボタン（クリック）処理 ---
  if (digitalRead(BTN_LEFT) == LOW) {
    if (!Mouse.isPressed(MOUSE_LEFT)) Mouse.press(MOUSE_LEFT);
  } else {
    if (Mouse.isPressed(MOUSE_LEFT)) Mouse.release(MOUSE_LEFT);
  }

  if (digitalRead(BTN_RIGHT) == LOW) {
    if (!Mouse.isPressed(MOUSE_RIGHT)) Mouse.press(MOUSE_RIGHT);
  } else {
    if (Mouse.isPressed(MOUSE_RIGHT)) Mouse.release(MOUSE_RIGHT);
  }

  // --- D. 中央ボタン（クレジット投入）処理：エッジ検出＆ロックアウト方式 ---
  bool currentMiddleState = (digitalRead(BTN_MIDDLE) == LOW);

  // ボタンが押された瞬間だけ実行
  if (currentMiddleState && !middleWasPressed) {
    Mouse.press(MOUSE_MIDDLE);    // 1. マウスの中央ボタンを押す
    delay(50);                   // 2. 確実に認識させるための待機
    Mouse.release(MOUSE_MIDDLE);  // 3. マウスの中央ボタンを離す
    
    middleWasPressed = true;     // 4. 押された状態を記録
    delay(200);                  // 5. 二重投入防止のガードタイム
  } 
  // ボタンが物理的に離されたら記録をリセット
  else if (!currentMiddleState) {
    middleWasPressed = false;
  }

  delay(1); 
}

// エンコーダ信号の読み取り処理（1パルスを確実に検知）
void updateEncoder() {
  int stateA = digitalRead(pinA);
  if (stateA != lastStateA) {
    if (digitalRead(pinB) != stateA) {
      encoderPos++;
    } else {
      encoderPos--;
    }
  }
  lastStateA = stateA;
}
