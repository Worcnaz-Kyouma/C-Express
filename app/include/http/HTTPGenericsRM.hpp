#ifndef CE_HTTPGENERICSRM_H
#define CE_HTPPGENERICSRM_H

class HTTPGenericsRM {
public:
    static HTTPGenericsRM* getHTTPGenericsRM(AvailableHTTPProtocols protocol);

    virtual void GRM_NoResourceFound(Request request, Response response) const;
};

#endif