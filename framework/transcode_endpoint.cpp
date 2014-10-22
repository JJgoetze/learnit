/*
 * endpoint_transcode.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: shshang
 */

#include "framework/transcode_endpoint.h"

EndpointTranscode::EndpointTranscode() {
}

EndpointTranscode::EndpointTranscode(Transcode* kernel) : Transcode(kernel) {
}

EndpointTranscode::~EndpointTranscode() {
}

BinaryData* EndpointTranscode::doEncode(const BinaryData* message) {
	return doEndpointEncode();
}

BinaryData* EndpointTranscode::doDecode(const BinaryData* message) {
	doEndpointDecode(message);
	return nullptr;
}


