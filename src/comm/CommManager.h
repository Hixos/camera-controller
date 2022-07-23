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

#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>

#include "EventHandler.h"
#include "JsonTcpServer.h"
#include "PrintLogger.h"

class CommManager : public EventHandlerBase
{
public:
    CommManager(uint16_t port);

    CommManager(const CommManager& other) = delete;
    CommManager(CommManager&& other) = delete;

    CommManager& operator=(const CommManager& rhs) = delete;
    CommManager& operator=(CommManager&& rhs) = delete;

    ~CommManager();

protected:
    void doPostEvent(const EventPtr& ev) override;

private:
    void messageHandler(const nlohmann::json& j);

    JsonTcpServer server;

    PrintLogger log = Logging::getLogger("ComMgr");
};