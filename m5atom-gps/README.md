# m5atom-gps

M5AtomでGPSキットからGNSS位置情報を受信しMQTTブローカーにメッセージを送信します

## Description
 * GNSSから位置情報を受信
 * Wi-Fi経由でMQTTブローカーにメッセージ送信
 * 設定項目をSDカードから読み取り

## Requirement
m5atom-gps requires the following to run:

  * [Arduino IDE](https://www.arduino.cc/en/software)
  * [TinyGPS++](http://arduiniana.org/libraries/tinygpsplus/)
  * [MQTT](https://github.com/256dpi/arduino-mqtt)
  * [ArduinoJson](https://arduinojson.org/)
  * [WiFi](https://www.arduino.cc/en/Reference/WiFi)
  * [SD](https://www.arduino.cc/en/reference/SD)


## Install
* Arduino IDEのインストール
* 参照先にしたがってライブラリのインストール
  * [TinyGPS++](http://arduiniana.org/libraries/tinygpsplus/)
  * [MQTT](https://github.com/256dpi/arduino-mqtt)
  * [ArduinoJson](https://arduinojson.org/)
  * [WiFi](https://www.arduino.cc/en/Reference/WiFi)
  * [SD](https://www.arduino.cc/en/reference/SD)
* 「スケッチ」->「検証・コンパイル」でエラーが出ないことを確認
  * 「#pragma message～」関係のメッセージが出るが下記メッセージが表示されればOKです
  * ```
    最大○○○○バイトのフラッシュメモリのうち、スケッチが○○○○バイト（○○%）を使っています。
    最大○○○○バイトのRAMのうち、グローバル変数が○○○○バイト（○○%）を使っていて、ローカル変数で○○○○バイト使うことができます。
    ```
  * ヘッダーが見つからないエラーが発生した場合はライブラリがインストールされていません。
    * 「ツール」->「ライブラリの管理…」から検索してインストールしてください 
* 「スケッチ」->「マイコンボードに書き込む」を実行
  
## Usage
事前に以下サービスへの登録が必要となります
* (任意)[shifter.io](https://shiftr.io/)
  * 無料のMQTTブローカーです。すでに使用しているMQTTブローカーがある、自分で立てる場合は登録不要です

### MQTT トピックの作成
トピック名は下記とします
* ``` 
  [デバイスID]/location 
  ```
  サンプルではデバイスIDを「m5atom」としています。複数台使用する場合はデバイス毎に変更してください
### config.jsonの作成
起動するには下記の情報が必要となります
* Wi-FiのSSIDとパスワード
* MQTTの情報一式

上記情報を含んだ **config.json** をSDカードのルートに配置してください<br>
config.json
```json
{
  "wlan": {
    "ssid": "XXXXXX",
    "password": "XXXXXX"
  },
  "mqtt": {
    "deviceId": "m5atom",
    "server": "XXXXXX",
    "port": "1883 or 8888",
    "user": "XXXXXX",
    "password": "XXXXXX",
    "location": {
      "topic": "location"
    }
  },
  "interval": "30000"
}
```
intervalは送信間隔(ms)です<br>

## Run
SDカードをAtomicGPSへ刺してGNSSが受信出来る場所で電源ONしてください<br>
数分するとMQTTブローカーに位置情報が送信されます

