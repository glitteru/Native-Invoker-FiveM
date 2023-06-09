#include <Windows.h>
#include <iostream>

// CREDIT : https://www.unknowncheats.me/forum/fivem/460504-native-invoker.html

class scrNativeCallContext
{
protected:
    void* m_pReturn;
    uint32_t m_nArgCount;
    void* m_pArgs;
    uint32_t m_nDataCount;

public:
    template<typename T>
    inline T GetArgument(int idx)
    {
        intptr_t* arguments = (intptr_t*)m_pArgs;

        return *(T*)&arguments[idx];
    }

    inline void* GetArgumentBuffer()
    {
        return m_pArgs;
    }

    template<typename T>
    inline void SetResult(int idx, T value)
    {
        intptr_t* returnValues = (intptr_t*)m_pReturn;

        *(T*)&returnValues[idx] = value;
    }

    inline int GetArgumentCount()
    {
        return m_nArgCount;
    }

    template<typename T>
    inline T GetResult(int idx)
    {
        intptr_t* returnValues = (intptr_t*)m_pReturn;

        return *(T*)&returnValues[idx];
    }

    inline void SetVectorResults() { }
};

class NativeContext :
    public scrNativeCallContext
{
private:
    enum
    {
        MaxNativeParams = 16,
        ArgSize = 4,
    };
    uint8_t m_TempStack[MaxNativeParams * ArgSize];

public:
    inline NativeContext()
    {
        m_pArgs = &m_TempStack;
        m_pReturn = &m_TempStack;
        m_nArgCount = 0;
        m_nDataCount = 0;
    }

    template <typename T>
    inline void Push(T value)
    {
        if (sizeof(T) > ArgSize)
        {
            throw "Argument has an invalid size";
        }
        else if (sizeof(T) < ArgSize)
        {
            *reinterpret_cast<uintptr_t*>(m_TempStack + ArgSize * m_nArgCount) = 0;
        }

        *reinterpret_cast<T*>(m_TempStack + ArgSize * m_nArgCount) = value;
        m_nArgCount++;
    }

    inline void Reverse()
    {
        uintptr_t tempValues[MaxNativeParams];
        uintptr_t* args = (uintptr_t*)m_pArgs;

        for (int i = 0; i < m_nArgCount; i++)
        {
            int target = m_nArgCount - i - 1;

            tempValues[target] = args[i];
        }

        memcpy(m_TempStack, tempValues, sizeof(m_TempStack));
    }

    template <typename T>
    inline T GetResult()
    {
        return *reinterpret_cast<T*>(m_TempStack);
    }
};

typedef void(__cdecl* NativeHandler)(scrNativeCallContext* context);

using GetNativeHandler_t = NativeHandler(__fastcall*)(__int64 hash);
using CallNativeHandler_t = void(__fastcall*)(NativeContext*);

struct pass
{
    template<typename ...T> pass(T...) {}
};

class NativeInvoke
{
private:

    static inline void Invoke(NativeContext* cxt, __int64 hash)
    {
        auto mod = GetModuleHandleA("rage-scripting-five.dll");
        auto inset = GetProcAddress(mod, "?GetNativeHandler@scrEngine@rage@@SAP6AXPEAVscrNativeCallContext@2@@Z_K@Z");

        NativeHandler fn = ((GetNativeHandler_t)(inset))(hash);

        if (fn != 0)
        {
            (fn)(cxt);
        }
    }

public:

    template<__int64 Hash, typename R, typename... Args>
    static inline R Invoke(Args... args)
    {
        NativeContext cxt;

        pass{ ([&]()
        {
            cxt.Push(args);
        }(), 1)... };

        Invoke(&cxt, Hash);

        return cxt.GetResult<R>();
    }

};