#include <QObject>
#include <QtCore/QCoreApplication>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

#include "MessageScheduler.hpp"
#include "WsClient.hpp"
#include "ClientNode.hpp"

// In a complete client, we expect these to be read from configuration
int max_queue_before_waiting = 1;
int verbosity = 2;
std::string host_url = "ws://localhost:8080";

void connect_client(
    WsClient& ws_client, 
    ClientNode& client_node,
    MessageScheduler& scheduler);

int main(int argc, char** argv) {
    QCoreApplication a(argc, argv);

    MessageScheduler scheduler(max_queue_before_waiting);

    // Websocket client setup.
    WsClient ws_client{QString::fromStdString(host_url)};
    
    // Initialize client node
    ClientNode client_node(verbosity);

    // Set up websocket and scheduler callbacks
    connect_client(ws_client, client_node, scheduler);

    return a.exec();
}

void connect_client(
    WsClient& ws_client, 
    ClientNode& client_node,
    MessageScheduler& scheduler) {
  // schedule messages
  QObject::connect(
      &client_node,
      &ClientNode::message_encoded,
      &scheduler,
      &MessageScheduler::enqueue);

  // run scheduler
  QObject::connect(
      &ws_client,
      &WsClient::backpressure_update,
      &scheduler,
      &MessageScheduler::backpressure_update);

  // send scheduled message
  QObject::connect(
      &scheduler,
      &MessageScheduler::scheduled,
      [&ws_client](const QByteArray& data) { ws_client.send_message(data); });

  // receive
  QObject::connect(
      &ws_client,
      &WsClient::message_received,
      &client_node,
      &ClientNode::message_received);

  // startup
  QObject::connect(
      &ws_client,
      &WsClient::connected,
      &client_node,
      &ClientNode::connected);
}