/**
 * Copyright (c) 2022 Luca Erbetta
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "CommManager.h"

#include <functional>

#include "EventBroker.h"
#include "Events.h"

using namespace nlohmann;
using namespace std::placeholders;

CommManager::CommManager(uint16_t port)
    : server(port, std::bind(&CommManager::messageHandler, this, _1))
{
    sEventBroker.subscribe(this, TOPIC_CAMERA_CONFIG);
    sEventBroker.subscribe(this, TOPIC_CAMERA_EVENT);
    sEventBroker.subscribe(this, TOPIC_MODE_STATE);
    sEventBroker.subscribe(this, TOPIC_HEARTBEAT);
}

CommManager::~CommManager() { sEventBroker.unsubscribe(this); }

void CommManager::doPostEvent(const EventPtr& ev)
{
    if(server.isConnected())
        server.send(ev->to_json());
}

void CommManager::messageHandler(const nlohmann::json& j)
{
    if (j.contains("event_id"))  // Event json
    {
        if (j.at("event_id").is_number_unsigned())
        {
            try
            {
                sBroker.post(jsonToEvent(j), TOPIC_REMOTE_CMD);
            }
            catch (nlohmann::json::exception& jsone)
            {
                LOG_ERR(log, "Could not parse JSON to Event: what: {} json: {}",
                        jsone.what(), j.dump());
            }
        }
        else
        {
            LOG_ERR(log, "Received invalid event: {}", j.dump());
        }
    }
}