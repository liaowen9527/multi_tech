#pragma once

class CVtCallback
{
public:
    CVtCallback() {};
    virtual ~CVtCallback() {};

public:
	virtual void OnClipLine(void* termline) = 0;
};
