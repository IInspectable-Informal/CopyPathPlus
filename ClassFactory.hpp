#pragma once

template<typename T>
class ClassFactory : public winrt::implements<ClassFactory<T>, IClassFactory>
{
public:
    HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject)
    {
        try
        {
            if (pUnkOuter)
            { return CLASS_E_NOAGGREGATION; }
            else
            { return winrt::make<T>().as(riid, ppvObject); }
        }
        catch (...)
        { return winrt::to_hresult(); }
    }
        
    HRESULT STDMETHODCALLTYPE LockServer(BOOL fLock)
    {
        if (fLock)
        { this->AddRef(); }
        else
        { this->Release(); }
        return S_OK;
    }
};
