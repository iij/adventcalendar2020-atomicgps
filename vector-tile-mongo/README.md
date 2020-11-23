# vector-tile-mongo
MongoDBに格納されたGeoJSONをベクトルタイルで返却します

## Description
 * 指定されたタイル座標からベクトルタイルを返却

## Requirement
vector-tile-mongo requires the following to run:

  * [Node.js](https://nodejs.org/en/) 12.0+
  * [npm](https://www.npmjs.com/) (normally comes with Node.js)
  * [MongoDB](https://mongodb.github.io/node-mongodb-native/) 3.6+
  * [Express](https://expressjs.com/ja/)


## Install

```sh
$ npm install
```

## Usage
事前に以下サービスへの登録が必要となります
* (任意)[MongoDB Atlas](https://www.mongodb.com/cloud/atlas)
  * MongoDBのSaaSです。無料枠でも十分使えます。自分でMongoDBを立てる場合は登録不要です。

### config.jsonの作成
起動するには下記の情報が必要となります
* MongoDBの情報一式

上記情報を含んだ **config.json** を実行時の引数に渡してください<br>
config.json
```json
{
    "api": {
        "port" : 3000
    },
    "mongodb" : {
        "url" : "XXXXXX",
        "db" : "devices",
        "collection" : "locations"
    }
}
```
### URL
```
http://{host}:{port}/api/geojson/{z}/{x}/{y}
```
* {host}：ホスト名
* {port}：ポート番号
* {x}：タイル座標のX値<br>
* {y}：タイル座標のY値<br>
* {z}：ズームレベル
  
タイル座標については国土地理院のHPを参照してください<br>
[ズームレベル・タイル座標](https://maps.gsi.go.jp/development/siyou.html#siyou-zm)
## Run
```sh
$ node ./index.js ./config.json
```