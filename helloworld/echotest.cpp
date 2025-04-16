#include <csignal>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <libwebsockets.h>
#include <unistd.h>
#include <array>

static int interrupted = 0;
static struct lws_context *context;
static struct lws *global_wsi = nullptr;

#define MAX_PAYLOAD_SIZE 128

static int counter = 0;

static auto CallbackWs(struct lws *wsi, enum lws_callback_reasons reason,
                       void * /*user*/, void *incomingMsg, size_t /*len*/) -> int {
  switch (reason) {
  case LWS_CALLBACK_ESTABLISHED:
    global_wsi = wsi;
    counter = 0; // Reset counter on new connection
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

void SendFunction(int num) {
  if (global_wsi == nullptr) {
    return;
  }

  std::array<char, MAX_PAYLOAD_SIZE> msg;
  snprintf(msg.data(), msg.size(), "%d", num);  // FIXED

  std::array<unsigned char, LWS_PRE + MAX_PAYLOAD_SIZE + 1> buffer;
  size_t len = strlen(msg.data());              // FIXED
  strcpy(reinterpret_cast<char *>(&buffer[LWS_PRE]), msg.data());  // FIXED

  lws_write(global_wsi, &buffer[LWS_PRE], len, LWS_WRITE_TEXT);
  lws_callback_on_writable(global_wsi);
}

// NOLINTNEXTLINE(modernize-avoid-c-arrays)
static const struct lws_protocols kProtocols[] = {{
                                                      "ws-protocol",
                                                      CallbackWs,
                                                      0,
                                                      MAX_PAYLOAD_SIZE,
                                                  },
                                                  {nullptr, nullptr, 0, 0}};

void SignalHandler(int /*sig*/) { interrupted = 1; }

auto main() -> int {
  struct lws_context_creation_info info;
  memset(&info, 0, sizeof(info));
  info.port = 9000;
  info.protocols = kProtocols;
  info.options = LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;

  context = lws_create_context(&info);
  if (context == nullptr) {
    lwsl_err("lws init failed\n");
    return -1;
  }

  signal(SIGINT, SignalHandler);

  while (interrupted == 0) {
    lws_service(context, 100);
    SendFunction(counter++);
  }

  lws_context_destroy(context);
  return 0;
}
