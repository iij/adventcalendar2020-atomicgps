'use strict'

if (process.argv.length !== 3) {
  console.error("アプリケーション設定ファイルが必要です")
  return
}

const fs = require('fs')
const mqtt = require('mqtt')
const mongodb = require('mongodb')

const appConfg = JSON.parse(fs.readFileSync(process.argv[2], 'utf8'))


function mqttInit() {
  let client = mqtt.connect(appConfg.mqtt.url, {
    clientId: appConfg.mqtt.deviceId + "-subscriber",
    username: appConfg.mqtt.user,
    password: appConfg.mqtt.password,
    port: appConfg.mqtt.port
  })
  return client
}


async function insertDocument(doc) {
  let client
  try {
    client = await mongodb.MongoClient.connect(appConfg.mongodb.url, {
      useNewUrlParser: true,
      useUnifiedTopology: true
    })

    const db = client.db(appConfg.mongodb.db)
    const res = await db
      .collection(appConfg.mongodb.collection)
      .insertOne(doc)

    return res
  } catch (error) {
    console.log(error)
  } finally {
    client.close()
  }
}


function buildMongoDocument(payload) {
  let now = new Date()

  let payloadJson = JSON.parse(payload)
  let mongoDocument = {
    deviceId: appConfg.mqtt.deviceId,
    geometry: { type: "Point", coordinates: [parseFloat(payloadJson.lon), parseFloat(payloadJson.lat), parseFloat(payloadJson.alt)] },
    gpstime: payloadJson.gpstime,
    uploadtime: now.toISOString()
  }
  return mongoDocument
}



function main() {
  const mqttClient = mqttInit()

  mqttClient.on('connect', function () {
    mqttClient.subscribe('/' + appConfg.mqtt.deviceId + '/location', { qos: 0 })
  })


  mqttClient.on('message', function (topic, message) {
    //console.log(message.toString())
    let doc = buildMongoDocument(message)

    insertDocument(doc)

  })
}

main()