#pragma once
#include <QByteArray>
#include <QHash>
#include <QObject>
#include <QString>
#include <chrono>
#include <deque>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "MessageSchedulerLib.hpp"
#include <functional>

/**
 * @brief Queues messages and schedules them on demand.
 *
 * Wraps the MesssageSchedulerLib in a QT interface, wiring up events as necessary
 */
class MessageScheduler : public QObject {
  Q_OBJECT;
 MessageSchedulerLib<QByteArray>* ms;
 public:
  MessageScheduler(uint64_t mq) {
    setupSchedulerLib(mq);
  }

  void setupSchedulerLib(uint64_t mq) {
    std::function<void(const QByteArray&)> bound_callback(std::bind(&MessageScheduler::scheduling_callback, this, std::placeholders::_1));
    ms = new MessageSchedulerLib<QByteArray>(mq, bound_callback);
  }

  void scheduling_callback(const QByteArray& data) {
    Q_EMIT scheduled(data);
  }

 Q_SIGNALS:
  void scheduled(const QByteArray& data);

 public Q_SLOTS:
  void enqueue(
      const QString& topic, const QByteArray& data, double priority, double rate_limit,
      bool no_drop) {
    ms->enqueue(topic.toUtf8().constData(), data, priority, rate_limit, no_drop);
  }

  void backpressure_update(uint64_t message_index, uint64_t last_ponged_index) {
    ms->backpressure_update(message_index, last_ponged_index);
  }

  void schedule() {
    ms->schedule();
  }
};
