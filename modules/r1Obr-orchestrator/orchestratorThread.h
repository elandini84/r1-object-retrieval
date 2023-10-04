/*
 * Copyright (C) 2006-2020 Istituto Italiano di Tecnologia (IIT)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef R1OBR_ORCHESTRATOR_THREAD_H
#define R1OBR_ORCHESTRATOR_THREAD_H

#include <yarp/os/all.h>
#include "nav2loc.h"
#include "continuousSearch.h"
#include "chatBot.h"

using namespace yarp::os;
using namespace std;

class OrchestratorThread : public Thread, public TypedReaderCallback<Bottle>
{
private:
    enum R1_status 
    {
        R1_IDLE,
        R1_ASKING_NETWORK,
        R1_SEARCHING,
        R1_CONTINUOUS_SEARCH,
        R1_OBJECT_FOUND,
        R1_OBJECT_NOT_FOUND,
        R1_GOING,
        R1_STOP      
    };

public:
    enum R1_says 
    {
        // start_search,
        // stop_search,
        // reset_search,
        // resume_search,
        object_found_maybe,
        object_found_true,
        object_found_false,
        object_not_found,
        navigation_position,
        something_bad_happened,
        go_target_reached,
        fallback
    };

private:
    
    // Ports
    string                  m_sensor_network_rpc_port_name;
    RpcClient               m_sensor_network_rpc_port;

    string                  m_nextLoc_rpc_port_name;
    RpcClient               m_nextLoc_rpc_port;

    string                  m_goandfindit_rpc_port_name;
    RpcClient               m_goandfindit_rpc_port;

    string                  m_goandfindit_result_port_name;
    BufferedPort<Bottle>    m_goandfindit_result_port;

    string                  m_positive_outcome_port_name;
    BufferedPort<Bottle>    m_positive_outcome_port;

    string                  m_negative_outcome_port_name;
    BufferedPort<Bottle>    m_negative_outcome_port;

    string                  m_faceexpression_rpc_port_name;
    RpcClient               m_faceexpression_rpc_port;

    //Navigator to Home location
    Nav2Loc*                m_nav2loc;

    //Continuous Search thread
    ContinuousSearch*       m_continuousSearch;

    //Chat Bot
    ChatBot*                m_chat_bot;

    // Others
    R1_status               m_status;
    string                  m_object;
    Bottle                  m_request;
    Bottle                  m_result;
    bool                    m_where_specified;
    bool                    m_object_found;
    bool                    m_object_not_found;

    ResourceFinder&         m_rf;

public:
    //Contructor and distructor
    OrchestratorThread(ResourceFinder &rf);
    ~OrchestratorThread() = default;

    //methods inherited from Thread
    virtual void run() override;
    virtual bool threadInit() override;
    virtual void threadRelease() override;
    virtual void onStop() override;

    //Port inherited from TypedReaderCallback
    using TypedReaderCallback<Bottle>::onRead;
    void onRead(Bottle& b) override;

    Bottle      forwardRequest(const Bottle& b);
    void        search(const Bottle& btl);
    bool        resizeSearchBottle(const Bottle& btl);
    bool        askNetwork();
    string      stopOrReset(const string& cmd);
    string      resetHome();
    string      resume();
    void        objectFound();
    
    void        setObject(string obj);
    string      getWhat();
    string      getWhere();
    string      getStatus();
    void        info(Bottle& reply);
    
    void        setEmotion();
    
    bool        askChatBotToSpeak(R1_says stat);

    bool        go(string loc);

};

#endif
