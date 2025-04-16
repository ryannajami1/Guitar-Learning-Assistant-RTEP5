#include <libwebsockets.h>
#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <chrono>

using namespace std;

static struct lws *gws = nullptr;
static int counter = 0;

static int callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    return 0; // We don't handle HTTP requests in this example
}

static int callback_ws(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            cout << "Connection established" << endl;
            gws = wsi;
            break;

        case LWS_CALLBACK_RECEIVE: {
            string receivedMessage((char *)in, len);
            lws_write(wsi, (unsigned char *)receivedMessage.c_str(), receivedMessage.length(), LWS_WRITE_TEXT);
            break;
        }

        case LWS_CALLBACK_CLOSED:
            cout << "Connection closed" << endl;
            gws = nullptr;
            break;

        default:
            break;
    }

    return 0;
}

static struct lws_protocols protocols[] = {
    {
        "http-only",
        callback_http,
        0,
    },
    {
        "ws",
        callback_ws,
        0,
    },
    {
        NULL,
        NULL,
        0,
    }
};

void send_counter_messages() {
    while (true) {
        this_thread::sleep_for(chrono::seconds(1));
        if (gws) {
            counter++;
            string message = to_string(counter);
            cout << "calling sent: " + message << endl;
            lws_write(gws, (unsigned char *)message.c_str(), message.length(), LWS_WRITE_TEXT);
        }
    }
}

int main() {
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof info);

    info.port = 9001;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;
    info.options |= LWS_SERVER_OPTION_VALIDATE_UTF8;

    struct lws_context *context = lws_create_context(&info);
    if (!context) {
        cerr << "Failed to create libwebsockets context" << endl;
        return 1;
    }

    cout << "Listening on port 9001" << endl;

    thread counter_thread(send_counter_messages);

    while (lws_service(context, 50) >= 0);

    lws_context_destroy(context);
    counter_thread.join();

    return 0;
}
