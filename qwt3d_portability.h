#ifndef QWT3D_PORTABILITY_H
#define QWT3D_PORTABILITY_H

#include <qnamespace.h>

#include <QMouseEvent>

namespace Qwt3D {
#define QWT3DLOCAL8BIT(qstring) \
    ((const char*)(qstring.toLocal8Bit()))

    const Qt::TextFlag SingleLine = Qt::TextSingleLine;

    class MouseState{
    public:
        MouseState(Qt::MouseButtons mb = Qt::NoButton, Qt::KeyboardModifiers km = Qt::NoModifier) : mb_(mb), km_(km){

        }

        MouseState(Qt::MouseButtons mb, Qt::KeyboardModifiers km = Qt::NoModifier) : mb_(mb), km_(km){

        }

        bool operator==(const MouseState& ms){
            return mb_ == ms.mb_ && km_ == ms.km_;
        }

        bool operator!=(const MouseState& ms){
            return !operator==(ms);
        }

    private:
        Qt::MouseButtons mb_;
        Qt::KeyboardModifiers km_;
    };

    class KeyboardState{
    public:
        KeyboardState(int key = Qt::Key_unknown, Qt::KeyboardModifiers km = Qt::NoModifier) : key_(key), km_(km){

        }
        bool operator==(const KeyboardState& ms){
            return key_ == ms.key_ && km_ == ms.km_;
        }

        bool operator!=(const KeyboardState& ms){
            return !operator==(ms);
        }

    private:
        int key_;
        Qt::KeyboardModifiers km_;
    };
}

#endif // QWT3D_PORTABILITY_H
