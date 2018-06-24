import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3
import Ubuntu.Content 1.3
import ubtd 1.0

MainView {
    applicationName: "ubtd.fourloop2002"


    width: units.gu(100)
    height: units.gu(75)

    PageStack {
        id: pageStack
        Component.onCompleted: push(mainPage)
    }

    Page {
        id: mainPage
        header: PageHeader {
            title: i18n.tr("Bluetooth file transfer....")
            leadingActionBar.actions: []
        }

        ColumnLayout {
            spacing: units.gu(1)
            anchors {
                fill: parent
                topMargin: mainPage.header.height + units.gu(1)
            }

            Column {
                Layout.fillWidth: true
                Layout.preferredHeight: childrenRect.height
                spacing: units.gu(1)

                ColumnLayout {
                    width: parent.width - units.gu(4)
                    anchors.horizontalCenter: parent.horizontalCenter

                    Label {
                        id: label
                        text: i18n.tr("Your Ubuntu Touch device is now ready to receive files via Bluetooth...")
                        fontSize: "large"
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                    }
                    Label {
                        text: i18n.tr("Send a file via Bluetooth to your Ubuntu Touch phone. It will appear in the list below.")
                        fontSize: "x-small"
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                    }
                }
                ThinDivider {

                }
                Label {
                    text: i18n.tr("Transfers:")
                    anchors { left: parent.left; right: parent.right; margins: units.gu(2) }
                }
            }

            ListView {
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true

                model: obexd

                delegate: ListItem {
                    id: listItem
                    height: units.gu(10)
                    onClicked: {
                        pageStack.push(pickerPageComponent, {contentType: listItem.contentType, filePath: model.filePath + "/" + model.filename})
                    }

                    leadingActions: ListItemActions {
                        actions: [
                            Action {
                                iconName: "delete"
                                onTriggered: obexd.deleteFile(index)
                            }
                        ]
                    }

                    property var contentType: null

                    RowLayout {
                        anchors { fill: parent; leftMargin: units.gu(2); rightMargin: units.gu(2); topMargin: units.gu(1); bottomMargin: units.gu(1) }


                        Item {
                            Layout.fillHeight: true
                            Layout.preferredWidth: height

                            UbuntuShape {
                                anchors.fill: parent
                                aspect: UbuntuShape.DropShadow
                                sourceFillMode: Image.PreserveAspectCrop
                                source: Image {
                                    id: transferredImage
                                    source: model.status === Transfer.StatusComplete ? "file://" + filePath + "/" + filename : ""
                                }
                            }

                            Icon {
                                anchors.fill: parent
                                visible: status !== Transfer.StatusComplete || transferredImage.status === Image.Error
                                name: {
                                    var extension = filename.split(".").pop()
                                    switch(extension) {
                                    case "pdf":
                                        listItem.contentType = ContentType.Documents;
                                        return "application-pdf-symbolic";
                                    case "tar":
                                    case "gz":
                                    case "gzip":
                                    case "zip":
                                    case "xz":
                                        listItem.contentType = ContentType.Unknown;
                                        return "application-x-archive-symbolic";
                                    case "mp3":
                                    case "ogg":
                                    case "wav":
                                    case "flac":
                                        listItem.contentType = ContentType.Music;
                                        return "audio-x-generic-symbolic";
                                    case "jpg":
                                    case "gif":
                                    case "jpeg":
                                    case "png":
                                    case "webp":
                                        listItem.contentType = ContentType.Pictures;
                                        return "image-x-generic-symbolic";
                                    case "click":
                                    case "deb":
                                        listItem.contentType = ContentType.Unknown;
                                        return "package-x-generic-symbolic";
                                    case "txt":
                                        listItem.contentType = ContentType.Text;
                                        return "text-generic-symbolic";
                                    case "mp4":
                                    case "mkv":
                                    case "avi":
                                    case "mpeg":
                                    case "mpg":
                                        listItem.contentType = ContentType.Videos;
                                        return "video-x-generic-symbolic";
                                    default:
                                        listItem.contentType = ContentType.Unknown;
                                        return "empty-symbolic";
                                    }
                                }
                            }
                        }

                        ColumnLayout {
                            Label {
                                Layout.fillWidth: true
                                text: filename
                            }
                            UbuntuShape {
                                Layout.fillWidth: true
                                Layout.preferredHeight: units.dp(5)
                                visible: status == Transfer.StatusActive
                                UbuntuShape {
                                    anchors.fill: parent
                                    color: UbuntuColors.blue
                                    // trans : total = x : width
                                    anchors.rightMargin: parent.width - (transferred * parent.width / size)
                                }
                            }
                            Label {
                                text: {
                                    switch (status) {
                                    case Transfer.StatusQueued:
                                        return "Waiting...";
                                    case Transfer.StatusActive:
                                        return "Transferring...";
                                    case Transfer.StatusSuspended:
                                        return "Paused"
                                    case Transfer.StatusComplete:
                                        return "Completed (" + Qt.formatDateTime(date) + ")";
                                    case Transfer.StatusError:
                                        return "Failed";
                                    }
                                    return "Uunknown";
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    ContentItem {
        id: exportItem
        name: i18n.tr("Attachment")
    }

    Component {
        id: pickerPageComponent

        Page {
            id: pickerPage
            property alias contentType: picker.contentType
            property string filePath

            head {
                visible: false
                locked: true
            }

            ContentPeerPicker {
                id: picker
                anchors.fill: parent
                handler: ContentHandler.Destination

                onPeerSelected: {
                     var transfer = peer.request();
                     if (transfer.state === ContentTransfer.InProgress) {
                         var items = new Array()
                         var path = "file://" + pickerPage.filePath;
                         exportItem.url = path
                         items.push(exportItem);
                         transfer.items = items;
                         transfer.stateChanged.connect(function() {
                             if (transfer.state == ContentTransfer.Finalized && pageStack.depth > 1) {
                                 pageStack.pop();
                             }
                         })
                         transfer.state = ContentTransfer.Charged;
                     }
                 }
                 onCancelPressed: pageStack.pop();
            }
        }
    }

}

