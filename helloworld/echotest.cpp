#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libwebsockets.h>

struct per_session_data__echo {
    size_t len;
    unsigned char payload[2048];
};

static int callback_echo(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    struct per_session_data__echo *pss = (struct per_session_data__echo *)user;
    int flags = LWS_WRITE_TEXT;

    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            printf("Connection established\n");
            break;

        case LWS_CALLBACK_RECEIVE:
            printf("Received message: len=%zu\n", len);
            if (len < sizeof(pss->payload) - 1) {
                memcpy(pss->payload, in, len);
                pss->len = len;
                printf("Requesting writable callback from RECEIVE\n"); // Debug
                lws_callback_on_writable(wsi);
            } else {
                printf("Error: Message too large for buffer\n");
                return -1;
            }
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE:
            printf("Writeable callback: len=%zu\n", pss->len);
            if (pss->len > 0) {
                int m = lws_write(wsi, pss->payload, pss->len, (enum lws_write_protocol)flags);
                printf("lws_write result: %d\n", m);
                if (m < 0) {
                    fprintf(stderr, "LWS_WRITE ERROR: %d\n", m);
                    return -1;
                }
                pss->len = 0;
            }
            break;

        case LWS_CALLBACK_CLOSED:
            printf("Connection closed\n");
            break;

        default:
            break;
    }

    return 0;
}

static struct lws_protocols protocols[] = {
    {
        "echo-protocol",
        callback_echo,
        sizeof(struct per_session_data__echo),
    },
    { NULL, NULL, 0 }
};

int main(int argc, char **argv) {
    struct lws_context_creation_info info;
    struct lws_context *context;
    const char *address = "localhost";
    int port = 9000;

    memset(&info, 0, sizeof info);

    info.port = port;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;
    info.options = 0;

    context = lws_create_context(&info);
    if (context == NULL) {
        fprintf(stderr, "Failed to create context\n");
        return 1;
    }

    printf("WebSocket server started on %s:%d\n", address, port);

    while (lws_service(context, 50) >= 0) {
	printf("LWS_SERVICE");
    }

    lws_context_destroy(context);

    return 0;
}
