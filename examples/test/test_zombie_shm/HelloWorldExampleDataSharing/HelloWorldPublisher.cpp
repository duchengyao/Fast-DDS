// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file HelloWorldPublisher.cpp
 *
 */

#include "HelloWorldPublisher.h"
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/PublisherAttributes.h>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>

#include <thread>

using namespace eprosima::fastdds::dds;

HelloWorldPublisher::HelloWorldPublisher()
    : participant_(nullptr), publisher_(nullptr), topic_(nullptr), writer_(nullptr), type_(new HelloWorldPubSubType()) {
}

bool HelloWorldPublisher::init() {
  hello_.index(0);
  hello_.message("HelloWorld");
  DomainParticipantQos pqos;
  pqos.name("Participant_pub");
  participant_ = DomainParticipantFactory::get_instance()->create_participant(0, pqos);

  if (participant_ == nullptr) {
    return false;
  }

  //REGISTER THE TYPE
  type_.register_type(participant_);

  //CREATE THE PUBLISHER
  publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);

  if (publisher_ == nullptr) {
    return false;
  }

  topic_ = participant_->create_topic("HelloWorldTopic", "HelloWorld", TOPIC_QOS_DEFAULT);

  if (topic_ == nullptr) {
    return false;
  }

  // CREATE THE WRITER
  DataWriterQos wqos = DATAWRITER_QOS_DEFAULT;
  wqos.reliability().kind = BEST_EFFORT_RELIABILITY_QOS;
  wqos.history().depth = 10;
  wqos.data_sharing().automatic();
  writer_ = publisher_->create_datawriter(topic_, wqos, &listener_);

  if (writer_ == nullptr) {
    return false;
  }
  return true;
}

HelloWorldPublisher::~HelloWorldPublisher() {
  if (writer_ != nullptr) {
    publisher_->delete_datawriter(writer_);
  }
  if (publisher_ != nullptr) {
    participant_->delete_publisher(publisher_);
  }
  if (topic_ != nullptr) {
    participant_->delete_topic(topic_);
  }
  DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

void HelloWorldPublisher::PubListener::on_publication_matched(
    eprosima::fastdds::dds::DataWriter*,
    const eprosima::fastdds::dds::PublicationMatchedStatus& info) {
  if (info.current_count_change == 1) {
    matched_ = info.total_count;
    firstConnected_ = true;
    std::cout << "Publisher matched." << std::endl;
  } else if (info.current_count_change == -1) {
    matched_ = info.total_count;
    std::cout << "Publisher unmatched." << std::endl;
  } else {
    std::cout << info.current_count_change
              << " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
  }
}

[[noreturn]] void HelloWorldPublisher::run() {
  while (true) {

    if (listener_.firstConnected_ || listener_.matched_ > 0) {
      hello_.index(hello_.index() + 1);
      writer_->write(&hello_);
    }

    std::cout << "Message: " << hello_.message() << " with index: " << hello_.index()
              << " SENT" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }
}

int main() {
  HelloWorldPublisher hello_world_publisher;
  hello_world_publisher.init();
  hello_world_publisher.run();
}