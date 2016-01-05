/* Copyright 2013-present Barefoot Networks, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Antonin Bas (antonin@barefootnetworks.com)
 *
 */

#ifndef BM_SIM_INCLUDE_BM_SIM_FIELD_LISTS_H_
#define BM_SIM_INCLUDE_BM_SIM_FIELD_LISTS_H_

#include <boost/functional/hash.hpp>

#include <utility>  // for pair<>
#include <vector>
#include <unordered_set>

#include "phv.h"

class FieldList {
 public:
  struct field_t {
    header_id_t header;
    int offset;

    bool operator==(const field_t& other) const {
      return header == other.header && offset == other.offset;
    }

    bool operator!=(const field_t& other) const {
      return !(*this == other);
    }
  };

 public:
  typedef std::vector<field_t>::iterator iterator;
  typedef std::vector<field_t>::const_iterator const_iterator;
  typedef std::vector<field_t>::reference reference;
  typedef std::vector<field_t>::const_reference const_reference;
  typedef size_t size_type;

 public:
  void push_back_field(header_id_t header, int field_offset) {
    fields.push_back({header, field_offset});
    fields_set.insert({header, field_offset});
  }

  // iterators
  iterator begin() { return fields.begin(); }

  const_iterator begin() const { return fields.begin(); }

  iterator end() { return fields.end(); }

  const_iterator end() const { return fields.end(); }

  bool contains(header_id_t header, int field_offset) const {
    auto it = fields_set.find({header, field_offset});
    return it != fields_set.end();
  }

 private:
  struct FieldKeyHash {
    std::size_t operator()(const field_t& f) const {
      std::size_t seed = 0;
      boost::hash_combine(seed, f.header);
      boost::hash_combine(seed, f.offset);

      return seed;
    }
  };

 private:
  std::vector<field_t> fields{};
  std::unordered_set<field_t, FieldKeyHash> fields_set{};
};

#endif  // BM_SIM_INCLUDE_BM_SIM_FIELD_LISTS_H_