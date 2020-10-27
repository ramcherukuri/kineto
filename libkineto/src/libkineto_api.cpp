/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "libkineto.h"

namespace libkineto {

LibkinetoApi& api() {
  static LibkinetoApi instance;
  return instance;
}

void LibkinetoApi::initClientIfRegistered() {
  if (client_) {
    if (clientRegisterThread_ != pthread_self()) {
      fprintf(
          stderr,
          "ERROR: External init callback must run in same thread as registerClient "
          "(%d != %d)\n",
          (int)pthread_self(),
          (int)clientRegisterThread_);
    } else {
      client_->init();
    }
  }
}

void LibkinetoApi::registerClient(ClientInterface* client) {
  client_ = client;
  if (client && activityProfiler_) {
    // Can initialize straight away
    client->init();
  }
  // Assume here that the external init callback is *not* threadsafe
  // and only call it if it's the same thread that called registerClient
  clientRegisterThread_ = pthread_self();
}


void LibkinetoApi::prepareTrace(const std::set<ActivityType>& activities) {
  activityProfiler_->prepareTrace(activities);
}

} // namespace libkineto
