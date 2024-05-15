var btsMapFormat = {
    name: "below the surface map format",
    extension: "txt",

    write: function (map, fileName) {
        for (var i = 0; i < map.layerCount; ++i) {
            var m = ''
            var layer = map.layerAt(i);
            if (layer.isTileLayer) {
                for (y = 0; y < layer.height; ++y) {
                    for (x = 0; x < layer.width; ++x){
                        var temp = layer.tileAt(x, y);
                        if (temp !== null) {
                            m += String(temp.id + temp.tileset.resolvedProperty('Offset'));
                        }
                        else {
                            m += String(0);
                        }
                        m += ' ';
                    }
                    m += "\n";
                }
            }
            var file = new TextFile(fileName.replace(/.txt/, '_' + String(i) + '.txt'), TextFile.WriteOnly);
            file.write(m);
            file.commit();
        }
    },
}

tiled.registerMapFormat("custom", btsMapFormat);