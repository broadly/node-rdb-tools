// Copyright 2013 Danny Yates

//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at

//        http://www.apache.org/licenses/LICENSE-2.0

//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#include "nan.h"

using namespace v8;
using namespace node;


extern "C"
uint64_t crc64(uint64_t crc, const unsigned char *s, uint64_t l);


class Crc64 : public ObjectWrap {

public:

  static void Init(Handle<Object> exports) {
    Nan::HandleScope scope;

    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tmpl = Nan::New<v8::FunctionTemplate>(New);
    tmpl->SetClassName(Nan::New("Crc64").ToLocalChecked());
    tmpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tmpl, "push", Push);
    Nan::SetPrototypeMethod(tmpl, "value", GetValue);

    constructor.Reset(tmpl->GetFunction());
    exports->Set(Nan::New("Crc64").ToLocalChecked(), tmpl->GetFunction());
  }

private:

  Crc64() {
    crc = 0;
  }

  ~Crc64() {
  }


  static NAN_METHOD(New) {
    Crc64* instance = new Crc64();
    instance->Wrap( info.This() );
    info.GetReturnValue().Set( info.This() );
  }


  static NAN_METHOD(Push) {
    if (info.Length() != 1 || !Buffer::HasInstance(info[0])) {
      Nan::ThrowError("Expecting a single Buffer argument");
      return;
    }

    Crc64* instance = ObjectWrap::Unwrap<Crc64>(info.Holder());
    Local<Object> bytes = info[0]->ToObject();
    instance->crc = crc64(instance->crc, (unsigned char *)Buffer::Data(bytes), Buffer::Length(bytes));
    info.GetReturnValue().Set( Nan::Undefined() );
  }


  static NAN_METHOD(GetValue) {
    if (info.Length() != 0) {
      Nan::ThrowError("Unexpected arguments");
      return;
    }

    Crc64* instance = ObjectWrap::Unwrap<Crc64>(info.Holder());
    MaybeLocal<Object> BufferOut = Nan::CopyBuffer((char*)&(instance->crc), sizeof(uint64_t));
    info.GetReturnValue().Set( BufferOut.ToLocalChecked() );
  }


  static Nan::Persistent<v8::Function> constructor;
  uint64_t crc;
};


Nan::Persistent<v8::Function> Crc64::constructor;


extern "C" {

  static void init(Handle<Object> exports) {
    Crc64::Init(exports);
  }

  NODE_MODULE(Crc64, init)
};

