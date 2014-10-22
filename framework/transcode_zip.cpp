/*
 * zip_transcode.cpp
 *
 *  Created on: Nov 8, 2013
 *      Author: jszhang
 */

#include "framework/transcode_zip.h"

ZipTranscode::ZipTranscode () {
}

ZipTranscode::ZipTranscode (Transcode* kernel) : Transcode(kernel) {
}

ZipTranscode::~ZipTranscode () {

}

BinaryData* ZipTranscode::doEncode (const BinaryData* message) {
	LOG(INFO) << "Zip Encode Begin";
	size_t destLen = (message->offset + 12) * 1.001 + 1;                                             //waiting to be checked
	size_t lengthInData = sizeof(message->size) / sizeof(uint8_t);                                   //length stored in the front of Data[]
	size_t sizeOfEncodeDes = sizeof(*message) + lengthInData + destLen + PACKETLEN;
	BinaryData* encodeDes = reinterpret_cast<BinaryData*>(calloc(1, sizeOfEncodeDes));
	encodeDes->size = lengthInData + destLen + PACKETLEN;

	size_t sizeBeforeCompress = message->offset;

	memcpy(encodeDes->data + PACKETLEN, &sizeBeforeCompress, lengthInData);
	encodeDes->offset = destLen;
	LOG(INFO) << "before offset is " << encodeDes->offset;
	int32_t res = compress(encodeDes->data + lengthInData + PACKETLEN, &(encodeDes->offset), message->data, message->offset);
	encodeDes->offset += lengthInData + PACKETLEN;
	uint16_t lengthDataHead = encodeDes->offset;
	memcpy(encodeDes->data, &lengthDataHead, PACKETLEN);
	LOG(INFO) << "after offset is " << encodeDes->offset;
	if (res != Z_OK) {
		LOG(INFO) << "encode error code is " << res;
		free(encodeDes);
		encodeDes = nullptr;
	}
	LOG(INFO) << "Zip Encode Over";
	return encodeDes;
}

BinaryData* ZipTranscode::doDecode (const BinaryData* message) {
	LOG(INFO) << "Zip Decode Begin";
	uint16_t lengthDataHead = 0;
	if (message->offset > PACKETLEN) {
		memcpy(&lengthDataHead, message->data, PACKETLEN);
	} else {
		lengthDataHead = 0;
		return nullptr;
	}
	if(message->offset < lengthDataHead)
	{
		LOG(INFO) << "message is incomplete,aborting decoding";
		return nullptr;
	}
	size_t sizeBeforeCompress = 0;
	size_t lengthInData = sizeof(message->size) / sizeof(uint8_t);                                   //length stored in the front of Data[]

	memcpy(&sizeBeforeCompress, message->data + PACKETLEN, lengthInData);
	size_t sizeOfDecodeDes = sizeof(*message) + sizeBeforeCompress;
	BinaryData* decodeDes = reinterpret_cast<BinaryData*>(calloc(1, sizeOfDecodeDes));
	decodeDes->size = sizeBeforeCompress;
	decodeDes->offset = sizeBeforeCompress;
	int32_t res = uncompress(decodeDes->data, &(decodeDes->offset), message->data + lengthInData + PACKETLEN, message->offset - lengthInData - PACKETLEN);
	if(res != Z_OK)
	{
		LOG(INFO) << "decode error code is " << res;
		free(decodeDes);
		decodeDes = nullptr;
	}
	LOG(INFO) << "Zip Decode Over";
	return decodeDes;
}

