//
// Copyright (C) 2015 Red Hat, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Authors: Daniel Kopecek <dkopecek@redhat.com>
//
#include "RulePrivate.hpp"
#include "Common/Utility.hpp"

namespace usbguard {
  template<>
  String toRuleString(const String& value)
  {
    return Utility::quoteEscapeString(value);
  }

  const uint32_t Rule::RootID = std::numeric_limits<uint32_t>::min();
  const uint32_t Rule::DefaultID = std::numeric_limits<uint32_t>::max();
  const uint32_t Rule::LastID = std::numeric_limits<uint32_t>::max() - 2;
  const uint32_t Rule::ImplicitID = std::numeric_limits<uint32_t>::max() - 1;

  Rule::Rule()
  {
    d_pointer = new RulePrivate(*this);
  }

  Rule::~Rule()
  {
    delete d_pointer;
    d_pointer = nullptr;
  }

  Rule::Rule(const Rule& rhs)
  {
    d_pointer = new RulePrivate(*this, *rhs.d_pointer);
  }

  const Rule& Rule::operator=(const Rule& rhs)
  {
    RulePrivate* n_pointer = new RulePrivate(*this, *rhs.d_pointer);
    delete d_pointer;
    d_pointer = n_pointer;
    return *this;
  }

  void Rule::setRuleID(uint32_t rule_id)
  {
    d_pointer->setRuleID(rule_id);
  }

  uint32_t Rule::getRuleID() const
  {
    return d_pointer->getRuleID();
  }
 
  void Rule::setTarget(Rule::Target target)
  {
    d_pointer->setTarget(target);
  }
 
  Rule::Target Rule::getTarget() const
  {
    return d_pointer->getTarget();
  }

  void Rule::setDeviceID(const USBDeviceID& value)
  {
    d_pointer->setDeviceID(value);
  }

  const USBDeviceID& Rule::getDeviceID() const
  {
    return d_pointer->getDeviceID();
  }

  const Rule::Attribute<USBDeviceID>& Rule::attributeDeviceID() const
  {
    return d_pointer->attributeDeviceID();
  }

  Rule::Attribute<USBDeviceID>& Rule::attributeDeviceID()
  {
    return d_pointer->attributeDeviceID();
  }

  void Rule::setSerial(const String& value)
  {
    d_pointer->setSerial(value);
  }

  const String& Rule::getSerial() const
  {
    return d_pointer->getSerial();
  }

  const Rule::Attribute<String>& Rule::attributeSerial() const
  {
    return d_pointer->attributeSerial();
  }

  Rule::Attribute<String>& Rule::attributeSerial()
  {
    return d_pointer->attributeSerial();
  }

  void Rule::setName(const String& value)
  {
    d_pointer->setName(value);
  }

  const String& Rule::getName() const
  {
    return d_pointer->getName();
  }

  const Rule::Attribute<String>& Rule::attributeName() const
  {
    return d_pointer->attributeName();
  }

  Rule::Attribute<String>& Rule::attributeName()
  {
    return d_pointer->attributeName();
  }

  void Rule::setHash(const String& value)
  {
    d_pointer->setHash(value);
  }

  const String& Rule::getHash() const
  {
    return d_pointer->getHash();
  }

  const Rule::Attribute<String>& Rule::attributeHash() const
  {
    return d_pointer->attributeHash();
  }

  Rule::Attribute<String>& Rule::attributeHash()
  {
    return d_pointer->attributeHash();
  }

  void Rule::setParentHash(const String& value)
  {
    d_pointer->setParentHash(value);
  }

  const String& Rule::getParentHash() const
  {
    return d_pointer->getParentHash();
  }

  const Rule::Attribute<String>& Rule::attributeParentHash() const
  {
    return d_pointer->attributeParentHash();
  }

  Rule::Attribute<String>& Rule::attributeParentHash()
  {
    return d_pointer->attributeParentHash();
  }

  void Rule::setViaPort(const String& value)
  {
    d_pointer->setViaPort(value);
  }

  const String& Rule::getViaPort() const
  {
    return d_pointer->getViaPort();
  }

  const Rule::Attribute<String>& Rule::attributeViaPort() const
  {
    return d_pointer->attributeViaPort();
  }

  Rule::Attribute<String>& Rule::attributeViaPort()
  {
    return d_pointer->attributeViaPort();
  }

  const Rule::Attribute<USBInterfaceType>& Rule::attributeWithInterface() const
  {
    return d_pointer->attributeWithInterface();
  }

  Rule::Attribute<USBInterfaceType>& Rule::attributeWithInterface()
  {
    return d_pointer->attributeWithInterface();
  }

  const Rule::Attribute<RuleCondition>& Rule::attributeConditions() const
  {
    return d_pointer->attributeConditions();
  }

  Rule::Attribute<RuleCondition>& Rule::attributeConditions()
  {
    return d_pointer->attributeConditions();
  }

  void Rule::setTimeoutSeconds(uint32_t timeout_seconds)
  {
    d_pointer->setTimeoutSeconds(timeout_seconds);
  }

  uint32_t Rule::getTimeoutSeconds() const
  {
    return d_pointer->getTimeoutSeconds();
  }

  bool Rule::appliesTo(Pointer<const Rule> rhs) const
  {
    return appliesTo(*rhs);
  }

  bool Rule::appliesTo(const Rule& rhs) const
  {
    return d_pointer->appliesTo(rhs);
  }

  bool Rule::appliesTo(const Rule& rhs)
  {
    updateMetaDataCounters(/*applied=*/false, /*evaluated=*/true);
    return d_pointer->appliesTo(rhs);
  }

  bool Rule::isImplicit() const
  {
    return d_pointer->getRuleID() == Rule::DefaultID;
  }

  Rule::operator bool() const
  {
    return !(getTarget() == Target::Unknown ||
	     getTarget() == Target::Invalid);
  }

  String Rule::toString(bool invalid) const
  {
    return d_pointer->toString(invalid);
  }

  void Rule::updateMetaDataCounters(bool applied, bool evaluated)
  {
    d_pointer->updateMetaDataCounters(applied, evaluated);
  }

  Rule Rule::fromString(const String& rule_string)
  {
    return RulePrivate::fromString(rule_string);
  }

  RulePrivate* Rule::internal()
  {
    return d_pointer;
  }

  const RulePrivate* Rule::internal() const
  {
    return d_pointer;
  }

  static const std::vector<std::pair<String,Rule::Target> > target_ttable = {
    { "allow", Rule::Target::Allow },
    { "block", Rule::Target::Block },
    { "reject", Rule::Target::Reject },
    { "match", Rule::Target::Match },
    { "device", Rule::Target::Device }
  };

  const String Rule::targetToString(const Rule::Target target)
  {
    for (auto ttable_entry : target_ttable) {
      if (ttable_entry.second == target) {
	return ttable_entry.first;
      }
    }
    throw std::runtime_error("Invalid rule target string");
  }

  Rule::Target Rule::targetFromString(const String& target_string)
  {
    for (auto ttable_entry : target_ttable) {
      if (ttable_entry.first == target_string) {
	return ttable_entry.second;
      }
    }
    throw std::runtime_error("Invalid rule target string");
  }

  uint32_t Rule::targetToInteger(const Rule::Target target)
  {
    return static_cast<uint32_t>(target);
  }

  Rule::Target Rule::targetFromInteger(const uint32_t target_integer)
  {
    switch(target_integer) {
      case static_cast<uint32_t>(Rule::Target::Allow):
      case static_cast<uint32_t>(Rule::Target::Block):
      case static_cast<uint32_t>(Rule::Target::Reject):
      case static_cast<uint32_t>(Rule::Target::Match):
      case static_cast<uint32_t>(Rule::Target::Device):
        break;
      default:
        throw std::runtime_error("Invalid rule target integer value");
    }
    return static_cast<Rule::Target>(target_integer);
  }

  static const std::vector<std::pair<String,Rule::SetOperator> > set_operator_ttable = {
    { "all-of", Rule::SetOperator::AllOf },
    { "one-of", Rule::SetOperator::OneOf },
    { "none-of", Rule::SetOperator::NoneOf },
    { "equals", Rule::SetOperator::Equals },
    { "equals-ordered", Rule::SetOperator::EqualsOrdered },
    { "match", Rule::SetOperator::Match }
  };

  const String Rule::setOperatorToString(const Rule::SetOperator& op)
  {
    for (auto ttable_entry : set_operator_ttable) {
      if (ttable_entry.second == op) {
	return ttable_entry.first;
      }
    }
    throw std::runtime_error("Invalid set operator string");
  }

  Rule::SetOperator Rule::setOperatorFromString(const String& set_operator_string)
  {
    for (auto ttable_entry : set_operator_ttable) {
      if (ttable_entry.first == set_operator_string) {
	return ttable_entry.second;
      }
    }
    throw std::runtime_error("Invalid set operator string");
  }
} /* namespace usbguard */
