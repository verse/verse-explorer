#include "verse_client.h"
#include "verse_model.h"
#include "login_dialog.h"
#include "nodepermdialog.h"
#include "nodeownerdialog.h"
#include "taggroupdialog.h"
#include "tagdialog.h"
#include "tagvaluedialog.h"
#include "fpsdialog.h"
#include "ui_taggroupdialog.h"
#include "ui_verseclient.h"
#include "ui_logindialog.h"
#include "ui_tagdialog.h"
#include "nodedialog.h"
#include "verse_layer.h"
#include "verse_taggroup.h"
#include "verse_tag.h"

#include <QTimer>

#include <verse.h>

#include <iostream>
#include <unistd.h>


// Constructor
VerseClient::VerseClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VerseClient)
{
    this->timer = new QTimer(this);

    ui->setupUi(this);

    createStatusBar();

    this->verse_model = NULL;

    this->states = STATE_DISCONNECTED;

    this->node_auto_subscribe = false;

    this->node_auto_expand = false;

    connect(this->timer, SIGNAL(timeout()), this, SLOT(callUpdate()));

    this->fps = 60;
}


// Destructor
VerseClient::~VerseClient()
{
    qDeleteAll(this->users);
    this->users.clear();

    delete this->timer;
    delete this->ui;
}


// Quit application
void VerseClient::on_actionQuit_triggered()
{
    if(this->states == STATE_CONNECTED) {
        vrs_send_connect_terminate(this->session_id);
        // TODO: wait until server acknowledge disconnecting or wait some timeout
        sleep(1);
    }
    this->close();
}

void VerseClient::updateActionsAvailibility(void)
{
    if(this->states == STATE_CONNECTED) {

        if(ui->actionTesting_mode->isChecked() == true) {
            ui->actionCreate->setEnabled(true);
            ui->actionDestroy->setEnabled(true);
            ui->actionSubscribe->setEnabled(true);
            ui->actionUnSubscribe->setEnabled(true);
            ui->actionPermissions->setEnabled(true);
            ui->actionOwner->setEnabled(true);
            ui->actionLock->setEnabled(true);
            ui->actionUnLock->setEnabled(true);
            ui->actionAuto_subscribe->setEnabled(true);
            ui->actionExpand_all->setEnabled(true);
            ui->actionCreate_TagGroup->setEnabled(true);
            ui->actionDestroy_TagGroup->setEnabled(true);
            ui->actionSubscribe_TagGroup->setEnabled(true);
            ui->actionUnSubscribe_TagGroup->setEnabled(true);
            ui->actionCreate_Tag->setEnabled(true);
            ui->actionDestroy_Tag->setEnabled(true);
            ui->actionFPS->setEnabled(true);
            ui->actionChange_Value->setEnabled(true);
            return;
        }

        VerseNode *node = NULL;
        QModelIndexList index_list = ui->nodeTreeView->selectionModel()->selectedIndexes();
        int row = -1;
        bool is_my_node_selected = false;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                node = static_cast<VerseNode*>(index.internalPointer());
                if(node->getOwner() == this->user_id) {
                    is_my_node_selected = true;
                    break;
                }
                row = index.row();
            }
        }

        if(node != NULL) {
            if(node->getSubscribed()==true) {
                ui->actionSubscribe->setEnabled(false);
                if(node_auto_subscribe == false) {
                    ui->actionUnSubscribe->setEnabled(true);
                } else {
                    ui->actionUnSubscribe->setEnabled(false);
                }

                // Client should try to lock node only in situation, when
                // client is subscribed to the node
                if(node->can_write == true) {
                    if(node->getLocker() == (uint32_t)-1) {
                        // Node is unlocked
                        ui->actionLock->setEnabled(true);
                        ui->actionUnLock->setEnabled(false);
                    } else if(node->getLocker()==this->avatar_id) {
                        // Node is locked by this client
                        ui->actionLock->setEnabled(false);
                        ui->actionUnLock->setEnabled(true);
                    } else {
                        // Node is locked by some other client
                        ui->actionLock->setEnabled(false);
                        ui->actionUnLock->setEnabled(false);
                    }
                } else {
                    // Client can't write to this node
                    ui->actionLock->setEnabled(false);
                    ui->actionUnLock->setEnabled(false);
                }

            } else {
                ui->actionSubscribe->setEnabled(true);
                ui->actionUnSubscribe->setEnabled(false);

                // Do not try to lock node, when client is not subscribed to the node
                ui->actionLock->setEnabled(false);
                ui->actionUnLock->setEnabled(false);
            }

            // Can client write to this node?
            if(node->can_write == true) {
                ui->actionCreate_TagGroup->setEnabled(true);
            } else {
                ui->actionCreate_TagGroup->setEnabled(false);
            }

            // Get selected item in TreeView of data
            VerseData *data = NULL;
            index_list = ui->dataTreeView->selectionModel()->selectedIndexes();
            row = -1;
            foreach(QModelIndex index, index_list) {
                if (index.row()!=row && index.column()==0) {
                    data = static_cast<VerseData*>(index.internalPointer());
                    row = index.row();
                    break;
                }
            }

            // Was anything selected?
            if(data != NULL) {

                // Was TagGroup selected?
                if(data->type == VerseData::VERSE_TAG_GROUP) {
                    VerseTagGroup *tg = (VerseTagGroup*)data;

                    if(node->can_write == true) {
                        ui->actionDestroy_TagGroup->setEnabled(true);
                        ui->actionCreate_Tag->setEnabled(true);
                    } else {
                        ui->actionDestroy_TagGroup->setEnabled(false);
                        ui->actionCreate_Tag->setEnabled(false);
                    }

                    ui->actionDestroy_Tag->setEnabled(false);

                    if(tg->isSubscribed() == false) {
                        ui->actionSubscribe_TagGroup->setEnabled(true);
                        ui->actionUnSubscribe_TagGroup->setEnabled(false);
                    } else {
                        ui->actionSubscribe_TagGroup->setEnabled(false);
                        ui->actionUnSubscribe_TagGroup->setEnabled(true);
                    }
                } else if(data->type == VerseData::VERSE_TAG) {
                    if(node->can_write == true) {
                        ui->actionCreate_Tag->setEnabled(true);
                        ui->actionDestroy_Tag->setEnabled(true);
                        ui->actionChange_Value->setEnabled(true);
                    } else {
                        ui->actionCreate_Tag->setEnabled(false);
                        ui->actionDestroy_Tag->setEnabled(false);
                        ui->actionChange_Value->setEnabled(false);
                    }
                } else {
                    ui->actionDestroy_TagGroup->setEnabled(false);
                    ui->actionSubscribe_TagGroup->setEnabled(false);
                    ui->actionUnSubscribe_TagGroup->setEnabled(false);
                    ui->actionCreate_Tag->setEnabled(false);
                    ui->actionDestroy_Tag->setEnabled(false);
                    ui->actionChange_Value->setEnabled(false);
                }
            } else {
                ui->actionDestroy_TagGroup->setEnabled(false);
                ui->actionSubscribe_TagGroup->setEnabled(false);
                ui->actionUnSubscribe_TagGroup->setEnabled(false);
                ui->actionCreate_Tag->setEnabled(false);
                ui->actionDestroy_Tag->setEnabled(false);
                ui->actionChange_Value->setEnabled(false);
            }
        }

        if(is_my_node_selected == true) {
            ui->actionOwner->setEnabled(true);
            ui->actionPermissions->setEnabled(true);
            if(node->getChildNodes()->empty()==true) {
                ui->actionDestroy->setEnabled(true);
            } else {
                ui->actionDestroy->setEnabled(false);
            }
        } else {
            ui->actionOwner->setEnabled(false);
            ui->actionPermissions->setEnabled(false);
            ui->actionDestroy->setEnabled(false);
        }
    }
}

void VerseClient::setFPS(float _fps)
{
    this->fps = _fps;

    this->timer->start((int)1000/_fps);
}

void VerseClient::selectionChangedSlot(const QItemSelection &/*newSelection*/, const QItemSelection &/*oldSelection*/)
{
    if(this->states == STATE_CONNECTED) {
        QModelIndexList index_list = ui->nodeTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseNode *node = static_cast<VerseNode*>(index.internalPointer());
                ui->dataTreeView->setModel(node->getDataModel());
                break;
                row = index.row();
            }
        }

        ui->dataTableView->setModel(NULL);

        // Selection changes of data TreeView shall trigger a slot
        QItemSelectionModel *dataSelectionModel = ui->dataTreeView->selectionModel();
        connect(dataSelectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
                this, SLOT(dataSelectionChangedSlot(const QItemSelection &, const QItemSelection &)));
        ui->dataTreeView->expandAll();

        this->updateActionsAvailibility();
    }
}

void VerseClient::dataSelectionChangedSlot(const QItemSelection &/*newSelection*/, const QItemSelection &/*oldSelection*/)
{
    if(this->states == STATE_CONNECTED) {
        QModelIndexList index_list = ui->dataTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseData *data = static_cast<VerseData*>(index.internalPointer());

                if(data->getType() == VerseData::VERSE_TAG) {
                    VerseTag *tag = (VerseTag*)data;
                    ui->dataTableView->setModel(tag->getDataModel());
                } else {
                    ui->dataTableView->setModel(NULL);
                }
                break;
                row = index.row();
            }
        }

        this->updateActionsAvailibility();
    }
}

// Initial status in status bar
void VerseClient::createStatusBar()
{
    statusBar()->showMessage(tr("Disconnected"), 0);
}


// This method is called in never ending loop
void VerseClient::callUpdate()
{
    vrs_callback_update(session_id);
}


// This method sends connect request to the verse server
int VerseClient::connectToServer(const QString &hostname)
{
    QByteArray ba = hostname.toLocal8Bit();
    int ret;

    ret = vrs_send_connect_request(ba.data(), "12345", 0, &session_id);
    if(ret == VRS_SUCCESS) {
        this->timer->start((int)1000/this->fps);
        return 1;
    } else {
        return 0;
    }
}

void VerseClient::cbReceiveLayerCreate(const uint8_t session_id,
                                       const uint32_t node_id,
                                       const uint16_t parent_layer_id,
                                       const uint16_t layer_id,
                                       const uint8_t data_type,
                                       const uint8_t count,
                                       const uint16_t custom_type)
{
    VerseNode *node;

    std::cout << "session_id: " << session_id << " node_id: " << node_id << " parent_layer_id: " << parent_layer_id << " layer_id: " << layer_id << " data_type: " << data_type << " count: " << count << " custom_type:" << custom_type << std::endl;

    node = this->verse_model->getNode(node_id);
    if(node != NULL) {
        VerseLayer *parent_layer = node->getLayer(parent_layer_id);
        VerseLayer *layer = new VerseLayer(node, parent_layer, layer_id, data_type, count, custom_type);

        node->addLayer(layer);

        node->addData((VerseData*)layer);

        node->getDataModel()->update();
    }
}

void VerseClient::cbReceiveTagCreate(const uint8_t session_id,
                                     const uint32_t node_id,
                                     const uint16_t taggroup_id,
                                     const uint16_t tag_id,
                                     const uint8_t tag_type,
                                     const uint8_t count,
                                     const uint8_t client_type)
{
    VerseNode *node;

    std::cout << "session_id: " << session_id << " node_id: " << node_id << " taggroup_id: " << " tag_id: " << tag_id << " type: " << tag_type << taggroup_id << " client_type: " << client_type << std::endl;

    node = this->verse_model->getNode(node_id);
    if(node != NULL) {
        VerseTagGroup *tg;

        tg = node->getTagGroup(taggroup_id);

        if(tg != NULL) {
            VerseTag *tag = new VerseTag(node, tg, tag_id, tag_type, count, client_type);

            // TODO: fix this
            tag->model->setSessionID(this->getSessionID());

            tg->addTag(tag);

            tg->addData((VerseData*)tag);

            node->getDataModel()->update();
        }
    }
}

void VerseClient::cbReceiveTagDestroy(const uint8_t session_id,
                                      const uint32_t node_id,
                                      const uint16_t taggroup_id,
                                      const uint16_t tag_id)
{
    VerseNode *node;

    std::cout << "session_id: " << session_id << " node_id: " << node_id << " taggroup_id: " << " tag_id: " << tag_id << std::endl;

    node = this->verse_model->getNode(node_id);
    if(node != NULL) {
        VerseTagGroup *tg;

        tg = node->getTagGroup(taggroup_id);

        if(tg != NULL) {
            VerseTag *tag = tg->getTag(tag_id);

            if(tag != NULL) {
                QModelIndexList index_list = ui->dataTreeView->selectionModel()->selectedIndexes();
                int row = -1;

                foreach(QModelIndex index, index_list) {
                    if (index.row()!=row && index.column()==0) {
                        VerseData *data = static_cast<VerseData*>(index.internalPointer());
                        if (data == (VerseData*)tag) {
                            ui->dataTreeView->selectionModel()->clear();
                        }
                        row = index.row();
                    }
                }

                node->getDataModel()->destroyTag(tag);
            }
        }
    }
}

void VerseClient::cbReceiveTagSet(const uint8_t session_id,
                                  const uint32_t node_id,
                                  const uint16_t taggroup_id,
                                  const uint16_t tag_id,
                                  const uint8_t data_type,
                                  const uint8_t count,
                                  const void *value)
{
    VerseNode *node;

    std::cout << "session_id: " << session_id << " node_id: " << node_id << " taggroup_id: " << " tag_id: " << tag_id << std::endl;

    node = this->verse_model->getNode(node_id);
    if(node != NULL) {
        VerseTagGroup *tg;

        tg = node->getTagGroup(taggroup_id);

        if(tg != NULL) {
            VerseTag *tag = tg->getTag(tag_id);

            if(tag != NULL) {
                tag->setTagValue(value, data_type, count);
            }
        }
    }
}

void VerseClient::cbReceiveTaggroupCreate(const uint8_t session_id,
                                const uint32_t node_id,
                                const uint16_t taggroup_id,
                                const uint16_t taggroup_type)
{
    VerseNode *node;
    std::cout << "session_id: " << session_id << " node_id: " << node_id << " taggroup_id: " << taggroup_id << " type: " << taggroup_type << std::endl;
    node = this->verse_model->getNode(node_id);

    if(node != NULL) {
        VerseTagGroup *tg = new VerseTagGroup(node, taggroup_id, taggroup_type);

        node->addTagGroup(tg);

        node->addData((VerseData*)tg);

        node->getDataModel()->update();
    }
}

void VerseClient::cbReceiveTaggroupDestroy(const uint8_t session_id,
                                const uint32_t node_id,
                                const uint16_t taggroup_id)
{
    VerseNode *node;
    std::cout << "session_id: " << session_id << " node_id: " << node_id << " taggroup_id: " << taggroup_id << std::endl;
    node = this->verse_model->getNode(node_id);

    if(node != NULL) {
        VerseTagGroup *tg = node->getTagGroup(taggroup_id);

        QModelIndexList index_list = ui->dataTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseData *data = static_cast<VerseData*>(index.internalPointer());
                if (data == (VerseData*)tg) {
                    ui->dataTreeView->selectionModel()->clear();
                }
                row = index.row();
            }
        }

        node->getDataModel()->destroyTagGroup(tg);
    }
}

void VerseClient::cbReceiveNodeUnLock(const uint8_t session_id,
                       const uint32_t node_id,
                       const uint32_t avatar_id)
{
    VerseNode *node;
    std::cout << "session_id: " << session_id << " node_id: " << node_id << " avatar_id: " << avatar_id << std::endl;

    node = this->verse_model->getNode(node_id);

    if(node != NULL) {
        node->setLocker(-1);

        node->setCanWrite(this->getUserID(), this->getAvatarID());

        this->verse_model->update_view(node);

        this->updateActionsAvailibility();
    }
}


void VerseClient::cbReceiveNodeLock(const uint8_t session_id,
                       const uint32_t node_id,
                       const uint32_t avatar_id)
{
    VerseNode *node;
    std::cout << "session_id: " << session_id << " node_id: " << node_id << " avatar_id: " << avatar_id << std::endl;

    node = this->verse_model->getNode(node_id);

    if(node != NULL) {
        node->setLocker(avatar_id);

        node->setCanWrite(this->getUserID(), this->getAvatarID());

        this->verse_model->update_view(node);

        this->updateActionsAvailibility();
    }
}

void VerseClient::cbReceiveNodeOwner(const uint8_t session_id,
                                     const uint32_t node_id,
                                     const uint16_t user_id)
{
    VerseNode *node;
    std::cout << "session_id: " << session_id << " node_id: " << node_id << " user_id: " << user_id << std::endl;

    node = this->verse_model->getNode(node_id);

    if(node != NULL) {
        node->setOwner(user_id);

        node->setCanRead(this->getUserID());
        node->setCanWrite(this->getUserID(), this->getAvatarID());

        this->verse_model->update_view(node);

        this->updateActionsAvailibility();
    }
}


// This method set permission for the node
void VerseClient::cbReceiveNodePerm(const uint8_t session_id,
                                    const uint32_t node_id,
                                    const uint16_t user_id,
                                    const uint8_t perm)
{
    VerseNode *node;
    std::cout << "session_id: " << session_id << " node_id: " << node_id << " user_id: " << user_id << " permissions: " << perm << std::endl;

    node = this->verse_model->getNode(node_id);

    if(node != NULL) {
        node->setPermissions(user_id, perm);

        node->setCanRead(this->getUserID());
        node->setCanWrite(this->getUserID(), this->getAvatarID());

        this->verse_model->update_view(node);

        this->updateActionsAvailibility();
    }
}

// This method add new node to the model
void VerseClient::cbReceiveNodeCreate(const uint8_t session_id,
        const uint32_t node_id,
        const uint32_t parent_id,
        const uint16_t user_id,
        const uint16_t _type)
{
    VerseNode *parent_node, *child_node;

    std::cout << "session_id: " << session_id << " user_id: " << user_id << " node_id: " << node_id <<  " parent_id: " << parent_id << " type: " << _type << std::endl;

    parent_node = this->verse_model->getNode(parent_id);

    printf("client_type: %d\n", _type);

    // Create new child node
    child_node = new VerseNode(parent_node, node_id, user_id, _type);

    this->verse_model->addNode();

    // Expand all, when auto-expand is enabled
    if(this->node_auto_expand == true) {
        ui->nodeTreeView->expandAll();
    }

    // If user of this client own this node, then he/she can read and write to this node
    if(user_id == this->user_id) {
        child_node->can_read = true;
        child_node->can_write = true;
    }

    // Subscribe to all node, when autosubscription is enabled. Subscribe to the basic nodes in all cases
    if(this->node_auto_subscribe == true || (node_id==1 || node_id==2 || node_id==3 || node_id==this->avatar_id)) {
        vrs_send_node_subscribe(this->session_id, VRS_DEFAULT_PRIORITY, node_id, 0, 0);
        child_node->setSubscribed(true);
    }

    // Expand tree view, when avatar node is received
    if(node_id == this->avatar_id) {
        ui->nodeTreeView->expandAll();
        ui->nodeTreeView->resizeColumnToContents(0);
        ui->nodeTreeView->resizeColumnToContents(1);
        ui->nodeTreeView->resizeColumnToContents(2);
        ui->nodeTreeView->resizeColumnToContents(3);
        ui->nodeTreeView->resizeColumnToContents(4);
    }

    // When parent ID is 2, then this node represent user and add this user to
    // the list of known users
    if(parent_id == 2) {
        VerseUser *user = new VerseUser(node_id);
        this->users.append(user);
    }

    this->updateActionsAvailibility();
}

// This method delete node and it's child nodes from the model
void VerseClient::cbReceiveNodeDestroy(const uint8_t session_id,
        const uint32_t node_id)
{
    std::cout << "session_id: " << session_id << " node_id: " << node_id << std::endl;

    VerseNode *node = this->verse_model->getNode(node_id);

    qDebug("Del node, id: %d at %p\n", node_id, node);

    QModelIndexList index_list = ui->nodeTreeView->selectionModel()->selectedIndexes();
    int row = -1;

    foreach(QModelIndex index, index_list) {
        if (index.row()!=row && index.column()==0) {
            VerseNode *_node = static_cast<VerseNode*>(index.internalPointer());
            if (node == _node) {

                qDebug("Clear selection\n");

                ui->nodeTreeView->selectionModel()->clear();
                ui->nodeTreeView->clearSelection();
                ui->nodeTreeView->setCurrentIndex(QModelIndex());

                ui->dataTreeView->selectionModel()->clear();
                ui->dataTreeView->clearSelection();
                ui->dataTreeView->setCurrentIndex(QModelIndex());
                ui->dataTreeView->setModel(this->verse_model->getFakeRootNode()->getDataModel());

                break;
            }
            row = index.row();
        }
    }

    node->remAllDialogs();

    this->verse_model->delNode(node);

    this->updateActionsAvailibility();


}

// This method change parent of the node
void VerseClient::cbReceiveNodeLink(const uint8_t session_id,
        const uint32_t parent_node_id,
        const uint32_t child_node_id)
{
    std::cout << "session_id: " << session_id << " parent_node_id: " << parent_node_id  << " child_node_id: " << child_node_id << std::endl;

    VerseNode *parent_node = this->verse_model->getNode(parent_node_id);
    VerseNode *child_node = this->verse_model->getNode(child_node_id);

    if(parent_node != NULL) {
        this->verse_model->changeParent(child_node, parent_node);

        // Expand all, when auto-expand is enabled
        if(this->node_auto_expand == true) {
            ui->nodeTreeView->expandAll();
        }
    } else {
        // TODO: do something wise, when new parent is unknown

        QModelIndexList index_list = ui->nodeTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseNode *_node = static_cast<VerseNode*>(index.internalPointer());
                if (child_node == _node) {
                    ui->nodeTreeView->selectionModel()->clear();
                }
                row = index.row();
            }
        }

        this->verse_model->delNode(child_node);

        this->updateActionsAvailibility();
    }

    this->updateActionsAvailibility();
}

// This method is called, when client is connected to server
void VerseClient::cbReceiveConnectAccept(const uint8_t session_id,
                                         const uint16_t user_id,
                                         const uint32_t avatar_id)
{
    VerseNode *root_node;

    std::cout << "session_id: " << session_id << " user_id: " << user_id << " avatar_id: " << avatar_id << std::endl;

    ui->connectButton->setText("Disconnect");
    ui->connectButton->setEnabled(true);
    ui->nodeTreeView->setEnabled(true);
    ui->actionCreate->setEnabled(true);
    ui->actionDestroy->setEnabled(true);
    ui->actionSubscribe->setEnabled(true);
    ui->actionUnSubscribe->setEnabled(true);
    ui->actionPermissions->setEnabled(true);
    ui->actionOwner->setEnabled(true);
    ui->actionLock->setEnabled(true);
    ui->actionUnLock->setEnabled(true);
    ui->actionAuto_subscribe->setEnabled(true);
    ui->actionExpand_all->setEnabled(true);
    ui->actionAuto_expand->setEnabled(true);
    ui->actionFPS->setEnabled(true);

    ui->tabWidget->setTabText(0, this->username + "@" + ui->urlLineEdit->text());

    statusBar()->showMessage(tr("Connected"), 0);

    this->user_id = user_id;
    this->avatar_id = avatar_id;

    // Set model for node tree
    this->verse_model = new VerseNodeTreeModel(this);
    ui->nodeTreeView->setModel(verse_model);

    // Selection changes of node TreeView shall trigger a slot
    QItemSelectionModel *nodeSelectionModel = ui->nodeTreeView->selectionModel();
    connect(nodeSelectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this, SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));

    this->states = STATE_CONNECTED;

    VerseNode *fake_root_node = this->verse_model->getFakeRootNode();

    root_node = new VerseNode(fake_root_node, VRS_ROOT_NODE_ID, 100, 0);

    this->verse_model->addNode();

    // Set model for data
    ui->dataTreeView->setModel(root_node->getDataModel());

    vrs_send_node_subscribe(this->session_id, root_node->getPrio(), root_node->getID(), 0, 0);

    root_node->setSubscribed(true);
}


// This method is called, when client is disconnected from server
void VerseClient::cbReceiveConnectTerminate(const uint8_t session_id,
                                            const uint8_t error_num)
{
    // Selection changes of TreeView shall not trigger a slot
    QItemSelectionModel *selectionModel= ui->nodeTreeView->selectionModel();
    disconnect(selectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this, SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));

    // Selection changes of data TreeView shall trigger a slot
    QItemSelectionModel *dataSelectionModel = ui->dataTreeView->selectionModel();
    disconnect(dataSelectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this, SLOT(dataSelectionChangedSlot(const QItemSelection &, const QItemSelection &)));

    ui->nodeTreeView->setModel(NULL);
    if (this->verse_model != NULL) {
        delete this->verse_model;
        this->verse_model = NULL;
    }

    ui->dataTableView->setModel(NULL);

    // Delete list of users
    qDeleteAll(this->users);
    this->users.clear();

    std::cout << "session_id: " << session_id << " error: " << error_num << std::endl;

    ui->connectButton->setText("Connect");
    ui->connectButton->setEnabled(true);
    ui->urlLineEdit->setEnabled(true);
    ui->nodeTreeView->setEnabled(false);
    ui->actionCreate->setEnabled(false);
    ui->actionDestroy->setEnabled(false);
    ui->actionSubscribe->setEnabled(false);
    ui->actionUnSubscribe->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionPermissions->setEnabled(false);
    ui->actionOwner->setEnabled(false);
    ui->actionLock->setEnabled(false);
    ui->actionUnLock->setEnabled(false);
    ui->actionAuto_subscribe->setEnabled(false);
    ui->actionExpand_all->setEnabled(false);
    ui->actionAuto_expand->setEnabled(false);
    ui->actionFPS->setEnabled(false);

    ui->tabWidget->setTabText(0, ui->urlLineEdit->text());

    /* Print some explanation to status bar */
    switch(error_num) {
    case VRS_CONN_TERM_HOST_UNKNOWN:
        statusBar()->showMessage(tr("Host could not be found"));
        break;
    case VRS_CONN_TERM_HOST_DOWN:
        statusBar()->showMessage(tr("Host is not running"));
        break;
    case VRS_CONN_TERM_SERVER_DOWN:
        statusBar()->showMessage(tr("Server is not running"));
        break;
    case VRS_CONN_TERM_AUTH_FAILED:
        statusBar()->showMessage(tr("Authentication failed"));
        break;
    case VRS_CONN_TERM_TIMEOUT:
        statusBar()->showMessage(tr("Connection timed out"));
        break;
    case VRS_CONN_TERM_ERROR:
        statusBar()->showMessage(tr("Connection was broken"));
        break;
    case VRS_CONN_TERM_CLIENT:
    case VRS_CONN_TERM_SERVER:
        statusBar()->showMessage(tr("Connection closed"));
        break;
    default:
        statusBar()->showMessage(tr("Connection closed (error)"));
        break;
    }

    this->states = STATE_DISCONNECTED;

    this->timer->stop();
}


// This method is called, when client receives user authenticate request from server
void VerseClient::cbReceiveUserAuthenticate(const uint8_t session_id,
        const char *username,
        const uint8_t auth_methods_count,
        const uint8_t *methods)
{
    if(username != NULL) {
        this->username = QString(username);
    }

    LoginDialog *l = new LoginDialog(this, ui->urlLineEdit->text(), session_id, username, auth_methods_count, methods);

    // Debug prints
    std::cout << "session_id: " << (int)session_id << " username: " << username << " method count: " << (int)auth_methods_count;
    if(auth_methods_count > 0) {
        std::cout << " methods: ";
        for(int i=0; i < auth_methods_count; i++) {
            std::cout << methods[i] << ", ";
        }
    }
    std::cout << std::endl;

    l->show();
}


// This method is called, when client tries to connect or disconnect from verse server
void VerseClient::on_connectButton_clicked()
{
    this->on_actionConnectDisconnect();
}


// This method is called, when hostname in URL is changed
void VerseClient::on_urlLineEdit_editingFinished()
{
    ui->tabWidget->setTabText(0, ui->urlLineEdit->text());
}

// This method sends "node create" request to the server
void VerseClient::on_actionCreate_triggered()
{
    if(this->states == STATE_CONNECTED) {
        NodeDialog *nd = new NodeDialog(this, this->getSessionID());
        nd->show();
    }
}

// This method subcribe to selected node(s)
void VerseClient::on_actionSubscribe_triggered()
{
    if(this->states == STATE_CONNECTED) {
        QModelIndexList index_list = ui->nodeTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseNode *node = static_cast<VerseNode*>(index.internalPointer());
                this->verse_model->subscribe(node);
                row = index.row();
            }
        }

        this->updateActionsAvailibility();
    }
}

// This method unsubscribe from selected node(s)
void VerseClient::on_actionUnSubscribe_triggered()
{
    if(this->states == STATE_CONNECTED) {
        QModelIndexList index_list = ui->nodeTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseNode *node = static_cast<VerseNode*>(index.internalPointer());
                this->verse_model->unsubscribe(node);
                // delete data stored in node
                node->removeNodeData();
                row = index.row();
            }
        }

        this->updateActionsAvailibility();
    }
}

// This method send node destroy for selected node(s)
void VerseClient::on_actionDestroy_triggered()
{
    if(this->states == STATE_CONNECTED) {
        QModelIndexList index_list = ui->nodeTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseNode *node = static_cast<VerseNode*>(index.internalPointer());
                // Only owner can destroy node
                if(ui->actionTesting_mode->isChecked() == true || node->getOwner() == this->user_id) {
                    vrs_send_node_destroy(this->session_id, node->getPrio(), node->getID());
                    break;
                }
                row = index.row();
            }
        }
    }
}

void VerseClient::on_tabWidget_destroyed()
{
    // TODO
}

void VerseClient::on_actionConnect_triggered()
{
    this->on_actionConnectDisconnect();
}

void VerseClient::on_actionDisconnect_triggered()
{
    this->on_actionConnectDisconnect();
}

void VerseClient::on_actionConnectDisconnect()
{
    if(this->states == STATE_DISCONNECTED) {
        int ret;
        this->states = STATE_CONNECTING;

        ui->connectButton->setText("Cancel");

        statusBar()->showMessage(tr("Connecting ..."), 0);

        ui->connectButton->setEnabled(true);
        ui->urlLineEdit->setEnabled(false);
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);

        ret = connectToServer(ui->urlLineEdit->text());
        if(ret != 1) {
            this->states = STATE_DISCONNECTED;
            ui->connectButton->setEnabled(true);
            ui->urlLineEdit->setEnabled(true);
            ui->connectButton->setText("Connect");
            statusBar()->showMessage(tr("Connecting failed"));
        }

    } else if(this->states == STATE_CONNECTING) {
        this->states = STATE_DISCONNECTED;

        ui->connectButton->setText("Connect");
        statusBar()->showMessage(tr("Disconnected"), 0);

        ui->urlLineEdit->setEnabled(true);
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);

        vrs_send_connect_terminate(this->session_id);

    } else {
        this->states = STATE_DISCONNECTING;

        statusBar()->showMessage(tr("Disconnecting..."), 0);

        ui->connectButton->setEnabled(false);
        ui->urlLineEdit->setEnabled(false);
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);

        vrs_send_connect_terminate(this->session_id);
    }
}

void VerseClient::on_urlLineEdit_returnPressed()
{
    this->on_actionConnectDisconnect();
}

// Open dialog for changing permission of the node
void VerseClient::on_actionPermissions_triggered()
{
    if(this->states == STATE_CONNECTED) {
        QModelIndexList index_list = ui->nodeTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseNode *node = static_cast<VerseNode*>(index.internalPointer());
                if(ui->actionTesting_mode->isChecked() == true || node->getOwner() == this->user_id) {
                    NodePermDialog *npd = new NodePermDialog(this, node);
                    node->addDialog(npd);
                    npd->show();
                    break;
                }
                row = index.row();
            }
        }
    }
}

// Open dialog for changing owner of the node
void VerseClient::on_actionOwner_triggered()
{
    if(this->states == STATE_CONNECTED) {
        QModelIndexList index_list = ui->nodeTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseNode *node = static_cast<VerseNode*>(index.internalPointer());
                if(ui->actionTesting_mode->isChecked() == true || node->getOwner() == this->user_id) {
                    NodeOwnerDialog *nod = new NodeOwnerDialog(this, node);
                    node->addDialog(nod);
                    nod->show();
                    break;
                }
                row = index.row();
            }
        }
    }
}

void VerseClient::on_actionLock_triggered()
{
    if(this->states == STATE_CONNECTED) {
        QModelIndexList index_list = ui->nodeTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseNode *node = static_cast<VerseNode*>(index.internalPointer());
                vrs_send_node_lock(this->session_id, node->getPrio(), node->getID());
                row = index.row();
            }
        }
    }
}

void VerseClient::on_actionUnLock_triggered()
{
    if(this->states == STATE_CONNECTED) {
        QModelIndexList index_list = ui->nodeTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseNode *node = static_cast<VerseNode*>(index.internalPointer());
                vrs_send_node_unlock(this->session_id, node->getPrio(), node->getID());
                row = index.row();
            }
        }
    }
}

void VerseClient::on_actionTesting_mode_triggered()
{
    this->updateActionsAvailibility();
}

void VerseClient::on_actionAuto_subscribe_triggered()
{
    if(ui->actionAuto_subscribe->isChecked() == true) {
        VerseNode *node;
        std::map<uint32_t, VerseNode*>::iterator node_iter;

        node_auto_subscribe = true;
        for(node_iter = this->verse_model->getNode(0)->nodes->begin();
            node_iter != this->verse_model->getNode(0)->nodes->end();
            node_iter++)
        {
            node = node_iter->second;
            if(node->getSubscribed() == false) {
                node->setSubscribed(true);
                vrs_send_node_subscribe(this->getSessionID(), node->getPrio(), node->getID(), 0, 0);
            }
        }
    } else {
        node_auto_subscribe = false;
    }

    this->updateActionsAvailibility();
}

void VerseClient::on_actionExpand_all_triggered()
{
    ui->nodeTreeView->expandAll();
}

void VerseClient::on_actionAuto_expand_triggered()
{
    if(this->node_auto_expand == false) {
        this->node_auto_expand = true;
        ui->nodeTreeView->expandAll();
    } else {
        this->node_auto_expand = false;
    }
}

void VerseClient::on_actionCreate_TagGroup_triggered()
{
    if(this->states == STATE_CONNECTED) {
        QModelIndexList index_list = ui->nodeTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseNode *node = static_cast<VerseNode*>(index.internalPointer());
                if(ui->actionTesting_mode->isChecked() == true || node->can_write == true) {
                    TagGroupDialog *tg_dig = new TagGroupDialog(this, node);
                    node->addDialog(tg_dig);
                    tg_dig->show();
                    break;
                }
                row = index.row();
            }
        }
    }
}

void VerseClient::on_actionDestroy_TagGroup_triggered()
{
    if(this->states == STATE_CONNECTED) {
        QModelIndexList index_list = ui->dataTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseData *data = static_cast<VerseData*>(index.internalPointer());
                if(data->getType() == VerseData::VERSE_TAG_GROUP) {
                    VerseTagGroup *tg = (VerseTagGroup*)data;
                    VerseNode *node = tg->getNode();
                    if(ui->actionTesting_mode->isChecked() == true || node->can_write == true) {
                        vrs_send_taggroup_destroy(this->session_id, node->getPrio(), node->getID(), tg->getID());
                        break;
                    }
                }
                row = index.row();
            }
        }
    }

    this->updateActionsAvailibility();
}

void VerseClient::on_actionSubscribe_TagGroup_triggered()
{
    if(this->states == STATE_CONNECTED) {
        QModelIndexList index_list = ui->dataTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseData *data = static_cast<VerseData*>(index.internalPointer());
                if(data->getType() == VerseData::VERSE_TAG_GROUP) {
                    VerseTagGroup *tg = (VerseTagGroup*)data;
                    VerseNode *node = tg->getNode();
                    if(tg->isSubscribed() == false) {
                        vrs_send_taggroup_subscribe(this->session_id, node->getPrio(), node->getID(), tg->getID(), 0, 0);
                        tg->setSubscribed(true);
                        break;
                    }
                }
                row = index.row();
            }
        }
    }

    this->updateActionsAvailibility();
}

void VerseClient::on_actionUnSubscribe_TagGroup_triggered()
{
    if(this->states == STATE_CONNECTED) {
        QModelIndexList index_list = ui->dataTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseData *data = static_cast<VerseData*>(index.internalPointer());
                if(data->getType() == VerseData::VERSE_TAG_GROUP) {
                    VerseTagGroup *tg = (VerseTagGroup*)data;
                    VerseNode *node = tg->getNode();
                    if(tg->isSubscribed() == true) {
                        // Send unsubscribe to the verse server
                        vrs_send_taggroup_unsubscribe(this->session_id, node->getPrio(), node->getID(), tg->getID(), 0);
                        tg->setSubscribed(false);
                        // Remove all tags from this tag group
                        tg->removeTags();
                        tg->removeData();
                        node->getDataModel()->update();
                        break;
                    }
                }
                row = index.row();
            }
        }
    }

    this->updateActionsAvailibility();
}

void VerseClient::on_actionCreate_Tag_triggered()
{
    if(this->states == STATE_CONNECTED) {
        QModelIndexList index_list = ui->dataTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseData *data = static_cast<VerseData*>(index.internalPointer());
                if(data->getType() == VerseData::VERSE_TAG_GROUP) {
                    VerseTagGroup *tg = (VerseTagGroup*)data;
                    VerseNode *node = tg->getNode();

                    TagDialog *td = new TagDialog(this, this->getSessionID(), node, tg);
                    node->addDialog(td);
                    td->show();
                } else if(data->getType() == VerseData::VERSE_TAG) {
                    VerseTag *tag = (VerseTag*)data;
                    VerseTagGroup *tg = tag->getTagGroup();
                    VerseNode *node = tg->getNode();

                    TagDialog *td = new TagDialog(this, this->getSessionID(), node, tg);
                    node->addDialog(td);
                    td->show();
                }
                row = index.row();
            }
        }
    }

    this->updateActionsAvailibility();
}

void VerseClient::on_actionDestroy_Tag_triggered()
{
    if(this->states == STATE_CONNECTED) {
        QModelIndexList index_list = ui->dataTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseData *data = static_cast<VerseData*>(index.internalPointer());
                if(data->getType() == VerseData::VERSE_TAG) {
                    VerseTag *tag = (VerseTag*)data;
                    VerseTagGroup *tg = tag->getTagGroup();
                    VerseNode *node = tg->getNode();

                    vrs_send_tag_destroy(this->getSessionID(),
                                           node->getPrio(),
                                           node->getID(),
                                           tg->getID(),
                                           tag->getID());
                }
                row = index.row();
            }
        }
    }

    this->updateActionsAvailibility();
}

void VerseClient::on_actionFPS_triggered()
{
    if(this->states == STATE_CONNECTED) {
        FPSDialog *fps_dialog = new FPSDialog(this);

        fps_dialog->show();
    }
}

void VerseClient::on_nodeTreeView_customContextMenuRequested(const QPoint &/*pos*/)
{
    if(this->states == STATE_CONNECTED) {
        // TODO: add custom context menu
        QMenu *menu = ui->menuNode;
        menu->exec(QCursor::pos());
    }
}

void VerseClient::on_dataTreeView_customContextMenuRequested(const QPoint &/*pos*/)
{
    if(this->states == STATE_CONNECTED) {
        // Get selected item in TreeView of data
        VerseData *data = NULL;
        QModelIndexList index_list = ui->dataTreeView->selectionModel()->selectedIndexes();
        int row = -1;
        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                data = static_cast<VerseData*>(index.internalPointer());
                row = index.row();
                break;
            }
        }

        // Was anything selected?
        if(data != NULL) {
            // Was TagGroup selected?
            if(data->type == VerseData::VERSE_TAG_GROUP) {
                // TODO: add custom context menu
                QMenu *menu = ui->menuTagGroup;
                menu->exec(QCursor::pos());
            } else if(data->type == VerseData::VERSE_TAG) {
                // TODO: add custom context menu
                QMenu *menu = ui->menuTag;
                menu->exec(QCursor::pos());
            }
        } else {
            // TODO: add custom context menu
            QMenu *menu = ui->menuTagGroup;
            menu->exec(QCursor::pos());
        }
    }
}

void VerseClient::on_actionChange_Value_triggered()
{
    if(this->states == STATE_CONNECTED) {
        QModelIndexList index_list = ui->dataTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseData *data = static_cast<VerseData*>(index.internalPointer());
                if(data->getType() == VerseData::VERSE_TAG) {
                    VerseTag *tag = (VerseTag*)data;
                    TagValueDialog *tvd = new TagValueDialog(this, this->getSessionID(), tag);
                    tag->getNode()->addDialog(tvd);
                    tvd->show();
                    break;
                }
            }
        }
    }
}

void VerseClient::on_actionCreate_Layer_triggered()
{
    if(this->states == STATE_CONNECTED) {
        QModelIndexList index_list = ui->nodeTreeView->selectionModel()->selectedIndexes();
        int row = -1;

        foreach(QModelIndex index, index_list) {
            if (index.row()!=row && index.column()==0) {
                VerseNode *node = static_cast<VerseNode*>(index.internalPointer());
                if(ui->actionTesting_mode->isChecked() == true || node->can_write == true) {
                    /*
                    TODO: create and show dialog for new layer
                    */
                    break;
                }
                row = index.row();
            }
        }
    }
}
