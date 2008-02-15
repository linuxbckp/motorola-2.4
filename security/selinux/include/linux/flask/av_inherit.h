/* This file is automatically generated.  Do not edit. */
/* FLASK */

typedef struct
{
    security_class_t tclass;
    char **common_pts;
    access_vector_t common_base;
} av_inherit_t;

static av_inherit_t av_inherit[] = {
   { SECCLASS_DIR, common_file_perm_to_string, 0x00020000UL },
   { SECCLASS_FILE, common_file_perm_to_string, 0x00020000UL },
   { SECCLASS_LNK_FILE, common_file_perm_to_string, 0x00020000UL },
   { SECCLASS_CHR_FILE, common_file_perm_to_string, 0x00020000UL },
   { SECCLASS_BLK_FILE, common_file_perm_to_string, 0x00020000UL },
   { SECCLASS_SOCK_FILE, common_file_perm_to_string, 0x00020000UL },
   { SECCLASS_FIFO_FILE, common_file_perm_to_string, 0x00020000UL },
   { SECCLASS_SOCKET, common_socket_perm_to_string, 0x00400000UL },
   { SECCLASS_TCP_SOCKET, common_socket_perm_to_string, 0x00400000UL },
   { SECCLASS_UDP_SOCKET, common_socket_perm_to_string, 0x00400000UL },
   { SECCLASS_RAWIP_SOCKET, common_socket_perm_to_string, 0x00400000UL },
   { SECCLASS_NETLINK_SOCKET, common_socket_perm_to_string, 0x00400000UL },
   { SECCLASS_PACKET_SOCKET, common_socket_perm_to_string, 0x00400000UL },
   { SECCLASS_KEY_SOCKET, common_socket_perm_to_string, 0x00400000UL },
   { SECCLASS_UNIX_STREAM_SOCKET, common_socket_perm_to_string, 0x00400000UL },
   { SECCLASS_UNIX_DGRAM_SOCKET, common_socket_perm_to_string, 0x00400000UL },
   { SECCLASS_IPC, common_ipc_perm_to_string, 0x00000200UL },
   { SECCLASS_SEM, common_ipc_perm_to_string, 0x00000200UL },
   { SECCLASS_MSGQ, common_ipc_perm_to_string, 0x00000200UL },
   { SECCLASS_SHM, common_ipc_perm_to_string, 0x00000200UL },
};

#define AV_INHERIT_SIZE (sizeof(av_inherit)/sizeof(av_inherit_t))


/* FLASK */