/**
 * @file Cosa/Event.hh
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2012, Mikael Patel
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
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 *
 * @section Description
 * Event data structure with type, source and value.
 *
 * This file is part of the Arduino Che Cosa project.
 */

#ifndef __COSA_EVENT_HH__
#define __COSA_EVENT_HH__

#include "Cosa/Types.h"
#include "Cosa/Queue.hh"

class Event {
public:
  /**
   * Size of event queue.
   */
  static const uint8_t QUEUE_MAX = 16;

  /**
   * Event types are added here. Typical mapping from interrupts to
   * events. Note that the event is not a global numbering
   * scheme. Instead depends on the receiving/sending party, the
   * protocol.  
   */
  enum {
    NULL_TYPE = 0,

    FALLING_TYPE,		// Digital Pins
    RISING_TYPE,
    CHANGE_TYPE,

    SAMPLE_REQUEST_TYPE,	// Analog Pins
    SAMPLE_COMPLETED_TYPE,

    WATCHDOG_TYPE,		// Watchdog and timers
    TIMEOUT_TYPE,

    BEGIN_TYPE,			// Finite State Machines
    END_TYPE,

    RUN_TYPE,			// Thread

    CONNECT_TYPE,		// Device drivers and protocol stacks
    DISCONNECT_TYPE,
    RECEIVE_REQUEST_TYPE,	
    RECEIVE_COMPLETED_TYPE,
    SEND_REQUEST_TYPE,
    SEND_COMPLETED_TYPE,

    OPEN_TYPE,			// Device drivers and storage
    CLOSE_TYPE,
    READ_REQUEST_TYPE,
    READ_COMPLETED_TYPE,
    WRITE_REQUEST_TYPE,
    WRITE_COMPLETED_TYPE,
    COMMAND_REQUEST_TYPE,
    COMMAND_COMPLETED_TYPE,

    SERVICE_REQUEST_TYPE,	// Servers
    SERVICE_RESPONSE_TYPE,
    
    USER_TYPE = 64,		// User defined events/messages, 64-254

    ERROR_TYPE = 255		// Error event
  } __attribute__((packed));

  /**
   * Event handler root class.
   */
  class Handler {

  public:
    /**
     * Default null event handler. Should be redefined by sub-classes.
     * Called by Event::dispatch(). 
     * @param[in] type the event type.
     * @param[in] value the event value.
     */
    virtual void on_event(uint8_t type, uint16_t value) {}
  };

private:
  uint8_t m_type;
  Handler* m_target;
  uint16_t m_value;

public:
  /**
   * Construct event with given type, source and value.
   * @param[in] type event identity.
   * @param[in] target event receiver.
   * @param[in] value event value.
   */
  Event(int8_t type = NULL_TYPE, Handler* target = 0, uint16_t value = 0) :
    m_type(type),
    m_target(target),
    m_value(value)
  {}

  /**
   * Return event type.
   * @return type.
   */
  int get_type() 
  { 
    return (m_type); 
  }

  /**
   * Return event target.
   * @return pointer.
   */
  Handler* get_target() 
  { 
    return (m_target); 
  } 

  /**
   * Return event value.
   * @return value.
   */
  uint16_t get_value() 
  { 
    return (m_value); 
  }

  /**
   * Return event environment pointer.
   * @return pointer
   */
  void* get_env() 
  { 
    return ((void*) m_value); 
  }

  /**
   * Dispatch event handler for target object.
   */
  void dispatch()
  {
    if (m_target != 0) m_target->on_event(m_type, m_value);
  }

  /**
   * Push an event with given type, source and value into the event queue.
   * Return true(1) if successful otherwise false(0).
   * @param[in] type event identity.
   * @param[in] target event target.
   * @param[in] value event value.
   * @return bool.
   */
  static bool push(uint8_t type, Handler* target, uint16_t value = 0)
  {
    Event event(type, target, value);
    return (queue.enqueue(&event));
  }

  /**
   * Push an event with given type, source and value into the event queue.
   * Return true(1) if successful otherwise false(0).
   * @param[in] type event identity.
   * @param[in] target event target.
   * @param[in] env event environment pointer.
   * @return bool.
   */
  static bool push(uint8_t type, Handler* target, void* env)
  {
    return (push(type, target, (uint16_t) env));
  }

  /**
   * Event queue of size QUEUE_MAX.
   */
  static Queue queue;
};

#endif

