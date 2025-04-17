#include "ChordDetection.hpp"
#include "audio_input.hpp"
#include "guitarfft.hpp"
#include "websocket.hpp"
#include <cstring>
#include <iomanip>
#include <iostream>
#include <libwebsockets.h>
#include <thread>

auto main() -> int {

  // Setup websocket
  struct lws_context_creation_info info;
  memset(&info, 0, sizeof(info));
  info.port = 9000;
  info.protocols = kProtocols;
  info.options = LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;

  context = lws_create_context(&info);
  if (context == nullptr) {
    lwsl_err("lws init failed\n");
    return 1;
  }

  // Create processor for Sampled frames at 2kHz
  // Will collect frames (total 1024 samples) before processing
  GuitarFFTProcessor processor(FRAMES, 2000, FRAMES);
  AudioInput audio_in;

  // Initialize the processor
  if (!processor.Initialize()) {
    std::cerr << "Failed to initialize FFT processor" << std::endl;
    return 1;
  }

  audio_in.register_callback(
      [&processor](auto frame) { processor.ProcessFrames(frame); });

      audio_in.init();

  // Audio input thread
  std::thread t_audio([&audio_in]() { audio_in.start_loop(); });

  // Websocket thread
  std::thread t_websocket([]() {
    while (true) {
      lws_service(context, 100);
    }
  });

  t_audio.join();
  t_websocket.join();

  // Destroy websocket context
  lws_context_destroy(context);

  return 0;
}
