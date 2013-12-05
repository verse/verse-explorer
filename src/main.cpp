#include <QApplication> 

#include "verse_client.h"

#include <iostream>

VerseClient *client = NULL;

void cb_receive_tag_set_value(const uint8_t session_id,
        const uint32_t node_id,
        const uint16_t taggroup_id,
        const uint16_t tag_id,
        const uint8_t data_type,
        const uint8_t count,
        const void *value)
{
    if(client != NULL) {
        client->cbReceiveTagSet(session_id, node_id, taggroup_id, tag_id, data_type, count, value);
    }
}

void cb_receive_tag_create(const uint8_t session_id,
                           const uint32_t node_id,
                           const uint16_t taggroup_id,
                           const uint16_t tag_id,
                           const uint8_t data_type,
                           const uint8_t count,
                           const uint16_t tag_type)
{
    if(client != NULL) {
        client->cbReceiveTagCreate(session_id, node_id, taggroup_id, tag_id, data_type, count, tag_type);
    }
}

void cb_receive_tag_destroy(const uint8_t session_id,
                            const uint32_t node_id,
                            const uint16_t taggroup_id,
                            const uint16_t tag_id)
{
    if(client != NULL) {
        client->cbReceiveTagDestroy(session_id, node_id, taggroup_id, tag_id);
    }
}

void cb_receive_taggroup_create(const uint8_t session_id,
                                const uint32_t node_id,
                                const uint16_t taggroup_id,
                                const uint16_t tg_type)
{
    if(client!=NULL) {
        client->cbReceiveTaggroupCreate(session_id, node_id, taggroup_id, tg_type);
    }
}

void cb_receive_taggroup_destroy(const uint8_t session_id,
                                const uint32_t node_id,
                                const uint16_t taggroup_id)
{
    if(client!=NULL) {
        client->cbReceiveTaggroupDestroy(session_id, node_id, taggroup_id);
    }
}

void cb_receive_node_unlock(const uint8_t session_id,
                          const uint32_t node_id,
                          const uint32_t avatar_id)
{
    if(client!=NULL) {
        client->cbReceiveNodeUnLock(session_id, node_id, avatar_id);
    }
}

void cb_receive_node_lock(const uint8_t session_id,
                          const uint32_t node_id,
                          const uint32_t avatar_id)
{
    if(client!=NULL) {
        client->cbReceiveNodeLock(session_id, node_id, avatar_id);
    }
}

void cb_receive_node_owner(const uint8_t session_id,
        const uint32_t node_id,
        const uint16_t user_id)
{
    if(client!=NULL) {
        client->cbReceiveNodeOwner(session_id, node_id, user_id);
    }
}

void cb_receive_node_perm(const uint8_t session_id,
        const uint32_t node_id,
        const uint16_t user_id,
        const uint8_t perm)
{
    if(client!=NULL) {
        client->cbReceiveNodePerm(session_id, node_id, user_id, perm);
    }
}

void cb_receive_node_create(const uint8_t session_id,
        const uint32_t node_id,
        const uint32_t parent_id,
        const uint16_t user_id,
        const uint16_t type)
{
    if(client!=NULL) {
        printf("node: %d client_type: %d\n", node_id, type);
        client->cbReceiveNodeCreate(session_id, node_id, parent_id, user_id, type);
    }
}

void cb_receive_node_destroy(const uint8_t session_id,
        const uint32_t node_id)
{
    if(client!=NULL) {
        client->cbReceiveNodeDestroy(session_id, node_id);
    }
}

void cb_receive_node_link(const uint8_t session_id,
        const uint32_t parent_node_id,
        const uint32_t child_node_id)
{
    if(client!=NULL) {
        client->cbReceiveNodeLink(session_id, parent_node_id, child_node_id);
    }
}

void cb_receive_connect_accept(const uint8_t session_id,
                               const uint16_t user_id,
                               const uint32_t avatar_id)
{
    if(client != NULL) {
        client->cbReceiveConnectAccept(session_id, user_id, avatar_id);
    } else {
        std::cout << "client == NULL" << std::endl;
    }
}

void cb_receive_connect_terminate(const uint8_t session_id,
                                  const uint8_t error_num)
{
    if(client != NULL) {
        client->cbReceiveConnectTerminate(session_id, error_num);
    } else {
        std::cout << "client == NULL" << std::endl;
    }
}

void cb_receive_user_authenticate(const uint8_t session_id,
        const char *username,
        const uint8_t auth_methods_count,
        const uint8_t *methods)
{
    if(client != NULL) {
        client->cbReceiveUserAuthenticate(session_id, username, auth_methods_count, methods);
    } else {
        std::cout << "client == NULL" << std::endl;
    }
}


int main(int argc, char *argv[])
{
    // Set information about client
    std::string client_name = "Verse Explorer";
    std::string client_version = "0.1";
    vrs_set_client_info((char*)client_name.c_str(), (char*)client_version.c_str());
    // Set debug level
    vrs_set_debug_level(VRS_PRINT_DEBUG_MSG);

    // Register basic callback functions
    vrs_register_receive_connect_accept(cb_receive_connect_accept);
    vrs_register_receive_connect_terminate(cb_receive_connect_terminate);
    vrs_register_receive_user_authenticate(cb_receive_user_authenticate);

    // Register node callback functions
    vrs_register_receive_node_create(cb_receive_node_create);
    vrs_register_receive_node_destroy(cb_receive_node_destroy);
    vrs_register_receive_node_link(cb_receive_node_link);
    vrs_register_receive_node_perm(cb_receive_node_perm);
    vrs_register_receive_node_owner(cb_receive_node_owner);
    vrs_register_receive_node_lock(cb_receive_node_lock);
    vrs_register_receive_node_unlock(cb_receive_node_unlock);

    // Register taggroup callback functions
    vrs_register_receive_taggroup_create(cb_receive_taggroup_create);
    vrs_register_receive_taggroup_destroy(cb_receive_taggroup_destroy);

    // Register tag callback functions
    vrs_register_receive_tag_create(cb_receive_tag_create);
    vrs_register_receive_tag_destroy(cb_receive_tag_destroy);

    // Register tag set callback functions
    vrs_register_receive_tag_set_value(cb_receive_tag_set_value);

    QApplication a(argc, argv);
    VerseClient w;
    w.show();

    client = &w;
    
    return a.exec();
}
