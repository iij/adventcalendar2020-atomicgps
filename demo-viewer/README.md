# demo-viewer

## Description
 * AtomicGPSの現在位置をリアルタイム表示
 * 過去の履歴情報をヒートマップで表示
 * デモ向けデータを表示

## Requirement
demo-viewer requires the following to run:

  * [leaflet](https://leafletjs.com/) 1.2+
  * [Leaflet.heat Plugin](https://github.com/Leaflet/Leaflet.heat)
  * [leaflet-hash Plugin](https://github.com/mlevans/leaflet-hash)
  * MQTT

## Usage
### vector-tile-mongoのURL
on('tileload')内のurlを自身の環境に修正してください
```javascript
    L.gridLayer.GeoJson({
      maxZoom: 22,
      minZoom: 10,
      maxNativeZoom: 16
    }).on('tileload', function (event) {
      // タイルロードのタイミングでGeoJsonベクトルタイルを取得してヒートマップを生成
      // vector-tile-mongoのURL
      let url = "https://tamago.mydns.jp/api/geojson/{z}/{x}/{y}"
```
デフォルトではデモ向けベクトルタイルに接続しています
### MQTTブローカー
L.gridLayer.Mqtt({}).on("add")内のmqttClientを自身の環境に修正してください
```javascript
    L.gridLayer.Mqtt({
    }).on("add", function (event) {
      // demo用ユーザー
      let mqttClient = mqtt.connect('mqtts://21e02fc9:89e01e205d056a34@broker.shiftr.io')
```
デフォルトではデモ向けのMQTTブローカーに接続しています

## Run
index.htmlをWEBブラウザで開いてください