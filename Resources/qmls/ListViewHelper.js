function getListViewStartIndex(listView) {
    var idx = listView.indexAt(listView.contentX,1)
    if(-1 == idx)
    {
       idx = listView.count - 8
    }
    return idx
}

function incrementListViewStartIndex(listView, increment) {
    var idx = getListViewStartIndex(listView)
    idx += increment
    idx = Math.max(0, idx)
    idx = Math.min(listView.count - 8, idx)
    listView.positionViewAtIndex(idx, ListView.Beginning);
}