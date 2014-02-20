#ifndef VERSECLIENT_H
#define VERSECLIENT_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>

#include <verse.h>

#include "verse_model.h"

namespace Ui {
class VerseClient;
}

void cb_receive_node_lock(const uint8_t session_id,
                          const uint32_t node_id,
                          const uint32_t avatar_id);

void cb_receive_node_perm(const uint8_t session_id,
                          const uint32_t node_id,
                          const uint16_t user_id,
                          const uint8_t perm);

void cb_receive_node_create(const uint8_t session_id,
                            const uint32_t node_id,
                            const uint32_t parent_id,
                            const uint16_t user_id,
                            const uint16_t type);

void cb_receive_node_destroy(const uint8_t session_id,
                             const uint32_t node_id);

void cb_receive_node_link(const uint8_t session_id,
                          const uint32_t parent_node_id,
                          const uint32_t child_node_id);

void cb_receive_connect_accept(const uint8_t session_id,
                               const uint16_t user_id,
                               const uint32_t avatar_id);

void cb_receive_connect_terminate(const uint8_t session_id,
                                  const uint8_t error_num);

void cb_receive_user_authenticate(const uint8_t session_id,
                                  const char *username,
                                  const uint8_t auth_methods_count,
                                  const uint8_t *methods);


/**
  * The class for user
  */
class VerseUser
{
private:
    uint16_t      user_id;
public:
    VerseUser(uint16_t id) {this->user_id = id;}
    ~VerseUser() {}
    uint16_t getID(void) { return this->user_id; }
};

/**
  * \brief The class for main window
  */
class VerseClient : public QMainWindow
{
    Q_OBJECT
    
public:

    explicit VerseClient(QWidget *parent = 0);

    ~VerseClient();

    void cbReceiveConnectAccept(const uint8_t session_id,
                                const uint16_t user_id,
                                const uint32_t avatar_id);

    void cbReceiveConnectTerminate(const uint8_t session_id,
                                   const uint8_t error_num);

    void cbReceiveUserAuthenticate(const uint8_t session_id,
            const char *username,
            const uint8_t auth_methods_count,
            const uint8_t *methods);

    void cbReceiveNodeCreate(const uint8_t session_id,
            const uint32_t node_id,
            const uint32_t parent_id,
            const uint16_t user_id,
            const uint16_t _type);

    void cbReceiveNodeDestroy(const uint8_t session_id,
            const uint32_t node_id);

    void cbReceiveNodeLink(const uint8_t session_id,
            const uint32_t parent_node_id,
            const uint32_t child_node_id);

    void cbReceiveNodePerm(const uint8_t session_id,
                           const uint32_t node_id,
                           const uint16_t user_id,
                           const uint8_t perm);

    void cbReceiveNodeOwner(const uint8_t session_id,
                            const uint32_t node_id,
                            const uint16_t user_id);

    void cbReceiveNodeLock(const uint8_t session_id,
                           const uint32_t node_id,
                           const uint32_t avatar_id);

    void cbReceiveNodeUnLock(const uint8_t session_id,
                           const uint32_t node_id,
                           const uint32_t avatar_id);

    void cbReceiveTaggroupCreate(const uint8_t session_id,
                                    const uint32_t node_id,
                                    const uint16_t taggroup_id,
                                    const uint16_t taggroup_type);

    void cbReceiveTaggroupDestroy(const uint8_t session_id,
                                    const uint32_t node_id,
                                    const uint16_t taggroup_id);

    void cbReceiveTagCreate(const uint8_t session_id,
                               const uint32_t node_id,
                               const uint16_t taggroup_id,
                               const uint16_t tag_id,
                               const uint8_t data_type,
                               const uint8_t count,
                               const uint8_t client_type);

    void cbReceiveTagDestroy(const uint8_t session_id,
                                const uint32_t node_id,
                                const uint16_t taggroup_id,
                                const uint16_t tag_id);

    void cbReceiveTagSet(const uint8_t session_id,
                         const uint32_t node_id,
                         const uint16_t taggroup_id,
                         const uint16_t tag_id,
                         const uint8_t data_type,
                         const uint8_t count,
                         const void *value);

    void cbReceiveLayerCreate(const uint8_t session_id,
                              const uint32_t node_id,
                              const uint16_t parent_layer_id,
                              const uint16_t layer_id,
                              const uint8_t data_type,
                              const uint8_t count,
                              const uint16_t custom_type);

    void cbReceiveLayerDestroy(const uint8_t session_id,
                               const uint32_t node_id,
                               const uint16_t layer_id);

    void updateActionsAvailibility(void);

    uint8_t getSessionID(void) {return this->session_id;}

    uint32_t getAvatarID(void) {return this->avatar_id;}

    uint16_t getUserID(void) {return this->user_id;}

    QList<VerseUser*> getUserList(void) { return this->users; }

    float getFPS(void) { return this->fps; }

    void setFPS(float _fps);
    
private slots:

    void on_actionQuit_triggered();

    void on_connectButton_clicked();

    void callUpdate();

    void on_urlLineEdit_editingFinished();

    void on_actionCreate_triggered();

    void on_actionSubscribe_triggered();

    void on_actionUnSubscribe_triggered();

    void on_actionDestroy_triggered();

    void on_tabWidget_destroyed();

    void on_actionConnect_triggered();

    void on_actionDisconnect_triggered();

    void on_urlLineEdit_returnPressed();

    void on_actionPermissions_triggered();

    void on_actionOwner_triggered();

    void selectionChangedSlot(const QItemSelection &newSelection, const QItemSelection &oldSelection);

    void dataSelectionChangedSlot(const QItemSelection &newSelection, const QItemSelection &oldSelection);

    void on_actionLock_triggered();

    void on_actionUnLock_triggered();

    void on_actionTesting_mode_triggered();

    void on_actionAuto_subscribe_triggered();

    void on_actionExpand_all_triggered();

    void on_actionAuto_expand_triggered();

    void on_actionCreate_TagGroup_triggered();

    void on_actionDestroy_TagGroup_triggered();

    void on_actionSubscribe_TagGroup_triggered();

    void on_actionUnSubscribe_TagGroup_triggered();

    void on_actionCreate_Tag_triggered();

    void on_actionDestroy_Tag_triggered();

    void on_actionFPS_triggered();

    void on_nodeTreeView_customContextMenuRequested(const QPoint &);

    void on_dataTreeView_customContextMenuRequested(const QPoint &);

    void on_actionChange_Value_triggered();

    void on_actionCreate_Layer_triggered();

    void on_actionDestroy_Layer_triggered();

private:
    // UI stuff
    Ui::VerseClient *ui;

    void createStatusBar();

    void on_actionConnectDisconnect();

    // Verse stuff

    // States of connection
    enum connectionStates {
        STATE_RESERVED = 0,
        STATE_DISCONNECTED = 1,
        STATE_CONNECTING = 2,
        STATE_CONNECTED = 3,
        STATE_DISCONNECTING = 4
    } states;

    // Does client autosubscribe to new verse nodes
    bool node_auto_subscribe;

    // Does client automatically expand tree structure of nodes
    bool node_auto_expand;

    // Unique session ID
    uint8_t session_id;

    // User ID, that uses this client for current session
    uint16_t user_id;

    // Username
    QString username;

    // Avatar ID that uses this client for current sesstion
    uint32_t avatar_id;

    //
    int connectToServer(const QString &hostname);

    // Model holding all data
    VerseNodeTreeModel *verse_model;

    // List of users at verse server
    QList<VerseUser*> users;

    // Timer for calling verse_update()
    QTimer *timer;

    // FPS of connection
    float fps;
};

#endif // VERSECLIENT_H
