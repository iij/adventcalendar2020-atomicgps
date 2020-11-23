'use strict'

if (process.argv.length !== 3) {
    console.error("アプリケーション設定ファイルが必要です")
    return
}

let self = this
const fs = require('fs')
const express = require('express')
const mongodb = require('mongodb')

const appConfig = JSON.parse(fs.readFileSync(process.argv[2], 'utf8'))
const app = express()

self.db = null

app.get('/', (req, res) => res.send('OK'))

app.get('/api/geojson/:z/:x/:y/?', async (req, res) => {
    //console.log(req.params)

    res.header("Access-Control-Allow-Origin", "*")
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept")

    let response = await getVectorTile(req.params.x, req.params.y, req.params.z, res)
    res.send(response)
})

const getVectorTile = async (x, y, z) => {

    let minmax = tile2latlon(x, y, z)
    let result = []
    try {
        result = await self.db.collection(appConfig.mongodb.collection)
            .find({
                geometry:
                {
                    $geoWithin:
                    {
                        $geometry:
                        {
                            type: "Polygon",
                            coordinates: [[
                                [minmax.min.lon, minmax.min.lat],
                                [minmax.max.lon, minmax.min.lat],
                                [minmax.max.lon, minmax.max.lat],
                                [minmax.min.lon, minmax.max.lat],
                                [minmax.min.lon, minmax.min.lat]
                            ]]
                        }
                    }
                }
            }).map((doc) => {
                return {
                    type: "Feature",
                    geometry: doc.geometry,
                    properties: { _id: doc._id }
                }
            }).toArray()
        //        console.log(result)
    } catch (error) {
        console.log(error)
    }

    return { type: "FeatureCollection", features: result }

}



function tile2latlon(x, y, z) {

    const mod = (a, b) => {
        return a * b < 0 ? a % b + b : a % b
    }

    const x2lon = (x, z) => {

        let lon = mod((x / Math.pow(2, z) * 360), 360) - 180
        return (lon != (-180) ? lon : 180)
    }

    const y2lat = (y, z) => {
        let n = Math.PI - 2 * Math.PI * y / Math.pow(2, z)
        return 180 / Math.PI * Math.atan(0.5 * (Math.exp(n) - Math.exp(-n)))
    }

    let minlon = x2lon(x, z)
    let maxlon = x2lon(parseInt(x, 10) + 1, z)

    let minlat = y2lat(parseInt(y, 10) + 1, z)
    let maxlat = y2lat(parseInt(y, 10), z)

    //console.log([minlon,minlat,maxlon,maxlat])
    return { min: { lat: minlat, lon: minlon }, max: { lat: maxlat, lon: maxlon } }

}



async function main() {
    const url = appConfig.mongodb.url

    mongodb.MongoClient.connect(url, {
        useNewUrlParser: true,
        useUnifiedTopology: true
    },
        (err, client) => {
            if (err) {
                console.log(err)
                return
            }
            self.db = client.db(appConfig.mongodb.db)
            app.listen(appConfig.api.port, () => console.log('Listening on port ' + appConfig.api.port))
        })

}

main()
