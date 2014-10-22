/*
 * endpoint_transcode.h
 *
 *  Created on: Nov 14, 2013
 *      Author: shshang
 */

#ifndef FRAMEWORK_SERVER_TRANSCODE_ENDPOINT_H_
#define FRAMEWORK_SERVER_TRANSCODE_ENDPOINT_H_

#include "framework/transcode.h"

class EndpointTranscode : public Transcode {
public:
	EndpointTranscode();
	explicit EndpointTranscode(Transcode* kernel);
	virtual ~EndpointTranscode();

	virtual void Reset() = 0;

protected:
	virtual BinaryData* Encode() = 0;
	virtual void Decode(const BinaryData* message) = 0;

	virtual BinaryData* doEncode(const BinaryData* message);
	virtual BinaryData* doDecode(const BinaryData* message);

	virtual BinaryData* doEndpointEncode() = 0;
	virtual void doEndpointDecode(const BinaryData* message) = 0;

private:
	friend class TcpConnection;
};


#endif  // FRAMEWORK_SERVER_TRANSCODE_ENDPOINT_H_
