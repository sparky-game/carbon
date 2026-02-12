// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#undef TEST
#define TEST(x) CARBON_TEST(carbon_slotmap, x)

typedef struct {
  char name[8];
} NameComponent;

TEST(push_remove) {
  auto names = cbn::SlotMap<NameComponent>::make();
  carbon_should_be(sizeof(typeof(names)::value_type), names.stride);
  carbon_should_be(0, names.size);
  carbon_should_be(names.size, names.data.size);

  // Push #1
  typeof(names)::value_type name_1 = {"Wasym"};
  auto key_1 = names.Push(name_1);
  carbon_should_be(1, names.size);
  carbon_should_be(names.size, names.data.size);
  carbon_should_be(0, key_1.id);
  carbon_should_be(0, key_1.gen);

  // Push #2
  typeof(names)::value_type name_2 = {"Alonso"};
  auto key_2 = names.Push(name_2);
  carbon_should_be(2, names.size);
  carbon_should_be(names.size, names.data.size);
  carbon_should_be(1, key_2.id);
  carbon_should_be(1, key_2.gen);

  // Push #3
  typeof(names)::value_type name_3 = {"Miguel"};
  auto key_3 = names.Push(name_3);
  carbon_should_be(3, names.size);
  carbon_should_be(names.size, names.data.size);
  carbon_should_be(2, key_3.id);
  carbon_should_be(2, key_3.gen);

  // Remove `key_1`
  carbon_should_be_true(names.Remove(key_1));
  carbon_should_be(2, names.size);
  carbon_should_be(names.size, names.data.size);
  carbon_should_be(0, names.freelist);
  carbon_should_be(4, names.generation);
  carbon_should_be(3, (*(typeof(names)::indices_type *) &names.indices)[0].id);
  carbon_should_be(3, (*(typeof(names)::indices_type *) &names.indices)[0].gen);
  carbon_should_be(1, (*(typeof(names)::indices_type *) &names.indices)[1].id);
  carbon_should_be(1, (*(typeof(names)::indices_type *) &names.indices)[1].gen);
  carbon_should_be(0, (*(typeof(names)::indices_type *) &names.indices)[2].id);
  carbon_should_be(2, (*(typeof(names)::indices_type *) &names.indices)[2].gen);

  // Remove again `key_1` (not valid, remains untouched)
  carbon_should_be_false(names.Remove(key_1));
  carbon_should_be(2, names.size);
  carbon_should_be(names.size, names.data.size);
  carbon_should_be(0, names.freelist);
  carbon_should_be(4, names.generation);
  carbon_should_be(3, (*(typeof(names)::indices_type *) &names.indices)[0].id);
  carbon_should_be(3, (*(typeof(names)::indices_type *) &names.indices)[0].gen);
  carbon_should_be(1, (*(typeof(names)::indices_type *) &names.indices)[1].id);
  carbon_should_be(1, (*(typeof(names)::indices_type *) &names.indices)[1].gen);
  carbon_should_be(0, (*(typeof(names)::indices_type *) &names.indices)[2].id);
  carbon_should_be(2, (*(typeof(names)::indices_type *) &names.indices)[2].gen);

  // Remove `key_2`
  carbon_should_be_true(names.Remove(key_2));
  carbon_should_be(1, names.size);
  carbon_should_be(names.size, names.data.size);
  carbon_should_be(1, names.freelist);
  carbon_should_be(5, names.generation);
  carbon_should_be(3, (*(typeof(names)::indices_type *) &names.indices)[0].id);
  carbon_should_be(3, (*(typeof(names)::indices_type *) &names.indices)[0].gen);
  carbon_should_be(0, (*(typeof(names)::indices_type *) &names.indices)[1].id);
  carbon_should_be(4, (*(typeof(names)::indices_type *) &names.indices)[1].gen);
  carbon_should_be(0, (*(typeof(names)::indices_type *) &names.indices)[2].id);
  carbon_should_be(2, (*(typeof(names)::indices_type *) &names.indices)[2].gen);

  // Push #4
  typeof(names)::value_type name_4 = {"Javier"};
  auto key_4 = names.Push(name_4);
  carbon_should_be(2, names.size);
  carbon_should_be(names.size, names.data.size);
  carbon_should_be(1, key_4.id);
  carbon_should_be(5, key_4.gen);
  carbon_should_be(0, names.freelist);
  carbon_should_be(6, names.generation);
  carbon_should_be(2, (*(typeof(names)::erase_type *) &names.erase)[0]);
  carbon_should_be(1, (*(typeof(names)::erase_type *) &names.erase)[1]);

  // Push #5
  typeof(names)::value_type name_5 = {"Alfred"};
  auto key_5 = names.Push(name_5);
  carbon_should_be(3, names.size);
  carbon_should_be(names.size, names.data.size);
  carbon_should_be(0, key_5.id);
  carbon_should_be(6, key_5.gen);
  carbon_should_be(3, names.freelist);
  carbon_should_be(7, names.generation);
  carbon_should_be(2, (*(typeof(names)::erase_type *) &names.erase)[0]);
  carbon_should_be(1, (*(typeof(names)::erase_type *) &names.erase)[1]);
  carbon_should_be(0, (*(typeof(names)::erase_type *) &names.erase)[2]);

  auto name_1_r = names[key_1];
  carbon_should_be_false(name_1_r);

  auto name_2_r = names[key_2];
  carbon_should_be_false(name_2_r);

  auto name_3_r = names[key_3];
  carbon_should_be_true(name_3_r);
  carbon_should_be_s(name_3.name, name_3_r->name);

  auto name_4_r = names[key_4];
  carbon_should_be_true(name_4_r);
  carbon_should_be_s(name_4.name, name_4_r->name);

  auto name_5_r = names[key_5];
  carbon_should_be_true(name_5_r);
  carbon_should_be_s(name_5.name, name_5_r->name);

  names.Free();
  return true;
}
