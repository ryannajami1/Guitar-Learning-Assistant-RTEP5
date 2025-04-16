// #include "App.h"
// #include <iostream>
// #include <thread>
// #include <chrono>

// struct PerSocketData
// {
//     /* Fill with user data */
// };

// void startServer(uWS::App &app)
// {
//     app.listen(9001, [](auto *listen_socket)
//                {
//         if (listen_socket) {
//             std::cout << "Listening on port " << 9001 << std::endl;
//         } });

//     app.run();
// }

// void sender(uWS::App &app) {
//     while(1) {
//         std::cout << "Main thread is doing other work..." << std::endl;
//         std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
//         // app.publish("/*", "TEST", uWS::OpCode::TEXT);
//     }
// }

// int main()
// {
//     // Create the app instance
//     uWS::App app({
//         .key_file_name = "misc/key.pem",
//         .cert_file_name = "misc/cert.pem",
//         .passphrase = "1234"
//     });

//     app.ws<PerSocketData>("/*", {/* Settings */
//                                  .compression = uWS::CompressOptions(uWS::DEDICATED_COMPRESSOR | uWS::DEDICATED_DECOMPRESSOR),
//                                  .maxPayloadLength = 100 * 1024 * 1024,
//                                  .idleTimeout = 16,
//                                  .maxBackpressure = 100 * 1024 * 1024,
//                                  .closeOnBackpressureLimit = false,
//                                  .resetIdleTimeoutOnSend = false,
//                                  .sendPingsAutomatically = true,
//                                  /* Handlers */
//                                  .upgrade = nullptr,
//                                  .open = [](auto * /*ws*/)
//                                  {
//             /* Open event here */ },
//                                  .message = [](auto *ws, std::string_view message, uWS::OpCode opCode)
//                                  {
//             ws->send(message, opCode, false); },
//                                  .dropped = [](auto * /*ws*/, std::string_view /*message*/, uWS::OpCode /*opCode*/) {},
//                                  .drain = [](auto * /*ws*/) {},
//                                  .ping = [](auto * /*ws*/, std::string_view) {},
//                                  .pong = [](auto * /*ws*/, std::string_view) {},
//                                  .close = [](auto * /*ws*/, int /*code*/, std::string_view /*message*/) {}});

//     // Start the server in a separate thread
//     std::thread serverThread(startServer, std::ref(app));
//     std::thread senderThread(sender, std::ref(app));

//     // Main thread can do other work here
//     // for (int i = 0; i < 10; ++i)

//     // Wait for the server thread to finish (if needed)
//     serverThread.join(); // This will block until the server thread finishes
//     senderThread.join();

//     return 0;
// }

#include "./uWebSockets/src/App.h"

using namespace std;

struct PerSocketData
{
};

uWS::WebSocket<false, true, PerSocketData> *gws = nullptr;

int counter = 0;

int main()
{
    auto loop = uWS::Loop::get();

    struct us_timer_t *delayTimer = us_create_timer((struct us_loop_t *)loop, 0, 0);

    us_timer_set(delayTimer, [](struct us_timer_t *)
                 {
                               if (gws) {
                                  counter += 1;
                                  cout << "calling sent: " + std::to_string(counter) << endl;
                                  gws->send(std::to_string(counter), uWS::OpCode::TEXT);
                               } }, 1000, 1000);

    uWS::App app;

    app.ws<PerSocketData>("/*", {.idleTimeout = 0,
                                 .sendPingsAutomatically = false,
                                 .open = [](auto *ws)
                                    { 
                                    cout << "Listening on 9001" << endl;
                                    gws = ws;
                                    },

                                 .message = [](auto *ws, std::string_view message, uWS::OpCode opCode)
                                 { ws->send(message, opCode, false); },

                                 .close = [](auto * /*ws*/, int /*code*/, std::string_view /*message*/)
                                 { gws = nullptr; }})

        .listen(9001, [](auto *) {});

    app.run();
}
