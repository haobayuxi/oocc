
#include "dtx.h"

bool lease_expired(uint64_t lease) { return lease > get_clock_sys_time_us(); }

bool DTX::LockReadOnly() {
  if (DrTM_lease == 0) {
    DrTM_lease = get_clock_sys_time_us() + 1000;
  }
  std::vector<CasRead> pending_cas_ro;
  std::vector<HashRead> pending_hash_ro;

  std::list<CasRead> pending_next_cas_ro;
  std::list<HashRead> pending_next_hash_ro;
  IssueLockReadOnly(pending_cas_ro, pending_hash_ro);
  context->Sync();

  return true;
}

bool DTX::IssueLockReadOnly(std::vector<CasRead> &pending_cas_ro,
                            std::vector<HashRead> &pending_hash_ro) {
  for (auto &item : read_only_set) {
    if (item.is_fetched) continue;
    auto it = item.item_ptr;
    node_id_t node_id = GetPrimaryNodeID(it->table_id);
    item.read_which_node = node_id;
    auto offset = addr_cache->Search(node_id, it->table_id, it->key);
    if (offset != NOT_FOUND) {
      it->remote_offset = offset;
      char *cas_buf = AllocLocalBuffer(sizeof(lock_t));
      char *data_buf = AllocLocalBuffer(DataItemSize);
      pending_cas_ro.emplace_back(
          CasRead{.item = it, .cas_buf = cas_buf, .data_buf = data_buf});
      context->CompareAndSwap(cas_buf,
                              GlobalAddress(node_id, GetRemoteLockAddr(offset)),
                              0, DrTM_lease);
      context->read(data_buf, GlobalAddress(node_id, offset), DataItemSize);
      context->PostRequest();
    } else {
      HashMeta meta = GetPrimaryHashMetaWithTableID(it->table_id);
      uint64_t idx = MurmurHash64A(it->key, 0xdeadbeef) % meta.bucket_num;
      offset_t node_off = idx * meta.node_size + meta.base_off;
      char *buf = AllocLocalBuffer(sizeof(HashNode));
      pending_hash_ro.emplace_back(HashRead{
          .node_id = node_id, .item = &item, .buf = buf, .meta = meta});
      context->read(buf, GlobalAddress(node_id, node_off), sizeof(HashNode));
      context->PostRequest();
    }
  }
  return true;
}

bool DTX::CheckLockReadOnly(std::vector<CasRead> &pending_cas_ro,
                            std::vector<HashRead> &pending_hash_ro,
                            std::list<CasRead> pending_next_cas_ro,
                            std::list<HashRead> pending_next_hash_ro) {
  //  check cas only

  return true;
}

bool DTX::CheckCasRO(std::vector<CasRead> &pending_cas_ro,
                     std::list<CasRead> pending_next_cas_ro) {
  for (auto &re : pending_cas_ro) {
    // get lease
  }
  return true;
}