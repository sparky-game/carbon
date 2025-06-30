// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

#define TEST(name) CARBON_TEST(carbon_slotmap, name)

typedef struct {
  char name[8];
} NameComponent;

TEST(push_remove) {
  CBN_SlotMap names = carbon_slotmap_create(sizeof(NameComponent));
  carbon_should_be(0, names.size);
  carbon_should_be(names.size, names.data.size);

  // Push #1
  NameComponent name_1 = {"Wasym"};
  CBN_SlotMap_Key key_1 = carbon_slotmap_push(&names, &name_1);
  carbon_should_be(1, names.size);
  carbon_should_be(names.size, names.data.size);
  carbon_should_be(0, key_1.id);
  carbon_should_be(0, key_1.gen);

  // Push #2
  NameComponent name_2 = {"Alonso"};
  CBN_SlotMap_Key key_2 = carbon_slotmap_push(&names, &name_2);
  carbon_should_be(2, names.size);
  carbon_should_be(names.size, names.data.size);
  carbon_should_be(1, key_2.id);
  carbon_should_be(1, key_2.gen);

  // Push #3
  NameComponent name_3 = {"Miguel"};
  CBN_SlotMap_Key key_3 = carbon_slotmap_push(&names, &name_3);
  carbon_should_be(3, names.size);
  carbon_should_be(names.size, names.data.size);
  carbon_should_be(2, key_3.id);
  carbon_should_be(2, key_3.gen);

  // Remove `key_1`
  carbon_should_be_true(carbon_slotmap_remove(&names, key_1));
  carbon_should_be(2, names.size);
  carbon_should_be(names.size, names.data.size);
  carbon_should_be(0, names.freelist);
  carbon_should_be(4, names.generation);
  carbon_should_be(3, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 0).id);
  carbon_should_be(3, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 0).gen);
  carbon_should_be(1, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 1).id);
  carbon_should_be(1, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 1).gen);
  carbon_should_be(0, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 2).id);
  carbon_should_be(2, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 2).gen);

  // Remove again `key_1` (not valid, remains untouched)
  carbon_should_be_false(carbon_slotmap_remove(&names, key_1));
  carbon_should_be(2, names.size);
  carbon_should_be(names.size, names.data.size);
  carbon_should_be(0, names.freelist);
  carbon_should_be(4, names.generation);
  carbon_should_be(3, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 0).id);
  carbon_should_be(3, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 0).gen);
  carbon_should_be(1, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 1).id);
  carbon_should_be(1, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 1).gen);
  carbon_should_be(0, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 2).id);
  carbon_should_be(2, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 2).gen);

  // Remove `key_2`
  carbon_should_be_true(carbon_slotmap_remove(&names, key_2));
  carbon_should_be(1, names.size);
  carbon_should_be(names.size, names.data.size);
  carbon_should_be(1, names.freelist);
  carbon_should_be(5, names.generation);
  carbon_should_be(3, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 0).id);
  carbon_should_be(3, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 0).gen);
  carbon_should_be(0, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 1).id);
  carbon_should_be(4, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 1).gen);
  carbon_should_be(0, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 2).id);
  carbon_should_be(2, carbon_list_at_raw(CBN_SlotMap_Key, names.indices, 2).gen);

  // Push #4
  NameComponent name_4 = {"Javier"};
  CBN_SlotMap_Key key_4 = carbon_slotmap_push(&names, &name_4);
  carbon_should_be(2, names.size);
  carbon_should_be(names.size, names.data.size);
  carbon_should_be(1, key_4.id);
  carbon_should_be(5, key_4.gen);
  carbon_should_be(0, names.freelist);
  carbon_should_be(6, names.generation);
  carbon_should_be(2, carbon_list_at_raw(u64, names.erase, 0));
  carbon_should_be(1, carbon_list_at_raw(u64, names.erase, 1));

  // Push #5
  NameComponent name_5 = {"Alfred"};
  CBN_SlotMap_Key key_5 = carbon_slotmap_push(&names, &name_5);
  carbon_should_be(3, names.size);
  carbon_should_be(names.size, names.data.size);
  carbon_should_be(0, key_5.id);
  carbon_should_be(6, key_5.gen);
  carbon_should_be(3, names.freelist);
  carbon_should_be(7, names.generation);
  carbon_should_be(2, carbon_list_at_raw(u64, names.erase, 0));
  carbon_should_be(1, carbon_list_at_raw(u64, names.erase, 1));
  carbon_should_be(0, carbon_list_at_raw(u64, names.erase, 2));

  NameComponent name_1_r = {"N/A"};
  carbon_should_be_false(carbon_slotmap_lookup(&names, key_1, &name_1_r));
  carbon_should_be_s("N/A", name_1_r.name);

  NameComponent name_2_r = {"N/A"};
  carbon_should_be_false(carbon_slotmap_lookup(&names, key_2, &name_2_r));
  carbon_should_be_s("N/A", name_2_r.name);

  NameComponent name_3_r = {"N/A"};
  carbon_should_be_true(carbon_slotmap_lookup(&names, key_3, &name_3_r));
  carbon_should_be_s(name_3.name, name_3_r.name);

  NameComponent name_4_r = {"N/A"};
  carbon_should_be_true(carbon_slotmap_lookup(&names, key_4, &name_4_r));
  carbon_should_be_s(name_4.name, name_4_r.name);

  NameComponent name_5_r = {"N/A"};
  carbon_should_be_true(carbon_slotmap_lookup(&names, key_5, &name_5_r));
  carbon_should_be_s(name_5.name, name_5_r.name);

  carbon_slotmap_destroy(&names);
  return CARBON_OK;
}
