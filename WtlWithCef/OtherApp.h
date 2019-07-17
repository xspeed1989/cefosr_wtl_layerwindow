#pragma once
#include <include/cef_app.h>
class OtherApp
    : public CefApp
{
public:
    OtherApp();
    virtual ~OtherApp();

    IMPLEMENT_REFCOUNTING(OtherApp);
};

