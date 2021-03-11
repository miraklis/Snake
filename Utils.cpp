#include "Utils.h"

namespace Utils {

    void Msg(string msg) {
        cout << "INFO: " << msg << endl;
    }

    void ErrorMsg(string msg, string err) {
        cout << "ERROR: " << msg << ":::" << err << endl;
    }

}