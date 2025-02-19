#include "RelayAddressListener.h"

#include <dds/rtpsrelaylib/RelayTypeSupportImpl.h>

#include <dds/DCPS/DCPS_Utils.h>
#include <dds/DCPS/TimeTypes.h>

namespace RtpsRelay {

RelayAddressListener::RelayAddressListener(
  RelayPartitionTable& relay_partition_table,
  RelayStatisticsReporter& relay_statistics_reporter)
  : relay_partition_table_(relay_partition_table)
  , relay_statistics_reporter_(relay_statistics_reporter)
{}

void RelayAddressListener::on_data_available(DDS::DataReader_ptr reader)
{
  RelayAddressDataReader_var dr = RelayAddressDataReader::_narrow(reader);
  if (!dr) {
    ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: RelayAddressListener::on_data_available failed to narrow RtpsRelay::ReaderEntryDataReader\n")));
    return;
  }

  RelayAddressSeq datas;
  DDS::SampleInfoSeq infos;
  DDS::ReturnCode_t ret = dr->take(datas,
                                   infos,
                                   DDS::LENGTH_UNLIMITED,
                                   DDS::NOT_READ_SAMPLE_STATE,
                                   DDS::ANY_VIEW_STATE,
                                   DDS::ANY_INSTANCE_STATE);
  if (ret != DDS::RETCODE_OK) {
    ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: RelayAddressListener::on_data_available failed to take %C\n"), OpenDDS::DCPS::retcode_to_string(ret)));
    return;
  }

  for (CORBA::ULong idx = 0; idx != infos.length(); ++idx) {
    const auto& data = datas[idx];
    const auto& info = infos[idx];

    switch (info.instance_state) {
    case DDS::ALIVE_INSTANCE_STATE:
      if (info.valid_data) {
        relay_partition_table_.insert(data.relay_id(),
                                      data.name(),
                                      ACE_INET_Addr(data.address().c_str()));
      }
      break;
    case DDS::NOT_ALIVE_DISPOSED_INSTANCE_STATE:
    case DDS::NOT_ALIVE_NO_WRITERS_INSTANCE_STATE:
      relay_partition_table_.remove(data.relay_id(),
                                    data.name());
      break;
    }
  }
}

void RelayAddressListener::on_subscription_matched(
  DDS::DataReader_ptr /*reader*/, const DDS::SubscriptionMatchedStatus& status)
{
  relay_statistics_reporter_.relay_address_pub_count(
    static_cast<uint32_t>(status.total_count), OpenDDS::DCPS::MonotonicTimePoint::now());
}


}
