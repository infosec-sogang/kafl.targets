#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <sys/types.h>
#include "json/json.h"
#include <windows.h>
#include "Config.h"
#include "Alloc.h"
#include "Output.h"
#include "SetupArg.h"



void nop(void) {

    //for (int i = 0; i < 10000000; i++) {
    //    continue;
    //}

}


void storeArg(Json::Value* argJson, uint64_t* curPtr, size_t ptrSize) {
    char buf[32];
    int id;
    size_t size, width, index, offset, count;
    uint64_t* ptr;
    Json::Value contentJson, arrayJson, elemJson, fieldJson;

    strncpy(buf, argJson->get("kind", NULL).asCString(), sizeof(buf));

    if (strcmp(buf, "funcptr") == 0) {
        *curPtr = (uint64_t)nop;
    }
    else if (strcmp(buf, "string") == 0) {
        char srcStr[MAX_PATH] = { 0, };
        strncpy(srcStr, argJson->get("val", NULL).asCString(), sizeof(srcStr));
        mbstowcs((wchar_t *)curPtr, srcStr, ptrSize / sizeof(wchar_t));
    }
    else if (strcmp(buf, "byte") == 0) {
        *(uint8_t*)curPtr = (uint8_t)argJson->get("val", NULL).asUInt();
    }
    else if (strcmp(buf, "word") == 0) {
        *(uint16_t*)curPtr = (uint16_t)argJson->get("val", NULL).asUInt();
    }
    else if (strcmp(buf, "dword") == 0) {
        *curPtr = argJson->get("val", NULL).asUInt();
    }
    else if (strcmp(buf, "qword") == 0) {
        *curPtr = argJson->get("val", NULL).asUInt64();
    }
    else if (strcmp(buf, "retval") == 0) {
        id = argJson->get("id", NULL).asInt();
        *curPtr = fetchOutput(id);
    }
    else if (strcmp(buf, "ptr") == 0) {

        size = argJson->get("size", NULL).asInt();
        ptr = (uint64_t*)alloc(size);
        if (argJson->isMember("val")) {
            contentJson = argJson->get("val", NULL);
            storeArg(&contentJson, ptr, size);
        }
        *(uint64_t**)curPtr = ptr;

    }
    else if (strcmp(buf, "rsc") == 0) {
        id = argJson->get("id", NULL).asInt();
        registerOutputPtr(curPtr, id);

    }
    else if (strcmp(buf, "struct") == 0) {
        arrayJson = argJson->get("val", NULL);
        for (index = 0; index < arrayJson.size(); ++index) {
            elemJson = arrayJson.get(index, NULL);
            offset = elemJson.get("offset", NULL).asInt();
            uint64_t* offset_ptr = (uint64_t*)((byte*)curPtr + offset);
            storeArg(&elemJson, offset_ptr, ptrSize - offset);
        }
    }
    else if(strcmp(buf, "array") == 0) {
        elemJson = argJson->get("val", NULL);
        count = argJson->get("count", NULL).asInt();
        width = argJson->get("width", NULL).asInt();
        for (index = 0; index < count; ++index) {
            uint64_t* offset_ptr = (uint64_t*)((byte*)curPtr + (width * index));
            storeArg(&elemJson, offset_ptr, width);
        }

    }
    else {
        printf("Unexpected argument kind to store: %s\n", buf);
        exit(1);
    }
}


uint64_t prepareArg(Json::Value* argJson) {
  char buf[64];
  size_t size;
  int id;
  uint64_t* ptr;
  Json::Value contentJson;

  strncpy(buf, argJson->get("kind", NULL).asCString(), sizeof(buf));

  if (strcmp(buf, "funcptr") == 0) {
    return (uint64_t)nop;
  }
  else if (strcmp(buf, "byte") == 0 || strcmp(buf, "word") == 0  || strcmp(buf, "dword") == 0 || strcmp(buf, "qword") == 0) {
      return argJson->get("val", NULL).asUInt64();
  }
  else if (strcmp(buf, "retval") == 0) {
    id = argJson->get("id", NULL).asInt();
    return fetchOutput(id);
  }
  else if (strcmp(buf, "ptr") == 0) {
    size = argJson->get("size", NULL).asInt();
    ptr = (uint64_t*)alloc(size);
    if (argJson->isMember("val")) {
        contentJson = argJson->get("val", NULL);
        storeArg(&contentJson, ptr, size);
    }
    return (uint64_t)ptr;
  }
  else {
    printf("Unexpected argument kind to prepare: %s\n", buf);
    exit(1);
  }

  return 0;
}
