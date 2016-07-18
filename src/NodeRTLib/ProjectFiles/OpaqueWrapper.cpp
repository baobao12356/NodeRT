// Copyright (c) Microsoft Corporation
// All rights reserved. 
//
// Licensed under the Apache License, Version 2.0 (the ""License""); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 
//
// THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT. 
//
// See the Apache Version 2.0 License for specific language governing permissions and limitations under the License.

#include "OpaqueWrapper.h"
#include "NodeRtUtils.h"

using v8::String;
using v8::FunctionTemplate;

Nan::Persistent<v8::FunctionTemplate> NodeRT::OpaqueWrapper::s_constructorTemplate;

void NodeRT::OpaqueWrapper::New(Nan::NAN_METHOD_ARGS_TYPE info)
{
  info.This()->SetHiddenValue(Nan::New<String>("__winrtOpaqueWrapper__").ToLocalChecked(), Nan::True());

  info.GetReturnValue().Set(info.This());
}


void  NodeRT::OpaqueWrapper::Init()
{
  Nan::HandleScope scope;
  // Prepare constructor template
  s_constructorTemplate = Nan::New<FunctionTemplate>(New);
  Nan::MaybeLocal<v8::FunctionTemplate> localRef = Nan::New<FunctionTemplate>(s_constructorTemplate);
  localRef.ToLocalChecked()->SetClassName(Nan::New<String>("OpaqueWrapper").ToLocalChecked());
  localRef.ToLocalChecked()->InstanceTemplate()->SetInternalFieldCount(1);
}

namespace NodeRT {
  v8::Handle<v8::Object> CreateOpaqueWrapper(::Platform::Object^ winRtInstance)
  {
    Nan::EscapableHandleScope scope;
    if (winRtInstance == nullptr)
    {
      return scope.Escape(Nan::Undefined());
    }

    v8::Handle<v8::Value> args[] = { Nan::Undefined() };
    if (OpaqueWrapper::s_constructorTemplate.IsEmpty())
    {
      OpaqueWrapper::Init();
    }

	  Nan::MaybeLocal<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(OpaqueWrapper::s_constructorTemplate);
    v8::Handle<v8::Object> objectInstance = localRef.ToLocalChecked()->GetFunction()->NewInstance(0, args);
    if (objectInstance.IsEmpty())
    {
      return scope.Escape(Nan::Undefined());
    }
    OpaqueWrapper* wrapperInstance = new OpaqueWrapper(winRtInstance);
    wrapperInstance->Wrap(objectInstance);
    return scope.Escape(objectInstance);
  }
}

