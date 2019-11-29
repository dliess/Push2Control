
function createSelfDestroyingObject(qmlFile, parent, params) {
    var component = Qt.createComponent(qmlFile);
    var object = component.createObject(parent, params);

    if (object == null) {
        // Error Handling
        console.log("Error creating object");
    }
}