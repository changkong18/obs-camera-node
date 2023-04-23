#include <stdexcept>
#include <optional>
#include "virtual_output.h"

class Camera : public Napi::ObjectWrap<Camera> {
  private:
    VirtualOutput virtual_output;

  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports) {
        Napi::HandleScope scope(env);  
        Napi::Function func =
            DefineClass(env, "Camera", {
                InstanceMethod("close", &Camera::close),
                InstanceMethod("device", &Camera::device),
                InstanceMethod("native_fourcc", &Camera::native_fourcc),
                InstanceMethod("send", &Camera::send),
            });
        Napi::FunctionReference* constructor = new Napi::FunctionReference();
        *constructor = Napi::Persistent(func);
        exports.Set("Camera", func);
        env.SetInstanceData(constructor);
        return exports;
    }
    
    Camera(const Napi::CallbackInfo &info)
        : Napi::ObjectWrap<Camera>(info),
        virtual_output {
            info[0].As<Napi::Number>(),
            info[1].As<Napi::Number>(),
            info[2].As<Napi::Number>(),
            info[3].As<Napi::Number>(),
            info[4].ToString().Utf8Value()
        } { }
    
    void close(const Napi::CallbackInfo &info) {
        virtual_output.stop();
    }

    Napi::Value device(const Napi::CallbackInfo &info) {
        Napi::Env env = info.Env();
        return Napi::String::New(env, virtual_output.device());
    }

    Napi::Value native_fourcc(const Napi::CallbackInfo &info) {
        Napi::Env env = info.Env();
        return Napi::Number::New(env, virtual_output.native_fourcc());
    }

    void send(const Napi::CallbackInfo &info) {
        virtual_output.send(static_cast<uint8_t*>(info[0].As<Napi::ArrayBuffer>().Data()));
    }
};

Napi::Object Init (Napi::Env env, Napi::Object exports) {
    Camera::Init(env, exports);
    return exports;
}

NODE_API_MODULE(obs, Init)