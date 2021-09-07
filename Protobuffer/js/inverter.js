// source: devices.proto
/**
 * @fileoverview
 * @enhanceable
 * @suppress {missingRequire} reports error on implicit type usages.
 * @suppress {messageConventions} JS Compiler reports an error if a variable or
 *     field starts with 'MSG_' and isn't a translatable message.
 * @public
 */
// GENERATED CODE -- DO NOT EDIT!
/* eslint-disable */
// @ts-nocheck

goog.provide('proto.devices.Inverter');

goog.require('jspb.BinaryReader');
goog.require('jspb.BinaryWriter');
goog.require('jspb.Message');

/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.devices.Inverter = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.devices.Inverter, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.devices.Inverter.displayName = 'proto.devices.Inverter';
}



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.devices.Inverter.prototype.toObject = function(opt_includeInstance) {
  return proto.devices.Inverter.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.devices.Inverter} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.devices.Inverter.toObject = function(includeInstance, msg) {
  var f, obj = {
    timestamp: jspb.Message.getFloatingPointFieldWithDefault(msg, 1, 0.0),
    temperature: jspb.Message.getFloatingPointFieldWithDefault(msg, 2, 0.0),
    motorTemp: jspb.Message.getFloatingPointFieldWithDefault(msg, 3, 0.0),
    torque: jspb.Message.getFloatingPointFieldWithDefault(msg, 4, 0.0),
    speed: jspb.Message.getFloatingPointFieldWithDefault(msg, 5, 0.0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.devices.Inverter}
 */
proto.devices.Inverter.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.devices.Inverter;
  return proto.devices.Inverter.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.devices.Inverter} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.devices.Inverter}
 */
proto.devices.Inverter.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readFloat());
      msg.setTimestamp(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readFloat());
      msg.setTemperature(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readFloat());
      msg.setMotorTemp(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readFloat());
      msg.setTorque(value);
      break;
    case 5:
      var value = /** @type {number} */ (reader.readFloat());
      msg.setSpeed(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.devices.Inverter.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.devices.Inverter.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.devices.Inverter} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.devices.Inverter.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = /** @type {number} */ (jspb.Message.getField(message, 1));
  if (f != null) {
    writer.writeFloat(
      1,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 2));
  if (f != null) {
    writer.writeFloat(
      2,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 3));
  if (f != null) {
    writer.writeFloat(
      3,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 4));
  if (f != null) {
    writer.writeFloat(
      4,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 5));
  if (f != null) {
    writer.writeFloat(
      5,
      f
    );
  }
};


/**
 * optional float timestamp = 1;
 * @return {number}
 */
proto.devices.Inverter.prototype.getTimestamp = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 1, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.devices.Inverter} returns this
 */
proto.devices.Inverter.prototype.setTimestamp = function(value) {
  return jspb.Message.setField(this, 1, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.devices.Inverter} returns this
 */
proto.devices.Inverter.prototype.clearTimestamp = function() {
  return jspb.Message.setField(this, 1, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.devices.Inverter.prototype.hasTimestamp = function() {
  return jspb.Message.getField(this, 1) != null;
};


/**
 * optional float temperature = 2;
 * @return {number}
 */
proto.devices.Inverter.prototype.getTemperature = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 2, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.devices.Inverter} returns this
 */
proto.devices.Inverter.prototype.setTemperature = function(value) {
  return jspb.Message.setField(this, 2, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.devices.Inverter} returns this
 */
proto.devices.Inverter.prototype.clearTemperature = function() {
  return jspb.Message.setField(this, 2, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.devices.Inverter.prototype.hasTemperature = function() {
  return jspb.Message.getField(this, 2) != null;
};


/**
 * optional float motor_temp = 3;
 * @return {number}
 */
proto.devices.Inverter.prototype.getMotorTemp = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 3, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.devices.Inverter} returns this
 */
proto.devices.Inverter.prototype.setMotorTemp = function(value) {
  return jspb.Message.setField(this, 3, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.devices.Inverter} returns this
 */
proto.devices.Inverter.prototype.clearMotorTemp = function() {
  return jspb.Message.setField(this, 3, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.devices.Inverter.prototype.hasMotorTemp = function() {
  return jspb.Message.getField(this, 3) != null;
};


/**
 * optional float torque = 4;
 * @return {number}
 */
proto.devices.Inverter.prototype.getTorque = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 4, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.devices.Inverter} returns this
 */
proto.devices.Inverter.prototype.setTorque = function(value) {
  return jspb.Message.setField(this, 4, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.devices.Inverter} returns this
 */
proto.devices.Inverter.prototype.clearTorque = function() {
  return jspb.Message.setField(this, 4, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.devices.Inverter.prototype.hasTorque = function() {
  return jspb.Message.getField(this, 4) != null;
};


/**
 * optional float speed = 5;
 * @return {number}
 */
proto.devices.Inverter.prototype.getSpeed = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 5, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.devices.Inverter} returns this
 */
proto.devices.Inverter.prototype.setSpeed = function(value) {
  return jspb.Message.setField(this, 5, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.devices.Inverter} returns this
 */
proto.devices.Inverter.prototype.clearSpeed = function() {
  return jspb.Message.setField(this, 5, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.devices.Inverter.prototype.hasSpeed = function() {
  return jspb.Message.getField(this, 5) != null;
};


