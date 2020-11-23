# insert-pubsub-mongo
MQTTブローカーから受信した位置情報のメッセージをMongoDBに登録します

## Description
 * MQTTブローカーからGNSSの位置情報を受信
 * 受信した位置情報をMongoDBへ登録

## Requirement
insert-pubsub-mongo requires the following to run:

  * [Node.js](https://nodejs.org/en/) 12.0+
  * [npm](https://www.npmjs.com/) (normally comes with Node.js)
  * [MongoDB](https://mongodb.github.io/node-mongodb-native/) 3.6+
  * [MQTT](https://www.npmjs.com/package/mqtt) 4.2+


## Install

```sh
$ npm install
```

## Usage
事前に以下サービスへの登録が必要となります
* (任意)[shifter.io](https://shiftr.io/)
  * 無料のMQTTブローカーです。すでに使用しているMQTTブローカーがある、自分で立てる場合は登録不要です
* (任意)[MongoDB Atlas](https://www.mongodb.com/cloud/atlas)
  * MongoDBのSaaSです。無料枠でも十分使えます。自分でMongoDBを立てる場合は登録不要です。

### MQTT トピックの作成
トピック名は下記とします
* ``` 
  [デバイスID]/location 
  ```
  サンプルではデバイスIDを「m5atom」としています。複数台使用する場合はデバイス毎に変更してください
  ここではデバイスIDは1台なのでconfig.jsonから取得していますが、複数台に対応する場合はDBなどで管理してください

### MongoDB DBとCollectionの作成
DBとCollection名は下記とします
* ```
  [DB].[Collection]
  devices.locations
  ```
### config.jsonの作成
起動するには下記の情報が必要となります
* MQTTの情報一式
* MongoDBの情報一式

上記情報を含んだ **config.json** を実行時の引数に渡してください<br>
config.json
```json
{
  "mqtt" : {
    "deviceId" : "m5atom",
    "url" : "XXXXXX",
    "port" : "1883 or 8883",
    "user" : "XXXXXX",
    "password" : "XXXXXX"
  },
  "mongodb" : {
    "url" : "XXXXXX",
    "db" : "devices",
    "collection" : "locations"
  }
}
```
## Run
```sh
$ node ./index.js ./config.json
```