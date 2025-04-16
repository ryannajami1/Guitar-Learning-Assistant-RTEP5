#include "websocket.hpp"

#include <cstdio>
#include <cstring>
#include <unistd.h>

int interrupted = 0;
struct lws_context *context = nullptr;
struct lws *global_wsi = nullptr;
int counter = 0;

auto CallbackWs(struct lws *wsi, enum lws_callback_reasons reason,
               void * /*user*/, void *incomingMsg, size_t /*len*/) -> int {
  switch (reason) {
  case LWS_CALLBACK_ESTABLISHED:
    global_wsi = wsi;
    counter = 0;
    lwsl_user("Client connected\n");
    break;

  case LWS_CALLBACK_RECEIVE:
    lwsl_user("Received: %s\n", (char *)incomingMsg);
    break;

  case LWS_CALLBACK_CLOSED:
    lwsl_user("Client disconnected\n");
    global_wsi = nullptr;
    break;

  default:
    break;
  }

  return 0;
}

void LWS_SendMessage(const std::string& message) {
  if (global_wsi == nullptr) {
    return;
  }

  std::array<unsigned char, LWS_PRE + MAX_PAYLOAD_SIZE + 1> buffer;
  size_t len = std::min(message.size(), static_cast<size_t>(MAX_PAYLOAD_SIZE));

  memcpy(&buffer[LWS_PRE], message.c_str(), len);

  lws_write(global_wsi, &buffer[LWS_PRE], len, LWS_WRITE_TEXT);
  lws_callback_on_writable(global_wsi);
}


// NOLINTNEXTLINE(modernize-avoid-c-arrays)
const struct lws_protocols kProtocols[] = {
    {
        "ws-protocol",
        CallbackWs,
        0,
        MAX_PAYLOAD_SIZE,
    },
    {nullptr, nullptr, 0, 0}
};
