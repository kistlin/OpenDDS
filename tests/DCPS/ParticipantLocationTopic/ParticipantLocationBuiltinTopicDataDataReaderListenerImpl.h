/*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#ifndef PARTICIPANT_LOCATION_BUILTIN_TOPIC_DATA_DATAREADER_LISTENER_IMPL
#define PARTICIPANT_LOCATION_BUILTIN_TOPIC_DATA_DATAREADER_LISTENER_IMPL

#include <dds/DdsDcpsSubscriptionC.h>
#include <dds/DCPS/LocalObject.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class ParticipantLocationBuiltinTopicDataDataReaderListenerImpl
    : public virtual OpenDDS::DCPS::LocalObject<DDS::DataReaderListener>
{
public:
    //Constructor
    ParticipantLocationBuiltinTopicDataDataReaderListenerImpl(unsigned long& locations);

    //Destructor
    virtual ~ParticipantLocationBuiltinTopicDataDataReaderListenerImpl();

    virtual void on_requested_deadline_missed(
        DDS::DataReader_ptr reader,
        const DDS::RequestedDeadlineMissedStatus & status);

    virtual void on_requested_incompatible_qos(
        DDS::DataReader_ptr reader,
        const DDS::RequestedIncompatibleQosStatus & status);

    virtual void on_liveliness_changed(
        DDS::DataReader_ptr reader,
        const DDS::LivelinessChangedStatus & status);

    virtual void on_subscription_matched(
        DDS::DataReader_ptr reader,
        const DDS::SubscriptionMatchedStatus & status);

    virtual void on_sample_rejected(
        DDS::DataReader_ptr reader,
        const DDS::SampleRejectedStatus& status);

    virtual void on_data_available(
        DDS::DataReader_ptr reader);

    virtual void on_sample_lost(
        DDS::DataReader_ptr reader,
        const DDS::SampleLostStatus& status);

private:

    unsigned long& location_mask;

};

#endif /* PARTICIPANT_LOCATION_BUILTIN_TOPIC_DATA_DATAREADER_LISTENER_IMPL  */
