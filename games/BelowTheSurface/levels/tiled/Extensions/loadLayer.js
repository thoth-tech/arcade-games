function convertID(ID) {
    tiled.log(ID);
    switch (true) {
        case ID <= 0:
            return null;
        case ID <= 99:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'solid_blocks.tsx')).tile(ID - 1);
        case ID <= 199:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'half_blocks_up.tsx')).tile(ID - 101);
        case ID <= 299:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'half_blocks_down.tsx')).tile(ID - 201);
        case ID <= 399:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'ladder.tsx')).tile(ID - 301);
        case ID <= 499:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'pipes_64.tsx')).tile(ID - 401);
        case ID <= 599:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'water_blocks.tsx')).tile(ID - 501);
        case ID <= 699:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'toxic_blocks.tsx')).tile(ID - 601);
        case ID <= 799:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'holdable_pipes.tsx')).tile(ID - 701);
        case ID <= 899:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'holdable_pipes_receiver.tsx')).tile(ID - 801);
        case ID <= 999:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'turn_pipes.tsx')).tile(ID - 901);
        case ID <= 1099:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'turn_pipes_receiver.tsx')).tile(ID - 1001);
        case ID <= 1199:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'decorative_blocks.tsx')).tile(ID - 1101);
        case ID <= 1299:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'Player.tsx')).tile(ID - 1201);
        case ID <= 1399:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'door.tsx')).tile(ID - 1301);
        case ID <= 1499:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'enemies.tsx')).tile(ID - 1401);
        case ID <= 1599:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'multi_pipes.tsx')).tile(ID - 1501);
        case ID <= 1699:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'multi_pipes_receiver.tsx')).tile(ID - 1601);
        case ID <= 1799:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'collectables.tsx')).tile(ID - 1701);
        case ID <= 1899:
            return tiled.open(tiled.projectFilePath.replace(/level_editor.tiled-project/, 'enemy_collider.tsx')).tile(ID - 1801);
        default:
            return null;
    }
}

var action = tiled.registerAction("BTSLoadLayer", function (action) {
    importLayer(tiled.promptOpenFile(tiled.projectFilePath.replace(/level_editor.tiled-project/, ''), "Text files (*.txt)"));
})

function importLayer(fileName) {
    var file = new TextFile(fileName, TextFile.ReadOnly);
    var map = tiled.activeAsset;
    //if (tiled.activeAsset.isTileMap) {
    //    var map = tiled.activeAsset;
    //}
    //else {
    //    return null;
    //    var map = new TileMap();
    //    map.setTileSize(64, 64);
    //    map.setSize(0, 0);
    //}
    var count = 0;
    var layer = new TileLayer();
    var layerEdit = layer.edit();
    while (!file.atEof) {
        var line = file.readLine();
        line = line.split(/ |\t/);
        if (line.length > 0) {
            map.width = Math.max(map.width, line.length);
            for (var x = 0; x < line.length; x++) {
                layerEdit.setTile(x, count, convertID(Number(line[x])));
            }
        }
        count++;
        map.height = Math.max(count, map.height);
    }
    layerEdit.apply();
    layer.name = "BTS Layer";
    map.addLayer(layer);
    file.close();
    tiled.activeAsset = map;
    return;
}

action.text = "BTS level load";
action.checkable = false;
action.shortcut = "Ctrl+K";

tiled.extendMenu("Map", [
    { action: "BTSLoadLayer"},
    { separator: true }
]);