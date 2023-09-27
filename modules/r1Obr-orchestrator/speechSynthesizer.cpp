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

#include "speechSynthesizer.h"

YARP_LOG_COMPONENT(SPEECH_SYNTHESIZER, "r1_obr.orchestrator.speechSynthesizer")


// ------------------------------------------------------ //
bool SpeechSynthesizer::configure(ResourceFinder &rf)
{
    //Defaults
    string device_nwc = "speechSynthesizer_nwc_yarp";
    string local_nwc = "/r1Obr-orchestrator/speech_sythesizer/";
    string remote_nwc = "/speechSynthesizer_nws_yarp";
    string language = "auto";
    string voice = "auto";
    double pitch = 100;
    double speed = 100;

    Property speechProp;
    if(!rf.check("SPEECH_SYNTHESIZER"))
    {
        yCWarning(SPEECH_SYNTHESIZER,"SPEECH_SYNTHESIZER section missing in ini file. Using the default values");
    }
    Searchable& speech_config = rf.findGroup("SPEECH_SYNTHESIZER");
    m_active = speech_config.check("active") ? (speech_config.find("active").asString() == "true") : false;
    
    if(m_active)
    {
        speechProp.put("device", speech_config.check("device", Value(device_nwc)));
        speechProp.put("local",  speech_config.check("local", Value()));
        speechProp.put("remote", speech_config.check("remote", Value(remote_nwc)));
        m_PolySpeech.open(speechProp);
        if(!m_PolySpeech.isValid())
        {
            yCWarning(SPEECH_SYNTHESIZER,"Error opening PolyDriver check parameters. Using the default values");
        }
        m_PolySpeech.view(m_iSpeech);
        if(!m_iSpeech){
            yCError(SPEECH_SYNTHESIZER,"Error opening ISpeechSynthesizer interface. Device not available");
            return false;
        }

        if(speech_config.check("language")) language = speech_config.find("language").asString();
        if(speech_config.check("voice")) voice = speech_config.find("voice").asString();
        if(speech_config.check("pitch")) pitch = speech_config.find("pitch").asFloat32();
        if(speech_config.check("speed")) speed = speech_config.find("speed").asFloat32();
           
        m_iSpeech->setLanguage(language);
        m_iSpeech->setVoice(voice);
        m_iSpeech->setPitch(pitch);
        m_iSpeech->setSpeed(speed);
    }
    
    return true;
}


// ------------------------------------------------------ //
void SpeechSynthesizer::close()
{    
    if(m_PolySpeech.isValid())
        m_PolySpeech.close();

    yCInfo(SPEECH_SYNTHESIZER, "Speech synthesizer thread released");
}


// ------------------------------------------------------ //
bool SpeechSynthesizer::say(string sentence)
{
    if (!m_active)
        return false;
    
    yarp::sig::Sound ss;
    m_iSpeech->synthesize(sentence,ss);

    return true;
}


// ------------------------------------------------------ //
bool SpeechSynthesizer::setLanguage(string language)
{
    m_iSpeech->setLanguage(language);
}


// ------------------------------------------------------ //
bool SpeechSynthesizer::setVoice(string voice)
{
    m_iSpeech->setVoice(voice);
}


// ------------------------------------------------------ //
bool SpeechSynthesizer::setPitch(double pitch)
{
    m_iSpeech->setPitch(pitch);
}


// ------------------------------------------------------ //
bool SpeechSynthesizer::setSpeed(double speed)
{
    m_iSpeech->setSpeed(speed);
}



