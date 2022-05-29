#ifndef _I_INTERFACE_H
#define _I_INTERFACE_H

namespace faker_tsn {

class IInterface {
  public:
    virtual ~IInterface() = default;

    virtual std::string toString() = 0;
};

}  // namespace faker_tsn

#endif  // _I_INTERFACE_H