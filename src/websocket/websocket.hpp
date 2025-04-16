#ifndef WEBSOCKET_HPP
#define WEBSOCKET_HPP

#include <libwebsockets.h>
#include <array>

constexpr int MAX_PAYLOAD_SIZE = 128;

extern int interrupted;
extern struct lws_context *context;
extern struct lws *global_wsi;
extern int counter;

extern const struct lws_protocols kProtocols[];

int CallbackWs(struct lws *wsi, enum lws_callback_reasons reason,
               void *user, void *incomingMsg, size_t len);

void LWS_SendMessage(const std::string& message);

#endif  // WEBSOCKET_HPP
