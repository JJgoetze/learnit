#include <gtest/gtest.h>
#include "service/example_pimpl.h"

class ExampleServiceTest : public ::testing::Test {
protected:
	ExampleServiceTest() {}
	virtual ~ExampleServiceTest() {}

	virtual void SetUp() {
	}

	virtual void TearDown() {
	}

	void testHandleEnterRoom() {		
	}
	
    void testHandleLeaveRoom() {		
	}		
	
	void testHandleQueryTop20ByMoney() {		
	}
	
	void testHandleQueryAccountBeForbidened() {		
	}
};

TEST_F(ExampleServiceTest, testHandleEnterRoom) {
	testHandleEnterRoom();
}

TEST_F(ExampleServiceTest, testHandleLeaveRoom) {
	testHandleLeaveRoom();
}

TEST_F(ExampleServiceTest, testHandleQueryTop20ByMoney) {
	testHandleQueryTop20ByMoney();
}

TEST_F(ExampleServiceTest, testHandleQueryAccountBeForbidened) {
	testHandleQueryAccountBeForbidened();
}


